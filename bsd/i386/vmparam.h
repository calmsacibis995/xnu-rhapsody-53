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
 * HISTORY
 * 16-Jan-98  Wilfredo Sanchez (wsanchez@apple.com)
 *   Remove #ifdef KERNEL_PRiVATE around USRSTACK to match ppc version.
 *   Sendmail needs it.
 *
 * 05-Sep-97  Umesh Vaishampayan (umeshv@apple.com)
 *	Made MAXSSIZ a finite value.
 *
 * 05-Mar-89  Avadis Tevanian, Jr. (avie) at NeXT
 *	Make MAXDSIZ infinity.
 *
 * 12-Aug-87  John Seamons (jks) at NeXT
 *	Ported to NeXT.
 */ 

#ifndef	_BSD_I386_VMPARAM_H_
#define	_BSD_I386_VMPARAM_H_ 1

#import <sys/resource.h>

#define	USRSTACK	0xc0000000

/*
 * Virtual memory related constants, all in bytes
 */
#ifndef DFLDSIZ
#define	DFLDSIZ		(6*1024*1024)		/* initial data size limit */
#endif
#ifndef MAXDSIZ
#define	MAXDSIZ		(RLIM_INFINITY)		/* max data size */
#endif
#ifndef	DFLSSIZ
#define	DFLSSIZ		(512*1024)		/* initial stack size limit */
#endif
#ifndef	MAXSSIZ
#define	MAXSSIZ		(64*1024*1024)			/* max stack size */
#endif
#ifndef	DFLCSIZ
#define DFLCSIZ		(0)			/* initial core size limit */
#endif
#ifndef	MAXCSIZ
#define MAXCSIZ		(RLIM_INFINITY)		/* max core size */
#endif	/* MAXCSIZ */

#endif	/* _BSD_I386_VMPARAM_H_ */
