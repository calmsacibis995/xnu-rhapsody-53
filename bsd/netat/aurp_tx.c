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
 *	Copyright (c) 1996 Apple Computer, Inc. 
 *
 *	The information contained herein is subject to change without
 *	notice and  should not be  construed as a commitment by Apple
 *	Computer, Inc. Apple Computer, Inc. assumes no responsibility
 *	for any errors that may appear.
 *
 *	Confidential and Proprietary to Apple Computer, Inc.
 *
 *	File: tx.c
 */
#include <sysglue.h>
#include <at/appletalk.h>
#include <lap.h>
#include <routing_tables.h>
#define _AURP
#define _KERNSYS
#include <at/aurp.h>
#include <at_aurp.h>

/*
 * Any AURP protocol or appletalk data (ddp) packets flowing through
 *  are inserted into the kernel aurpd process's (atalk) input queue.
 * Assume here that we deal with single packets, i.e., someone earlier
 *  in the food chain has broken up packet chains.
 */
void AURPsend(mdata, type, node)
	gbuf_t *mdata;
	int type, node;
{
	struct domain *domain;
	aurp_hdr_t *hdr;
	gbuf_t *m;
	int msize = AT_WR_OFFSET+32+IP_DOMAINSIZE;

	/* Add the domain header */
	if ((m = gbuf_alloc(msize, PRI_MED)) == 0) {
		gbuf_freem(mdata);
		return;
	}
	gbuf_wset(m,msize);
	gbuf_rinc(m,AT_WR_OFFSET+32);
	gbuf_cont(m) = mdata;
	domain = (struct domain *)gbuf_rptr(m);
	domain->dst_length = IP_LENGTH;
	domain->dst_authority = IP_AUTHORITY;
	domain->dst_distinguisher = IP_DISTINGUISHER;
	domain->src_length = IP_LENGTH;
	domain->src_authority = IP_AUTHORITY;
	domain->src_distinguisher = IP_DISTINGUISHER;
	domain->src_address = aurp_global.src_addr;
	domain->version = AUD_Version;
	domain->reserved = 0;
	domain->type = type;
	domain->dst_address = aurp_global.dst_addr[node];
	atalk_to_ip(m);
}

/*
 * Called from within ddp (via ddp_AURPsendx) to handle data (DDP) packets
 *  sent from the AppleTalk stack, routing updates, and routing info
 *  initialization.
 */
void AURPcmdx(code, mdata, param)
	int code;
	gbuf_t *mdata;
	int param;
{
	unsigned char node;
	gbuf_t *mdata_next;
	aurp_rtinfo_t *rtinfo;

	if (mdata == 0)
		return;
	if (aurp_gref == 0) {
		if ((code != AURPCODE_RTINFO) && (code != AURPCODE_DEBUGINFO))
			AURPfreemsg(mdata);
		return;
	}

	switch (code) {
	case AURPCODE_DATAPKT: /* data packet */
		node = (unsigned char)param;
		if (gbuf_next(mdata)) {
			mdata_next = gbuf_next(mdata);
			gbuf_next(mdata) = 0;
			AURPsend(mdata, AUD_Atalk, node);
			do {
				mdata = mdata_next;
				mdata_next = gbuf_next(mdata);
				gbuf_next(mdata) = 0;
				/* Indicate non-AURP packet, node id of peer */
				AURPsend(mdata, AUD_Atalk, node);
			} while (mdata_next);
		} else
			AURPsend(mdata, AUD_Atalk, node);
		break;

	case AURPCODE_RTUPDATE:
		AURPrtupdate(mdata, param);
		break;

	case AURPCODE_RTINFO: /* routing info */
		rtinfo = (aurp_rtinfo_t *)mdata;
		RT_table = (RT_entry *)rtinfo->RT_table;
		ZT_table = (ZT_entry *)rtinfo->ZT_table;
		RT_maxentry = rtinfo->RT_maxentry;
		ZT_maxentry = rtinfo->ZT_maxentry;
		RT_lock = rtinfo->rt_lock;
		RT_insert = (RT_entry *(*)())rtinfo->rt_insert;
		RT_delete = (RT_entry *(*)())rtinfo->rt_delete;
		RT_lookup = (RT_entry *(*)())rtinfo->rt_lookup;
		ZT_add_zname = (int (*)())rtinfo->zt_add_zname;
		ZT_set_zmap = (void (*)())rtinfo->zt_set_zmap;
		ZT_get_zindex = (int (*)())rtinfo->zt_get_zindex;
		ZT_remove_zones = (void (*)())rtinfo->zt_remove_zones;
		break;

	case AURPCODE_DEBUGINFO: /* debug info */
		dbgBits = *(dbgBits_t *)mdata;
		net_port = param;
		break;

	default:
		dPrintf(D_M_AURP, D_L_ERROR, ("AURPcmdx: bad code, %d\n", code));
	}
}
