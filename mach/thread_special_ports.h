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
 * Copyright (c) 1991,1990,1989,1988,1987 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon
 * the rights to redistribute these changes.
 */
/*
 *	File:	mach/thread_special_ports.h
 *
 *	Defines codes for special_purpose thread ports.  These are NOT
 *	port identifiers - they are only used for the thread_get_special_port
 *	and thread_set_special_port routines.
 *	
 */

#ifndef	_MACH_THREAD_SPECIAL_PORTS_H_
#define _MACH_THREAD_SPECIAL_PORTS_H_

#define THREAD_KERNEL_PORT	1	/* Represents the thread to the outside
					   world.*/
#define THREAD_EXCEPTION_PORT	3	/* Exception messages for the thread
					   are sent to this port. */

/*
 *	Definitions for ease of use
 */

#define thread_get_kernel_port(thread, port)	\
		(thread_get_special_port((thread), THREAD_KERNEL_PORT, (port)))

#define thread_set_kernel_port(thread, port)	\
		(thread_set_special_port((thread), THREAD_KERNEL_PORT, (port)))

#define thread_get_exception_port(thread, port)	\
		(thread_get_special_port((thread), THREAD_EXCEPTION_PORT, (port)))

#define thread_set_exception_port(thread, port)	\
		(thread_set_special_port((thread), THREAD_EXCEPTION_PORT, (port)))


/* Definitions for the old IPC interface. */

#define THREAD_REPLY_PORT	2	/* Default reply port for the thread's
					   use. */

#define thread_get_reply_port(thread, port)	\
		(thread_get_special_port((thread), THREAD_REPLY_PORT, (port)))

#define thread_set_reply_port(thread, port)	\
		(thread_set_special_port((thread), THREAD_REPLY_PORT, (port)))

#endif	/* _MACH_THREAD_SPECIAL_PORTS_H_ */
