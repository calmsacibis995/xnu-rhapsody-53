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

#include <mach_counters.h>

#include <kern/counters.h>

/*
 *	We explicitly initialize the counters to make
 *	them contiguous in the kernel's data space.
 *	This makes them easier to examine with ddb.
 */

mach_counter_t c_thread_invoke_hits = 0;
mach_counter_t c_thread_invoke_misses = 0;
mach_counter_t c_thread_invoke_csw = 0;
mach_counter_t c_thread_handoff_hits = 0;
mach_counter_t c_thread_handoff_misses = 0;

#if	MACH_COUNTERS
mach_counter_t c_threads_current = 0;
mach_counter_t c_threads_max = 0;
mach_counter_t c_threads_min = 0;
mach_counter_t c_threads_total = 0;
mach_counter_t c_stacks_current = 0;
mach_counter_t c_stacks_max = 0;
mach_counter_t c_stacks_min = 0;
mach_counter_t c_stacks_total = 0;
mach_counter_t c_clock_ticks = 0;
mach_counter_t c_ipc_mqueue_send_block = 0;
mach_counter_t c_ipc_mqueue_receive_block_user = 0;
mach_counter_t c_ipc_mqueue_receive_block_kernel = 0;
mach_counter_t c_mach_msg_trap_block_fast = 0;
mach_counter_t c_mach_msg_trap_block_slow = 0;
mach_counter_t c_mach_msg_trap_block_exc = 0;
mach_counter_t c_exception_raise_block = 0;
mach_counter_t c_swtch_block = 0;
mach_counter_t c_swtch_pri_block = 0;
mach_counter_t c_thread_switch_block = 0;
mach_counter_t c_thread_switch_handoff = 0;
mach_counter_t c_ast_taken_block = 0;
mach_counter_t c_thread_halt_self_block = 0;
mach_counter_t c_vm_fault_page_block_busy_user = 0;
mach_counter_t c_vm_fault_page_block_busy_kernel = 0;
mach_counter_t c_vm_fault_page_block_backoff_user = 0;
mach_counter_t c_vm_fault_page_block_backoff_kernel = 0;
mach_counter_t c_vm_page_wait_block_user = 0;
mach_counter_t c_vm_page_wait_block_kernel = 0;
mach_counter_t c_vm_pageout_block = 0;
mach_counter_t c_vm_pageout_scan_block = 0;
mach_counter_t c_idle_thread_block = 0;
mach_counter_t c_idle_thread_handoff = 0;
mach_counter_t c_sched_thread_block = 0;
mach_counter_t c_io_done_thread_block = 0;
mach_counter_t c_net_thread_block = 0;
mach_counter_t c_reaper_thread_block = 0;
mach_counter_t c_swapin_thread_block = 0;
mach_counter_t c_action_thread_block = 0;
#endif	MACH_COUNTERS
