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


#include	<adsp_local.h>

/*
 * calcSendFree
 *
 * INPUTS:
 *		sp		ADSP Stream
 * OUTPUTS:
 *		# of bytes avail in local send queue
 */
int CalcSendQFree(sp)		/* (CCBPtr sp) */
    CCBPtr sp;
{
    int bytes;
    
    bytes = calcSendQ(sp);
    bytes = sp->sbuflen - bytes;

    if (bytes < 0)
	return 0;
    return bytes;
}

calcSendQ(sp)
    CCBPtr sp;
{
    register gbuf_t *mp;
    int bytes = 0;

    if (sp->sData) {		/* There is data in buffer */
	if (mp = sp->sbuf_mb) {
	    do {
		bytes += gbuf_msgsize(mp);
		mp = gbuf_next(mp);
	    } while (mp);
	}
	if (mp = sp->csbuf_mb)
	    bytes += gbuf_msgsize(mp);
    }
    return bytes;
}

/*
 * dspStatus
 * 
 * INPUTS:
 * 	--> ccbRefNum		refnum of connection end
 *
 * OUTPUTS:
 *	<-- statusCCB		Pointer to the connection control block
 *	<-- sendQPending	bytes waiting to be sent or acknowledged
 *	<-- sendQFree		available buffer in bytes of send queue
 *	<-- recvQPending	bytes waiting to be read from queue
 *	<-- recvQFree		available buffer in bytes of receive queue
 *
 * ERRORS:
 *	errRefNum		bad connection refnum
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int adspStatus(sp, pb)	/* (DSPPBPtr pb) */
    CCBPtr sp;
    register struct adspcmd *pb;
{
    OSErr err;
    short bytes;
    int	s;

    if (sp == 0) {
	pb->ioResult = errRefNum;
	return EINVAL;
    }
	
    pb->u.statusParams.ccbPtr 	= (TPCCB)sp;
    ATDISABLE(s, sp->lock);	
	
    /*
     * pending bytes in send queue
     */		
    if (sp->sData) 
	bytes = calcSendQ(sp);
    else
	bytes = 0;
    pb->u.statusParams.sendQPending = bytes;
	
				/* available buffer space in send queue */
    pb->u.statusParams.sendQFree = CalcSendQFree(sp);
	
    /*
     * pending bytes in recv queue
     */		
    if (sp->rData)
	bytes = calcRecvQ(sp);
    else
	bytes = 0;
    pb->u.statusParams.recvQPending = bytes;
	
				/* available buffer space in receive queue */
    pb->u.statusParams.recvQFree = CalcRecvWdw(sp);

    ATENABLE(s, sp->lock);	
    pb->ioResult = 0;
    adspioc_ack(0, pb->ioc, pb->gref);
    return 0;

}
