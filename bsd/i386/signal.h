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
 * Copyright (c) 1992 NeXT Computer, Inc.
 *
 * HISTORY
 *
 * 8 April 1992 ? at NeXT
 *	Created.
 */

#ifndef	_i386_SIGNAL_
#define	_i386_SIGNAL_ 1

typedef int sig_atomic_t; 

/*
 * Information pushed on stack when a signal is delivered.
 * This is used by the kernel to restore state following
 * execution of the signal handler.  It is also made available
 * to the handler to allow it to properly restore state if
 * a non-standard exit is performed.
 */
struct	sigcontext {
    int			sc_onstack;	/* sigstack state to restore */
    int			sc_mask;	/* signal mask to restore */
    unsigned int	sc_eax;
    unsigned int	sc_ebx;
    unsigned int	sc_ecx;
    unsigned int	sc_edx;
    unsigned int	sc_edi;
    unsigned int	sc_esi;
    unsigned int	sc_ebp;
    unsigned int	sc_esp;
    unsigned int	sc_ss;
    unsigned int	sc_eflags;
    unsigned int	sc_eip;
    unsigned int	sc_cs;
    unsigned int	sc_ds;
    unsigned int	sc_es;
    unsigned int	sc_fs;
    unsigned int	sc_gs;
};

#endif	/* _i386_SIGNAL_ */
