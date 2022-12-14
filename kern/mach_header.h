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
 *	File: kern/mach_header.h
 *
 *	Definitions for accessing mach-o headers.
 *
 * HISTORY
 * 29-Jan-92  Mike DeMoney (mike@next.com)
 *	Made into machine independent form from machdep/m68k/mach_header.h.
 *	Ifdef'ed out most of this since I couldn't find any references.
 */

#ifndef	_KERN_MACH_HEADER_
#define	_KERN_MACH_HEADER_

#import <mach/mach_types.h>
#import <mach-o/loader.h>

#if	KERNEL
struct mach_header **getmachheaders(void);
vm_offset_t getlastaddr(void);

struct segment_command *firstseg(void);
struct segment_command *firstsegfromheader(struct mach_header *header);
struct segment_command *nextseg(struct segment_command *sgp);
struct segment_command *nextsegfromheader(
	struct mach_header	*header,
	struct segment_command	*seg);
struct segment_command *getsegbyname(char *seg_name);
struct segment_command *getsegbynamefromheader(
	struct mach_header	*header,
	char			*seg_name);
void *getsegdatafromheader(struct mach_header *, char *, int *);
struct section *getsectbyname(char *seg_name, char *sect_name);
struct section *getsectbynamefromheader(
	struct mach_header	*header,
	char			*seg_name,
	char			*sect_name);
void *getsectdatafromheader(struct mach_header *, char *, char *, int *);
struct section *firstsect(struct segment_command *sgp);
struct section *nextsect(struct segment_command *sgp, struct section *sp);
struct fvmlib_command *fvmlib(void);
struct fvmlib_command *fvmlibfromheader(struct mach_header *header);
struct segment_command *getfakefvmseg(void);

#endif	/* KERNEL */

#endif	/* _KERN_MACH_HEADER_ */
