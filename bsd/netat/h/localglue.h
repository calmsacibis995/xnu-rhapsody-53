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
 * localglue.h:
 * Assorted local glue.
 */

#ifndef LOCALGLUE_H
#define	LOCALGLUE_H

#include <sys/errno.h>
#include <sys/types.h>
#include <sys/param.h>
#include <machine/spl.h>


#define AT_ATP_LINK    111
#define AT_ATP_UNLINK  222
#define AT_ADSP_LINK   333
#define AT_ADSP_UNLINK 444

typedef struct {
	int  maxlen; /* max buffer length */
	int  len;    /* length of data */
	char *buf;   /* pointer to buffer */
} strbuf_t;
typedef struct {
	int  ic_cmd;
	int  ic_timout;
	int  ic_len;
	char *ic_dp;
} ioccmd_t;
typedef struct {
	int  ioc_cmd;
	void *ioc_cr;
	int  ioc_id;
	int  ioc_cnt;
	int  ioc_error;
	int  ioc_rval;
	void	*ioc_private;
	int  ioc_filler[4];
} ioc_t;
#ifndef ioc_count
#define ioc_count ioc_cnt
#endif
#ifndef MOREDATA
#define MOREDATA 1
#endif

#ifdef NEXT
/* 
 * Rhapsody specific defines: Right now they're triggered
 * by the NEXT define. Might want to change that latter
 * ld 04/28/97
 */

#define RHAPSODY 1
/* HZ ticks definition used throughout AppleTalk */
#define HZ hz

/* returned when the operation is not possible at this
 * time (ie when starting up or shutting down.
 * right now, uses ESHUTDOWN because ENOTREADY is not defined
 * in Rhapsody. Need to find a better Error code ###LD
 */
#define ENOTREADY 	ESHUTDOWN
#define ENOMSG 		EOPNOTSUPP
#define EPROTO 		EPROTOTYPE
#define ENOSR		ENOBUFS
#define ECHRNG		EOPNOTSUPP


/* T_MPSAFE is used only in atp_open. I suspect it's a
 * trick to accelerate local atp transactions.
 */
#define T_MPSAFE	0

#define INTERRUPTIBLE   1
#define POLLIN 		0x0001
#define POLLOUT 	0x0002
#define POLLPRI 	0x0004
#define POLLMSG 	0x0080
#define POLLSYNC 	0x8000
#define POLLMSG 	0x0080

/*
 * Define a new Data Type for file. it was DTYPE_OTHER for 
 * AIX, for Rhapsody there is no such define so defines
 * DTYPE_ATALK
 */

#define DTYPE_ATALK -1

typedef unsigned char uchar;

#endif


#ifdef _KERNEL
#include <sys/systm.h>
/* ### LD 4/28/97 tempo removed */
#ifdef _AIX
#include <sys/pri.h>
#include <sys/intr.h>
#include <sys/lockl.h>
#include <net/spl.h>
#else
#include <sys/kernel.h>
#include <sys/proc.h>
#include <sys/filedesc.h>
#include <sys/fcntl.h>
#endif
#include <sys/mbuf.h>

#define AT_WR_OFFSET 38
#ifndef EVENT_NULL
#define EVENT_NULL   -1
#define LOCK_HANDLER  2
#endif
typedef int atevent_t;
/* ### LD 4/28/97 tempo removed */
#ifdef _AIX
typedef Simple_lock atlock_t;
#define ATLOCKINIT(a)   simple_lock_init((void *)&a)
#define ATDISABLE(l, a) (l = disable_lock(PL_IMP, (void *)&a))
#define ATENABLE(l, a)  unlock_enable(l, (void *)&a)
#define ATEVENTINIT(a)  (a = EVENT_NULL)
#define DDP_OUTPUT(m) ddp_putmsg(0,m)
#define StaticProc static

#define PRI_LO		1
#define PRI_MED	2
#define PRI_HI		3

#else

/* ### LD 4/28/97 tempo for next */

typedef simple_lock_t atlock_t;
typedef int *atomic_p; 
#define ATLOCKINIT(a)  (a = EVENT_NULL)
#define ATDISABLE(l, a) (l = splimp())
#define ATENABLE(l, a)  splx(l)
#define ATEVENTINIT(a)  (a = EVENT_NULL)
#define DDP_OUTPUT(m) ddp_putmsg(0,m)
#define StaticProc static

#define PRI_LO		1
#define PRI_MED	2
#define PRI_HI		3


#endif

typedef struct mbuf gbuf_t;
extern gbuf_t *gbuf_copym();
extern gbuf_t *gbuf_dupb();
extern gbuf_t *gbuf_dupm();
extern void gbuf_freeb();
extern void gbuf_freem();
extern gbuf_t *gbuf_alloc();
#define gbuf_prepend(m,len) M_PREPEND(m,len,M_DONTWAIT)
#define gbuf_attach(buf,func,len,arg) m_clattach(buf,func,len,arg,M_DONTWAIT)
#define gbuf_cont(m)	m->m_next
#define gbuf_next(m)	m->m_nextpkt
#define gbuf_rptr(m)	m->m_data
#define gbuf_rinc(m,len)	{m->m_data += len; m->m_len -= len;}
#define gbuf_rdec(m,len)	{m->m_data -= len; m->m_len += len;}
#define gbuf_wptr(m)	(m->m_data + m->m_len)
#define gbuf_winc(m,len)	(m->m_len += len)
#define gbuf_wdec(m,len)	(m->m_len -= len)
#define gbuf_wset(m,len)	(m->m_len = len)
#define gbuf_type(m)	m->m_type
#define gbuf_len(m)	m->m_len

typedef struct gref {
	struct gref *next;
	void *info;
	gbuf_t *ichead;
	gbuf_t *ictail;
	gbuf_t *rdhead;
	gbuf_t *rdtail;
	unsigned char  proto;
	unsigned char  errno;
	unsigned short sevents;
	int pid;
	atlock_t lock;
	atevent_t event;
	atevent_t iocevent;
	int (*writeable)();
	int (*readable)();
	/* for BSD 4.4 we need a selinfo structure for selrecord/selwakeup */
#ifndef _AIX
	struct selinfo si;
#endif
} gref_t;

#undef timeoutcf
#undef timeout
#undef untimeout
#define	MAXATALKTIMERS		128

/* prototypes for the gbuf routines */

gbuf_t *gbuf_alloc(int size, int pri);
void gbuf_freeb(gbuf_t *m);
int gbuf_freel(gbuf_t *m);
void gbuf_linkb(gbuf_t *m1, gbuf_t *m2);
int gbuf_msgsize(gbuf_t *m);
void gbuf_set_type(gbuf_t *m, short mtype);

void data_error(int errno, gbuf_t *mp, gref_t *gref);

#endif		/* _KERNEL */

/*
 * Want these definitions outside the _KERNEL define for admin
 * program access.
 */
#ifdef _AIX
#define MSG_DATA	0x00
#define MSG_PROTO	0x01
#define MSG_IOCTL	0x0e
#define MSG_ERROR	0x8a
#define MSG_HANGUP	0x89
#define MSG_IOCACK	0x81
#define MSG_IOCNAK	0x82
#define MSG_CTL		0x0d
#else
/* ### LD 5/3/97 Rhapsody porting note:
 * Cannot use MSG_DATA = 0, because MT_FREE is defined as 0
 * and the sanity check in m_free cause a panic.
 */
 
#define MSG_DATA	(MT_MAX - 1)
#define MSG_PROTO	(MT_MAX - 2)
#define MSG_IOCTL	(MT_MAX - 3)
#define MSG_ERROR	(MT_MAX - 4)
#define MSG_HANGUP	(MT_MAX - 5)
#define MSG_IOCACK	(MT_MAX - 6)
#define MSG_IOCNAK	(MT_MAX - 7)
#define MSG_CTL		(MT_MAX - 8)
#endif

#endif /* LOCALGLUE_H */
