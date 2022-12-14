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
 * Mach Operating System
 * Copyright (c) 1989 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
 
/* 
 * Some inline code to speed up major block copies to and from the
 * screen buffer.
 * 
 * Copyright Ing. C. Olivetti & C. S.p.A. 1988, 1989.
 *  All rights reserved.
 *
 * orc!eugene	28 Oct 1988
 *
 */
/*
  Copyright 1988, 1989 by Olivetti Advanced Technology Center, Inc.,
Cupertino, California.

		All Rights Reserved

  Permission to use, copy, modify, and distribute this software and
its documentation for any purpose and without fee is hereby
granted, provided that the above copyright notice appears in all
copies and that both the copyright notice and this permission notice
appear in supporting documentation, and that the name of Olivetti
not be used in advertising or publicity pertaining to distribution
of the software without specific, written prior permission.

  OLIVETTI DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
IN NO EVENT SHALL OLIVETTI BE LIABLE FOR ANY SPECIAL, INDIRECT, OR
CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
LOSS OF USE, DATA OR PROFITS, WHETHER IN ACTION OF CONTRACT,
NEGLIGENCE, OR OTHER TORTIOUS ACTION, ARISING OUR OF OR IN CONNECTION
WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

 
#include <machdep/i386/asm.h>

/*
 * Function:	kd_slmwd()
 *
 *	This function "slams" a word (char/attr) into the screen memory using
 *	a block fill operation on the 386.
 *
 */

#define start 0x08(%ebp)
#define count 0x0c(%ebp)
#define value 0x10(%ebp)

ENTRY(kd_slmwd)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi

	movl	start, %edi
	movl	count, %ecx
	movw	value, %ax
	rep
	stosw

	popl	%edi
	leave
	ret
#undef start
#undef count
#undef value

/*
 * "slam up"
 */

#define from  0x08(%ebp)
#define to    0x0c(%ebp)
#define count 0x10(%ebp)
ENTRY(kd_slmscu)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%edi

	movl	from, %esi
	movl	to, %edi
	movl	count, %ecx
	cmpl	%edi, %esi
	rep
	movsw

	popl	%edi
	popl	%esi
	leave
	ret

/*
 * "slam down"
 */
ENTRY(kd_slmscd)
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	pushl	%edi

	movl	from, %esi
	movl	to, %edi
	movl	count, %ecx
	cmpl	%edi, %esi
	std
	rep
	movsw
	cld

	popl	%edi
	popl	%esi
	leave
	ret
#undef from
#undef to
#undef count
