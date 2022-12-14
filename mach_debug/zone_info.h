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
 * Copyright (c) 1991,1990,1989 Carnegie Mellon University
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

#ifndef	_MACH_DEBUG_ZONE_INFO_H_
#define _MACH_DEBUG_ZONE_INFO_H_

#include <mach/boolean.h>
#include <mach/machine/vm_types.h>

/*
 *	Remember to update the mig type definitions
 *	in mach_debug_types.defs when adding/removing fields.
 */

#define ZONE_NAME_MAX_LEN		80

typedef struct zone_name {
	char		zn_name[ZONE_NAME_MAX_LEN];
} zone_name_t;

typedef zone_name_t *zone_name_array_t;


typedef struct zone_info {
	integer_t	zi_count;	/* Number of elements used now */
	vm_size_t	zi_cur_size;	/* current memory utilization */
	vm_size_t	zi_max_size;	/* how large can this zone grow */
	vm_size_t	zi_elem_size;	/* size of an element */
	vm_size_t	zi_alloc_size;	/* size used for more memory */
/*boolean_t*/integer_t	zi_pageable;	/* zone pageable? */
/*boolean_t*/integer_t	zi_sleepable;	/* sleep if empty? */
/*boolean_t*/integer_t	zi_exhaustible;	/* merely return if empty? */
/*boolean_t*/integer_t	zi_collectable;	/* garbage collect elements? */
} zone_info_t;

typedef zone_info_t *zone_info_array_t;

typedef struct zone_free_space_info {
	vm_size_t	zf_alloc_unit;	/* unit size of alloc */
	vm_size_t	zf_alloc_max;	/* max size of alloc */
	integer_t	zf_num_chunks;	/* number of chunks */
} zone_free_space_info_t;

typedef zone_free_space_info_t *zone_free_space_info_array_t;

typedef struct zone_free_space_chunk {
	vm_offset_t	zf_address;	/* address of chunk */
	vm_size_t	zf_length;	/* length of chunk */
} zone_free_space_chunk_t;

typedef zone_free_space_chunk_t *zone_free_space_chunk_array_t;

#endif	/* _MACH_DEBUG_ZONE_INFO_H_ */
