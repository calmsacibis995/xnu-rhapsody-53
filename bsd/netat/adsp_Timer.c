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
 *	Copyright (c) 1990, 1996-1998 Apple Computer, Inc.
 *	All Rights Reserved.
 *
 *	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF APPLE COMPUTER, INC.
 *	The copyright notice above does not evidence any actual or
 *	intended publication of such source code.
 */


#include <adsp_local.h>

/*
 * TrashSession
 * 
 * Cleanly abort a session that might be open.  Called if probe timer expires,
 * or from AppleTalk event handler (close or network gone away)
 *
 * Only call if the session is active (I.e. not for closed or listeners)
 *
 * INPUTS:
 * 		session pointer
 * OUTPUTS:
 * 		none
 */
void TrashSession(sp)		/* (CCBPtr sp) */
    CCBPtr sp;
{
    int s;

    ATDISABLE(s, sp->lock);
    sp->userFlags |= eTearDown;
    sp->removing = 1;
    sp->state = sClosed;
    ATENABLE(s, sp->lock);

    DoClose(sp, errAborted, 1);
}


/*
 * DoTimerElem
 * 
 * INPUTS:
 * 	
 * OUTPUTS:
 * 	
 */
void DoTimerElem(t) /* (TimerElemPtr t) */
    TimerElemPtr t;
{
    CCBPtr sp;
    int	s;

    sp = (CCBPtr)((Ptr)t - t->type); /* Recover stream pointer for this guy */
    ATDISABLE(s, sp->lock);
	
    if (t->type == kFlushTimerType) { /* flush write data time just fired */
	if (sp->sData) {	/* If there's any data, flush it. */
	    sp->writeFlush = 1;
	    goto send;
	}
    } else if (t->type == kRetryTimerType) {
	if (sp->waitingAck) {
		
	    sp->waitingAck = 0;
	    sp->sendSeq	= sp->firstRtmtSeq;
	    sp->pktSendCnt = 0;
	    sp->resentData = 1;	/* Had to resend data */
	    sp->noXmitFlow = 1;	/* Don't incr. max packets. */

	    if ((sp->pktSendMax /= 2) == 0) /* Back off on max # packets 
					     * sent */
		sp->pktSendMax = 1;

	    if ((sp->roundTrip *= 2) > sp->probeInterval)
		sp->roundTrip = sp->probeInterval;
	    sp->rtmtInterval = sp->roundTrip + ((short)2 * 
						(short)sp->deviation);
	    goto send;
	}
    } else if (t->type == kAttnTimerType) {
	if (sp->sapb) {		/* Unacknowledged attn pkt */
	    sp->sendAttnData = 1;
	    goto send;
	}
    } else if (t->type == kResetTimerType) {
	if (sp->frpb) {		/* Unacknowledged forward reset */
	    sp->sendCtl |= B_CTL_FRESET;
	    goto send;
	}
    } else if (t->type == kProbeTimerType) {
	if (sp->state == sOpen || sp->state == sClosing) {
	    if (--sp->probeCntr == 0) { /* Connection died */
		ATENABLE(s, sp->lock);
		TrashSession(sp);
		return;
	    } else {
		InsertTimerElem(&adspGlobal.slowTimers, &sp->ProbeTimer, 
				sp->probeInterval);
		sp->sendCtl |= B_CTL_PROBE;
		goto send;
	    }
	} else if (sp->state == sOpening) {
	    if ((sp->openState == O_STATE_OPENWAIT) ||
		(sp->openState == O_STATE_ESTABLISHED))
	    {
		if (--sp->openRetrys == 0) { /* Oops, didn't open */
		    sp->state = sClosed;
		    ATENABLE(s, sp->lock);
		    DoClose(sp, errOpening, 1);
		    return;
		}		/* open failed */
		else		/* Send packet again */
		{
		    sp->sendCtl |= (sp->openState == O_STATE_OPENWAIT) ?
			B_CTL_OREQ : B_CTL_OREQACK;
		    goto send;
		}
	    }			/* we're opening */
	}
    }
			
    else {
	dPrintf(D_M_ADSP, D_L_ERROR, ("DoTimerElem:Unknown timer type!\n"));
    }

    ATENABLE(s, sp->lock);
	return;
	
send:
    ATENABLE(s, sp->lock);
    CheckSend(sp);
}


static void *Timer_tmo = 0;
static StopTimer;

/*
 * TimerTick
 * 
 * Called 6 times per second
 * INPUTS:
 * 	
 * OUTPUTS:
 * 	
 */
void TimerTick()		/* (void) */
{
    if (StopTimer)
	return;
    TimerQueueTick(&adspGlobal.slowTimers);
    TimerQueueTick(&adspGlobal.fastTimers);
    Timer_tmo = (void *)atalk_timeout(TimerTick, (caddr_t)0, HZ/6);
}

void TimerStop()
{
	StopTimer = 1;
	atalk_untimeout(TimerTick, (caddr_t) 0, Timer_tmo);
}
