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
 * HISTORY
 * Revision 1.1.1.1  1997/09/30 02:45:35  wsanchez
 * Import of kernel from umeshv/kernel
 *
 * 
 * Revision ?.?  97/07/17  14:39:51  umeshv
 *	Fixed multipe definition of kernel_pmap.
 * 
 * Revision 2.6  91/05/18  14:39:51  rpd
 * 	Removed pmap_update.
 * 	[91/04/12            rpd]
 * 	Removed pmap_bootstrap, pmap_map, pmap_valid_page.
 * 	Added pmap_startup, pmap_steal_memory, pmap_free_pages,
 * 	pmap_virtual_space, pmap_next_page.
 * 	[91/03/22            rpd]
 * 
 * Revision 2.5  91/05/14  17:48:08  mrt
 * 	Correcting copyright
 * 
 * Revision 2.4  91/02/05  17:57:34  mrt
 * 	Changed to new Mach copyright
 * 	[91/02/01  16:31:04  mrt]
 * 
 * Revision 2.3  90/01/22  23:09:05  af
 * 	Added (optional) pmap_attributes.  This should be implemented
 * 	just as a macro that evaluates to KERN_INVALID_ARGUMENT for
 * 	those architectures that do not support any special memory
 * 	feature in hardware.  NUMA machines and cache-incoherent
 * 	machines (e.g. mips) should instead implement it as appropriate.
 * 	[89/12/08            af]
 * 
 * Revision 2.2  89/11/29  14:16:53  af
 * 	Carried over the following mods from impure kernel.
 * 		Added pmap_phys_to_frame as inverse of pmap_phys_address, for use by
 * 		memory map function in machine-independent pseudo device drivers.
 * 		The correct thing to do, of course, is to change the device driver map
 * 		function spec to return a physical address instead of a frame, but
 * 		this would require changing too many device drivers...
 * 		[89/09/05  16:45:48  jsb]
 * 
 * 		Removed unused and ugly pmap_redzone() from the list of required
 * 		pmap functions.  Made more routines implementable as macros.
 * 		[89/08/05            af]
 * 	Separate user and kernel cases of PMAP_ACTIVATE and
 * 	PMAP_DEACTIVATE; supply default definitions.
 * 	[89/04/28            dbg]
 * 
 * Revision 2.1  89/08/03  16:44:35  rwd
 * Created.
 * 
 * Revision 2.8  89/04/18  21:24:43  mwyoung
 * 	Recent history [mwyoung]:
 * 	 	Remove pmap_remove_all(), pmap_copy_on_write(), as they're
 * 	 	 no longer required routines.  The machine-independent code
 * 	 	 uses pmap_page_protect() instead.
 * 		Documented interface.  The precise interface specification
 * 		 should be reconstructed from the pmap implementations.
 * 	History condensation:
 * 		Added calls for page locking [mwyoung].
 * 		Added reference bit handling [avie].
 * 		Created [avie].
 * 	[89/04/18            mwyoung]
 * 
 */
/*
 *	File:	vm/pmap.h
 *	Author:	Avadis Tevanian, Jr.
 *	Date:	1985
 *
 *	Machine address mapping definitions -- machine-independent
 *	section.  [For machine-dependent section, see "machine/pmap.h".]
 */

#ifndef	_VM_PMAP_H_
#define _VM_PMAP_H_

#import <mach/features.h>

#include <machdep/machine/pmap.h>
#include <mach/machine/vm_types.h>
#include <mach/boolean.h>

#if	OLD_VM_CODE
void		pmap_bootstrap();
void		pmap_init();
vm_offset_t	pmap_map();
pmap_t		pmap_create();
#ifdef pmap_kernel
extern
#endif
pmap_t		pmap_kernel();
void		pmap_destroy();
void		pmap_reference();
void		pmap_remove();
void		pmap_remove_all();
void		pmap_copy_on_write();
void		pmap_protect();
void		pmap_enter();
vm_offset_t	pmap_extract();
void		pmap_update();
void		pmap_collect();
void		pmap_activate();
void		pmap_deactivate();
#if !defined(ppc)
void		pmap_copy();
#endif
void		pmap_statistics();
void		pmap_clear_reference();
boolean_t	pmap_is_referenced();
void		pmap_change_wiring();
void		pmap_clear_modify();
void		pmap_page_protect();

void		pmap_redzone();
boolean_t	pmap_access();

typedef struct mem_region {
	struct vm_page	*vm_page_array;	/* First resident page in table */
	unsigned	first_page;	/* first physical page number */
					/* ... represented in vm_page_array */
	unsigned	last_page;	/* last physical page number + 1 */
					/* ... represented in vm_page_array */
					/* [INCLUSIVE] */
	unsigned	num_pages;	/* last_page - first_page */
	vm_offset_t	base_phys_addr;	/* fixed base of region for dump() */
	vm_offset_t	first_phys_addr;/* physical address for first_page */
	vm_offset_t	last_phys_addr;	/* physical address for last_page */
} *mem_region_t;

/*
 * Machine dependent code must initialize the mem_region table before
 * calling any machine independent code.
 *
 * Machine dependent code should initialize the following fields of
 * mem_region:
 *	base_phys_addr --	first physical address of the contiguous
 *				memory region
 *	first_phys_addr --	same as base_phys_addr
 *	last_phys_addr --	last_phys_addr + 1 of the contiguous
 *				memory region
 *
 * Currently, machine independent code assumes (bogusly) that it can
 * allocate 1-1 mapped kernel data from mem_regions.
 * FIXME: Undo this.  It's not portable.
 * 
 * After all 1-1 allocations are complete, MI code (vm_resident) then
 * goes and allocates vm_page_t's for the remaining memory in the
 * mem_region table, and sets or modifies the following values in the
 * mem_region table:
 *
 *	vm_page_array --	points to the allocated vm_page_t's
 *				for the physical page frame of this region
 *	first_page --		phys page num of first page in vm_page_array
 *	last_page --		phys page num + 1 of last page in vm_page_array
 *	num_pages --		number of page frames in this mem region
 *	first_phys_addr --	address corresponding to first_page
 *	last_phys_ddr --	address corresponding to last_page
 */
extern		struct mem_region mem_region[];
extern int	num_regions;

/* convert a pa to an ordinal of the physical page frames */
extern unsigned vm_ppi(vm_offset_t pa);
/* returns true if p refers to a page frame */
extern boolean_t vm_valid_page(vm_offset_t p);
/* translates physical address to vm_page pointer */
extern struct vm_page *vm_phys_to_vm_page(vm_offset_t pa);
/* allocates memory from the mem regions */
extern vm_offset_t vm_alloc_from_region(vm_size_t size, vm_size_t align);
#else
/*
 *	The following is a description of the interface to the
 *	machine-dependent "physical map" data structure.  The module
 *	must provide a "pmap_t" data type that represents the
 *	set of valid virtual-to-physical addresses for one user
 *	address space.  [The kernel address space is represented
 *	by a distinguished "pmap_t".]  The routines described manage
 *	this type, install and update virtual-to-physical mappings,
 *	and perform operations on physical addresses common to
 *	many address spaces.
 */

/*
 *	Routines used for initialization.
 *	There is traditionally also a pmap_bootstrap,
 *	used very early by machine-dependent code,
 *	but it is not part of the interface.
 */

extern vm_offset_t	pmap_steal_memory();	/* During VM initialization,
						 * steal a chunk of memory.
						 */
extern unsigned int	pmap_free_pages();	/* During VM initialization,
						 * report remaining unused
						 * physical pages.
						 */
extern void		pmap_startup();		/* During VM initialization,
						 * use remaining physical pages
						 * to allocate page frames.
						 */
extern void		pmap_init();		/* Initialization,
						 * after kernel runs
						 * in virtual memory.
						 */

#ifndef	MACHINE_PAGES
/*
 *	If machine/pmap.h defines MACHINE_PAGES, it must implement
 *	the above functions.  The pmap module has complete control.
 *	Otherwise, it must implement
 *		pmap_free_pages
 *		pmap_virtual_space
 *		pmap_next_page
 *		pmap_init
 *	and vm/vm_resident.c implements pmap_steal_memory and pmap_startup
 *	using pmap_free_pages, pmap_next_page, pmap_virtual_space,
 *	and pmap_enter.  pmap_free_pages may over-estimate the number
 *	of unused physical pages, and pmap_next_page may return FALSE
 *	to indicate that there are no more unused pages to return.
 *	However, for best performance pmap_free_pages should be accurate.
 */

extern boolean_t	pmap_next_page();	/* During VM initialization,
						 * return the next unused
						 * physical page.
						 */
extern void		pmap_virtual_space();	/* During VM initialization,
						 * report virtual space
						 * available for the kernel.
						 */
#endif	/* MACHINE_PAGES */

/*
 *	Routines to manage the physical map data structure.
 */
extern pmap_t		pmap_create();		/* Create a pmap_t. */
#ifndef pmap_kernel
extern pmap_t		pmap_kernel();		/* Return the kernel's pmap_t. */
#endif /* pmap_kernel */
extern void		pmap_reference();	/* Gain a reference. */
extern void		pmap_destroy();		/* Release a reference. */

extern void		pmap_enter();		/* Enter a mapping */

/*
 *	Routines that operate on ranges of virtual addresses.
 */
extern void		pmap_remove();		/* Remove mappings. */
extern void		pmap_protect();		/* Change protections. */

/*
 *	Routines to set up hardware state for physical maps to be used.
 */
extern void		pmap_activate();	/* Prepare pmap_t to run
						 * on a given processor.
						 */
extern void		pmap_deactivate();	/* Release pmap_t from
						 * use on processor.
						 */


/*
 *	Routines that operate on physical addresses.
 */
extern void		pmap_page_protect();	/* Restrict access to page. */

/*
 *	Routines to manage reference/modify bits based on
 *	physical addresses, simulating them if not provided
 *	by the hardware.
 */
extern void		pmap_clear_reference();	/* Clear reference bit */
#ifndef pmap_is_referenced
extern boolean_t	pmap_is_referenced();	/* Return reference bit */
#endif /* pmap_is_referenced */
extern void		pmap_clear_modify();	/* Clear modify bit */
extern boolean_t	pmap_is_modified();	/* Return modify bit */


/*
 *	Statistics routines
 */
extern void		pmap_statistics();	/* Return statistics */

#ifndef	pmap_resident_count
extern int		pmap_resident_count();
#endif	/* pmap_resident_count */

/*
 *	Sundry required routines
 */
extern vm_offset_t	pmap_extract();		/* Return a virtual-to-physical
						 * mapping, if possible.
						 */

extern boolean_t	pmap_access();		/* Is virtual address valid? */

extern void		pmap_collect();		/* Perform garbage
						 * collection, if any
						 */

extern void		pmap_change_wiring();	/* Specify pageability */

#ifndef	pmap_phys_address
extern vm_offset_t	pmap_phys_address();	/* Transform address
						 * returned by device
						 * driver mapping function
						 * to physical address
						 * known to this module.
						 */
#endif	/* pmap_phys_address */
#ifndef	pmap_phys_to_frame
extern int		pmap_phys_to_frame();	/* Inverse of
						 * pmap_phys_address,
						 * for use by device driver
						 * mapping function in
						 * machine-independent
						 * pseudo-devices.
						 */
#endif	/* pmap_phys_to_frame */

/*
 *	Optional routines
 */
#ifndef	pmap_copy
extern void		pmap_copy();		/* Copy range of
						 * mappings, if desired.
						 */
#endif	/* pmap_copy */
#ifndef pmap_attribute
extern kern_return_t	pmap_attribute();	/* Get/Set special
						 * memory attributes
						 */
#endif	/* pmap_attribute */

/*
 * Routines defined as macros.
 */
#ifndef	PMAP_ACTIVATE_USER
#define	PMAP_ACTIVATE_USER(pmap, thread, cpu) {		\
	if ((pmap) != kernel_pmap)			\
	    PMAP_ACTIVATE(pmap, thread, cpu);		\
}
#endif	/* PMAP_ACTIVATE_USER */

#ifndef	PMAP_DEACTIVATE_USER
#define	PMAP_DEACTIVATE_USER(pmap, thread, cpu) {	\
	if ((pmap) != kernel_pmap)			\
	    PMAP_DEACTIVATE(pmap, thread, cpu);		\
}
#endif	/* PMAP_DEACTIVATE_USER */



#ifndef	PMAP_ACTIVATE_KERNEL
#define	PMAP_ACTIVATE_KERNEL(cpu)			\
		PMAP_ACTIVATE(kernel_pmap, THREAD_NULL, cpu)
#endif	/* PMAP_ACTIVATE_KERNEL */

#ifndef	PMAP_DEACTIVATE_KERNEL
#define	PMAP_DEACTIVATE_KERNEL(cpu)			\
		PMAP_DEACTIVATE(kernel_pmap, THREAD_NULL, cpu)
#endif	/* PMAP_DEACTIVATE_KERNEL */
#endif

/*
 *	Exported data structures
 */

extern pmap_t	kernel_pmap;			/* The kernel's map */

#endif	/* _VM_PMAP_H_ */
