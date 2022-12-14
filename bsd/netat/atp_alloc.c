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

/*    Modified for MP, 1996 by Tuyen Nguyen */
/*
 *	tcb (transaction) allocation routine. If no transaction data structure
 *		is available then put the module on a queue of modules waiting
 *		for transaction structures. When a tcb is available it will be
 *		removed from this list and its write queue will be scheduled.
 */
#include <appletalk.h>
#include <atp.h>
#include <ddp.h>
#include <at_atp.h>
#include <atp_inc.h>

#ifdef NEXT
/*### Rhapsody MCLBYTE is 2048, not 4096 like AIX */
	#define TRPS_PER_BLK 16
#else
	#define TRPS_PER_BLK 32
#endif
gbuf_t *atp_resource_m = 0;
extern atlock_t atpgen_lock;

struct atp_trans *atp_trans_alloc(atp)
struct  atp_state *atp;
{
	int s;
	int i;
	gbuf_t *m;
	register struct atp_trans *trp, *trp_array;

	ATDISABLE(s, atpgen_lock);
	if (atp_trans_free_list == 0) {
		ATENABLE(s, atpgen_lock);
		if ((m = gbuf_alloc(TRPS_PER_BLK*sizeof(struct atp_trans),PRI_HI)) == 0)
			return (struct atp_trans *)0;
		bzero(gbuf_rptr(m), TRPS_PER_BLK*sizeof(struct atp_trans));
		trp_array = (struct atp_trans *)gbuf_rptr(m);
		for (i=0; i < TRPS_PER_BLK-1; i++)
			trp_array[i].tr_list.next = (struct atp_trans *)&trp_array[i+1];
		ATDISABLE(s, atpgen_lock);
		gbuf_cont(m) = atp_resource_m;
		atp_resource_m = m;
		trp_array[i].tr_list.next = atp_trans_free_list;
		atp_trans_free_list = (struct atp_trans *)&trp_array[0];
	}

	trp = atp_trans_free_list;
	atp_trans_free_list = trp->tr_list.next;
	ATENABLE(s, atpgen_lock);
	trp->tr_queue = atp;
	trp->tr_state = TRANS_TIMEOUT;
	trp->tr_local_node = 0;
	ATLOCKINIT(trp->tr_lock);
	ATEVENTINIT(trp->tr_event);

	dPrintf(D_M_ATP_LOW, D_L_TRACE,
		("atp_trans_alloc: alloc'd trp 0x%x\n", (u_int) trp));
	return trp;
}

/*
 *	tcb free routine - if modules are waiting schedule them
 *      always called at 'lock'
 */

void atp_trans_free(trp)
register struct atp_trans *trp;
{
	int s;

	ATDISABLE(s, atpgen_lock);
	trp->tr_queue = 0;
	trp->tr_list.next = atp_trans_free_list;
	atp_trans_free_list = trp;
	ATENABLE(s, atpgen_lock);
}

/*
 *	This routine allocates a rcb, if none are available it makes sure the
 *		the write service routine will be called when one is
 *      always called at 'lock'
 */

struct atp_rcb *atp_rcb_alloc(atp)
struct  atp_state *atp;
{
	register struct atp_rcb *rcbp;
	int s;

	ATDISABLE(s, atpgen_lock);
	if ((rcbp = atp_rcb_free_list) != NULL) {
		atp_rcb_free_list = rcbp->rc_list.next;
		rcbp->rc_queue = atp;
		rcbp->rc_pktcnt = 0;
		rcbp->rc_local_node = 0;
	}
	ATENABLE(s, atpgen_lock);
	dPrintf(D_M_ATP_LOW, D_L_TRACE,
		("atp_rcb_alloc: allocated rcbp 0x%x\n", (u_int) rcbp));
	return(rcbp);
}

/*
 *	Here we free rcbs, if required reschedule other people waiting for them
 *      always called at 'lock'
 */

void atp_rcb_free(rcbp)
register struct atp_rcb *rcbp;
{
	register struct atp_state *atp;
	register int i;
	register int rc_state;
	int s;

	dPrintf(D_M_ATP_LOW, D_L_TRACE,
		("atp_rcb_free: freeing rcbp 0x%x\n", (u_int) rcbp));
	ATDISABLE(s, atpgen_lock);
	atp = rcbp->rc_queue;
	if ((rc_state = rcbp->rc_state) == -1) {
		ATENABLE(s, atpgen_lock);
		dPrintf(D_M_ATP, D_L_WARNING,
			("atp_rcb_free(%d): tid=%d,loc=%d,rem=%d\n",
			0, rcbp->rc_tid,
			rcbp->rc_socket.socket, atp->atp_socket_no));
		return;
	}
	rcbp->rc_state = -1;
	rcbp->rc_xo = 0;
	rcbp->rc_queue = 0;

	if (rcbp->rc_timestamp) {
	        extern struct atp_rcb_qhead atp_need_rel;

		rcbp->rc_timestamp = 0;
		ATP_Q_REMOVE(atp_need_rel, rcbp, rc_tlist);
		rcbp->rc_tlist.prev = NULL;
		rcbp->rc_tlist.next = NULL;
	}
	if (rcbp->rc_xmt) {
		gbuf_freem(rcbp->rc_xmt);
		rcbp->rc_xmt = NULL;
		for (i=0; i < rcbp->rc_pktcnt; i++)
			rcbp->rc_snd[i] = 0;
	}
	if (rc_state != RCB_UNQUEUED) {
		if (rc_state == RCB_PENDING) {
			ATP_Q_REMOVE(atp->atp_attached, rcbp, rc_list);
		} else {
			ATP_Q_REMOVE(atp->atp_rcb, rcbp, rc_list);
		}
	}
        if (rcbp->rc_ioctl) {
       		gbuf_freem(rcbp->rc_ioctl);
		rcbp->rc_ioctl = NULL;
	}
	rcbp->rc_list.next = atp_rcb_free_list;
	atp_rcb_free_list = rcbp;
	ATENABLE(s, atpgen_lock);
}
