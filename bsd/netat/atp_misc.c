/*
 * Copyright (c) 1999 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * "Portions Copyright (c) 1999 Apple Computer, Inc.  All Rights
 * Reserved.  This file contains Original Code and/or Modifications of
 * Original Code as defined in and that are subject to the Apple Public
 * Source License Version 1.0 (the 'License').  You may not use this file
 * except in compliance with the License.  Please obtain a copy of the
 * License at http://www.apple.com/publicsource and read it before using
 * this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON-INFRINGEMENT.  Please see the
 * License for the specific language governing rights and limitations
 * under the License."
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

/*
 *	Copyright (c) 1996-1998 Apple Computer, Inc.
 *	All Rights Reserved.
 *
 *	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF APPLE COMPUTER, INC.
 *	The copyright notice above does not evidence any actual or
 *	intended publication of such source code.
 */

#include <appletalk.h>
#include <atp.h>
#include <ddp.h>
#include <at_atp.h>
#include <atp_inc.h>

extern atlock_t atpgen_lock;
void atp_free();
void atp_send(struct atp_trans *);

/*
 *	The request timer retries a request, if all retries are used up
 *		it returns a NAK
 */

void
atp_req_timeout(trp)
register struct atp_trans *trp;
{
	int s;
	register gbuf_t *m;
	gref_t *gref;
	struct atp_state *atp;
	struct atp_trans *ctrp;

	if ((atp = trp->tr_queue) == 0)
		return;
	ATDISABLE(s, atp->atp_lock);
	if (atp->atp_flags & ATP_CLOSING) {
		ATENABLE(s, atp->atp_lock);
		return;
	}
	for (ctrp = atp->atp_trans_wait.head; ctrp; ctrp = ctrp->tr_list.next) {
		if (ctrp == trp)
			break;
	}
	if (ctrp != trp) {
		ATENABLE(s, atp->atp_lock);
		return;
	}

	if ((m = gbuf_cont(trp->tr_xmt)) == NULL)
	        m = trp->tr_xmt;               /* issued via the new interface */

	if (trp->tr_retry == 0) {
		trp->tr_state = TRANS_FAILED;
		if (m == trp->tr_xmt) {
			trp->tr_xmt = NULL;
l_notify:
				gbuf_wset(m,1);
				*gbuf_rptr(m) = 99;
				gbuf_set_type(m, MSG_DATA);
				gref = trp->tr_queue->atp_gref;
				ATENABLE(s, atp->atp_lock);
				atalk_putnext(gref, m);

			return;
		}
		dPrintf(D_M_ATP_LOW,D_L_INFO, ("atp_req_timeout: skt=%d\n",
			trp->tr_local_socket));
		m = trp->tr_xmt;
		switch(((ioc_t *)(gbuf_rptr(trp->tr_xmt)))->ioc_cmd) {
		case AT_ATP_ISSUE_REQUEST:
			trp->tr_xmt = NULL;
			if (trp->tr_queue->dflag)
				((ioc_t *)gbuf_rptr(m))->ioc_cmd = AT_ATP_REQUEST_COMPLETE;
			else if (trp->tr_bdsp == NULL) {
				ATENABLE(s, atp->atp_lock);
				gbuf_freem(m);
				if (trp->tr_rsp_wait)
#ifdef _AIX
					e_wakeup(&trp->tr_event);
#else
					thread_wakeup(&trp->tr_event);
#endif
				break;
			}
			ATENABLE(s, atp->atp_lock);
			atp_iocnak(trp->tr_queue, m, ETIMEDOUT);
			atp_free(trp);
			return;

		case AT_ATP_ISSUE_REQUEST_NOTE:
		case AT_ATP_ISSUE_REQUEST_DEF_NOTE:
		  /* AT_ATP_ISSUE_REQUEST_DEF_NOTE is used just for tickle */
			trp->tr_xmt = gbuf_cont(m);
			gbuf_cont(m) = NULL;
			goto l_notify;
		}
	} else {
		(AT_ATP_HDR(m))->bitmap = trp->tr_bitmap;

		if (trp->tr_retry != (unsigned int) ATP_INFINITE_RETRIES)
			trp->tr_retry--;
		ATENABLE(s, atp->atp_lock);
		atp_send(trp);
	}
}


/*
 *	atp_free frees up a request, cleaning up the queues and freeing
 *		the request packet
 *      always called at 'lock'
 */

void atp_free(trp)
register struct atp_trans *trp;
{
	register struct atp_state *atp;
	register int i;
	int s;
	
	dPrintf(D_M_ATP_LOW, D_L_TRACE,
		("atp_free: freeing trp 0x%x\n", (u_int) trp));
	ATDISABLE(s, atpgen_lock);
	if (trp->tr_tmo_func)
	        atp_untimout(atp_req_timeout, trp);
	atp = trp->tr_queue;

	ATP_Q_REMOVE(atp->atp_trans_wait, trp, tr_list);

	if (trp->tr_xmt) {
	  	gbuf_freem(trp->tr_xmt);
		trp->tr_xmt = NULL;
	}
	for (i = 0; i < 8; i++) {
	        if (trp->tr_rcv[i]) {
		        gbuf_freem(trp->tr_rcv[i]);
			trp->tr_rcv[i] = NULL;
		}
	}
	if (trp->tr_bdsp) {
		gbuf_freem(trp->tr_bdsp);
		trp->tr_bdsp = NULL;
	}

	ATENABLE(s, atpgen_lock);
	atp_trans_free(trp);

} /* atp_free */


/*
 *	atp_send transmits a request packet by queuing it (if it isn't already) and
 *		scheduling the queue
 */

void atp_send(trp)
register struct atp_trans *trp;
{
	gbuf_t *m;
	struct atp_state *atp;

	dPrintf(D_M_ATP_LOW, D_L_OUTPUT, ("atp_send: trp=0x%x, skt=%d\n",
		(u_int) trp->tr_queue, trp->tr_local_socket));

	if ((atp = trp->tr_queue) != 0) {
	  if (trp->tr_state == TRANS_TIMEOUT) {
	    if ((m = gbuf_cont(trp->tr_xmt)) == NULL)
	        m = trp->tr_xmt;

	    /*
	     *	Now either release the transaction or start the timer
	     */
	    if (!trp->tr_retry && !trp->tr_bitmap && !trp->tr_xo) {
		m = (gbuf_t *)gbuf_copym(m);
		atp_x_done(trp);
	    } else {
		m = (gbuf_t *)gbuf_dupm(m);

		atp_timout(atp_req_timeout, trp, trp->tr_timeout);
	    }

	    if (m)
		DDP_OUTPUT(m);
	}
  }
}


/*
 *	atp_reply sends all the available messages in the bitmap again
 *		by queueing us to the write service routine
 */

void atp_reply(rcbp)
register struct atp_rcb *rcbp;
{
	register struct atp_state *atp;
	register int i;
	int s;

  if ((atp = rcbp->rc_queue) != 0) {
	ATDISABLE(s, atp->atp_lock);
	for (i = 0; i < rcbp->rc_pktcnt; i++) {
		if (rcbp->rc_bitmap&atp_mask[i])
			rcbp->rc_snd[i] = 1;
		else
			rcbp->rc_snd[i] = 0;
	}
        if (rcbp->rc_rep_waiting == 0) {
	        rcbp->rc_state = RCB_SENDING;
	        rcbp->rc_rep_waiting = 1;
	        ATENABLE(s, atp->atp_lock);
	        atp_send_replies(atp, rcbp);
	} else
	ATENABLE(s, atp->atp_lock);
  }
}


/*
 *	The rcb timer just frees the rcb, this happens when we missed a release for XO
 */

void atp_rcb_timer()
{  
	int s;
        register struct atp_rcb *rcbp;
	register struct atp_rcb *next_rcbp;
	extern   struct atp_rcb_qhead atp_need_rel;
	extern struct atp_trans *trp_tmo_rcb;

l_again:
	ATDISABLE(s, atpgen_lock);
	for (rcbp = atp_need_rel.head; rcbp; rcbp = next_rcbp) {
	        next_rcbp = rcbp->rc_tlist.next;

	        if (abs(time.tv_sec - rcbp->rc_timestamp) > 30) {
		        ATENABLE(s, atpgen_lock);
		        atp_rcb_free(rcbp);
		        goto l_again;
		}
	}
	ATENABLE(s, atpgen_lock);
	atp_timout(atp_rcb_timer, trp_tmo_rcb, 10 * HZ);
}

atp_iocack(atp, m)
struct   atp_state *atp;
register gbuf_t *m;
{
	if (gbuf_type(m) == MSG_IOCTL)
		gbuf_set_type(m, MSG_IOCACK);
	if (gbuf_cont(m))
		((ioc_t *)gbuf_rptr(m))->ioc_count = gbuf_msgsize(gbuf_cont(m));
	else
		((ioc_t *)gbuf_rptr(m))->ioc_count = 0;

	if (atp->dflag)
		asp_ack_reply(atp->atp_gref, m);
	else
		atalk_putnext(atp->atp_gref, m);
}

atp_iocnak(atp, m, err)
struct   atp_state *atp;
register gbuf_t *m;
register int err;
{
	if (gbuf_type(m) == MSG_IOCTL)
		gbuf_set_type(m, MSG_IOCNAK);
	((ioc_t *)gbuf_rptr(m))->ioc_count = 0;
	((ioc_t *)gbuf_rptr(m))->ioc_error = err ? err : ENXIO;
	((ioc_t *)gbuf_rptr(m))->ioc_rval = -1;
	if (gbuf_cont(m)) {
		gbuf_freem(gbuf_cont(m));
		gbuf_cont(m) = NULL;
	}

	if (atp->dflag)
		asp_nak_reply(atp->atp_gref, m);
	else
		atalk_putnext(atp->atp_gref, m);
}

/*
 *	Generate a transaction id for a socket
 */
static int lasttid;
atp_tid(atp)
register struct atp_state *atp;
{
	register int i;
	register struct atp_trans *trp;
	int s;

	ATDISABLE(s, atpgen_lock);
	for (i = lasttid;;) {
		i = (i+1)&0xffff;

		for (trp = atp->atp_trans_wait.head; trp; trp = trp->tr_list.next) {
		        if (trp->tr_tid == i)
			        break;
		}
		if (trp == NULL) {
			lasttid = i;
			ATENABLE(s, atpgen_lock);
			return(i);
		}
	}
}
