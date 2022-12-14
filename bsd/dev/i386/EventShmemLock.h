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

/* 	Copyright (c) 1992 NeXT Computer, Inc.  All rights reserved. 
 *
 * EventShmemLock.h -	Shared memory area locks for use between the
 *			WindowServer and the Event Driver.
 *
 *
 * HISTORY
 * 29 April 1992    Mike Paquette at NeXT
 *      Created. 
 *
 * Multiprocessor locks used within the shared memory area between the
 * kernel and event system.  These must work in both user and kernel mode.
 * The locks are defined in an include file so they get exported to the local
 * include file area.
 *
 * This is basically a ripoff of the spin locks under the cthreads packages.
 */
#ifdef DRIVER_PRIVATE

#include <architecture/i386/asm_help.h>
/* 
 * void
 * ev_lock(p)
 *	int *p;
 *
 * Lock the lock pointed to by p.  Spin (possibly forever) until the next
 * lock is available.
 */
	TEXT

LEAF(_ev_lock, 0)
	push 	%eax
	push	%ecx
	movl	$1, %ecx
	movl	12(%esp), %eax	
_spin:
	xchgl	%ecx,0(%eax)
	cmp	$0, %ecx
	jne	_spin
	pop	%ecx
	pop	%eax
END(_ev_lock)
	

/*
 * void
 * ev_unlock(p)
 *	int *p;
 *
 * Unlock the lock pointed to by p.
 */
LEAF(_ev_unlock, 0)
	push	%eax
	movl	8(%esp),%eax
	movl	$0,0(%eax)
	pop	%eax
END(_ev_unlock)



/*
 * int
 * ev_try_lock(p)
 *	int *p;
 *
 * Try to lock p.  Return zero if not successful.
 */

LEAF(_ev_try_lock, 0)
	movl	4(%esp), %eax
   lock;bts	$0, 0(%eax)
	jb	1f
	movl	$1, %eax		/* yes */
	ret
1:
	xorl	%eax, %eax		/* no */
END(_ev_try_lock)

#endif /* DRIVER_PRIVATE */

