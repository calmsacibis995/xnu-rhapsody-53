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
 *	Copyright (c) 1995-1998 Apple Computer, Inc. 
 *
 *	The information contained herein is subject to change without
 *	notice and  should not be  construed as a commitment by Apple
 *	Computer, Inc. Apple Computer, Inc. assumes no responsibility
 *	for any errors that may appear.
 *
 *	Confidential and Proprietary to Apple Computer, Inc.
 *
 */
#include <h/sysglue.h>
#include <h/debug.h>
#include <sys/malloc.h>
#ifdef _AIX
	#include <sys/device.h>
#else
#include <sys/file.h>
#endif

int (*sys_ATsocket)() = 0;
int (*sys_ATgetmsg)() = 0;
int (*sys_ATputmsg)() = 0;
int (*sys_ATPsndreq)() = 0;
int (*sys_ATPsndrsp)() = 0;
int (*sys_ATPgetreq)() = 0;
int (*sys_ATPgetrsp)() = 0;

#ifndef _AIX

int ATsocket(proc, uap, retval)
	void *proc;
	struct {
	int proto;
	} *uap;
	int *retval;
{
	int err;

	if (sys_ATsocket)
		*retval = (*sys_ATsocket)(uap->proto, &err, proc);
	else {
		*retval = -1;
		err = ENXIO;
	}
	return err;
}

int ATgetmsg(proc, uap, retval)
	void *proc;
	struct {
	int fd;
	void *ctlptr;
	void *datptr;
	int *flags;
	} *uap;
	int *retval;
{
	int err;

	if (sys_ATgetmsg)
		*retval = (*sys_ATgetmsg)(uap->fd,
			uap->ctlptr, uap->datptr, uap->flags, &err, proc);
	else {
		*retval = -1;
		err = ENXIO;
	}
	return err;
}

int ATputmsg(proc, uap, retval)
	void *proc;
	struct {
	int fd;
	void *ctlptr;
	void *datptr;
	int flags;
	} *uap;
	int *retval;
{
	int err;

	if (sys_ATputmsg)
		*retval = (*sys_ATputmsg)(uap->fd,
			uap->ctlptr, uap->datptr, uap->flags, &err, proc);
	else {
		*retval = -1;
		err = ENXIO;
	}
	return err;
}

int ATPsndreq(proc, uap, retval)
	void *proc;
	struct {
	int fd;
	unsigned char *buf;
	int len;
	int nowait;
	} *uap;
	int *retval;
{
	int err;

	if (sys_ATPsndreq)
		*retval = (*sys_ATPsndreq)(uap->fd,
			uap->buf, uap->len, uap->nowait, &err, proc);
	else {
		*retval = -1;
		err= ENXIO;
	}
	return err;
}

int ATPsndrsp(proc, uap, retval)
	void *proc;
	struct {
	int fd;
	unsigned char *respbuff;
	int resplen;
	int datalen;
	} *uap;
	int *retval;
{
	int err;

	if (sys_ATPsndrsp)
		*retval = (*sys_ATPsndrsp)(uap->fd,
			uap->respbuff, uap->resplen, uap->datalen, &err, proc);
	else {
		*retval = -1;
		err = ENXIO;
	}
	return err;
}

int ATPgetreq(proc, uap, retval)
	void *proc;
	struct {
	int fd;
	unsigned char *buf;
	int buflen;
	} *uap;
	int *retval;
{
	int err;

	if (sys_ATPgetreq)
		*retval = (*sys_ATPgetreq)(uap->fd,
			uap->buf, uap->buflen, &err, proc);
	else {
		*retval = -1;
		err = ENXIO;
	}
	return err;
}

int ATPgetrsp(proc, uap, retval)
	void *proc;
	struct {
	int fd;
	unsigned char *bdsp;
	} *uap;
	int *retval;
{
	int err;

	if (sys_ATPgetrsp)
		*retval = (*sys_ATPgetrsp)(uap->fd, uap->bdsp, &err, proc);
	else {
		*retval = -1;
		err = ENXIO;
	}
	return err;
}

void *at_cl_alloc(size, wait)
     int size;    /* in bytes */
     int wait;
{
	return((void *)_MALLOC(size, M_MCLUST, (wait)? M_WAITOK: M_NOWAIT));
}

void at_cl_free(buf)
     void *buf;
{
	FREE(buf, M_MCLUST);
}

int atalk_closeref(fp, grefp)
	struct file *fp;
	gref_t **grefp;
{
	*grefp = (gref_t *)fp->f_data;
	fp->f_data = 0;
	return 0;
}

int atalk_openref(gref, retfd, proc)
	gref_t *gref;
	int *retfd;
	struct proc *proc;
{
extern int _ATread(), _ATwrite(),_ATioctl(), _ATselect(), _ATclose();
static struct fileops fileops = {_ATread, _ATwrite, _ATioctl, _ATselect, _ATclose};
	int err, fd;
	struct file *fp;

	if ((err = falloc(proc, &fp, &fd)) != 0)
		return err;
	fp->f_flag = FREAD|FWRITE;
/*##### LD 5/7/96 Warning: we don't have a "DTYPE_OTHER" for
 * Rhapsody, so defines DTYPE_ATALK as DTYPE_SOCKET...
 */
	fp->f_type = DTYPE_ATALK+1;
	fp->f_ops = &fileops;
	*fdflags(proc, fd) &= ~UF_RESERVED;
	*retfd = fd;
	fp->f_data = (void *)gref;
	gref->next = (void *)fp;
	return 0;
}

int atalk_getref(fp, fd, grefp, proc)
	struct file *fp;
	int fd;
	gref_t **grefp;
	struct proc *proc;
{
	if (fp == 0) {
               int error = fdgetf(proc, fd, &fp);

               if (error) {

			*grefp = (gref_t *)0;
			return EBADF;
		}
	}
	if ((*grefp = (gref_t *)fp->f_data) == 0)
		return EBADF;
	return 0;
}

gbuf_t *gbuf_alloc_wait(size, wait)
	int size;
	int wait;
{
	gbuf_t *m;
/*
 * ### LD 5/12/97 Allocate only a mbuf if size is ok,
 *     otherwise add a cluster.
 */
	if (size > MCLBYTES) {
		void *buf;
		if ((buf = at_cl_alloc(size, wait)) == 0) {
#ifdef APPLETALK_DEBUG
			kprintf("gbuf_alloc: size =%d at_cl_alloc failed\n", 
				size);
#endif
			return NULL;
		}
		if ((m = (gbuf_t *)gbuf_attach( buf, at_cl_free, size, 0)) == 0) {
#ifdef APPLETALK_DEBUG
		  	kprintf("gbuf_alloc: size =%d gbuf_attach failed\n", 
				size);
#endif
			at_cl_free(buf);
			return NULL;
		}
/*
		kprintf("gbuf_alloc: allocated with MALLOC m=%x size =%d \n", 
			m, size);
*/
	}	
	else {
		if (!(m = 
		      (gbuf_t *)m_gethdr((wait)? M_WAIT: M_DONTWAIT, MSG_DATA)))
			return(NULL);
		if (size > MHLEN) {
			MCLGET(m, (wait)? M_WAIT: M_DONTWAIT);
			if (!(m->m_flags & M_EXT)) {
				(void)m_free(m);
				return(NULL);
			}
		}
	}
	gbuf_next(m) = 0;
	gbuf_cont(m) = 0;
	gbuf_wset(m,0);
	return m;
} /* gbuf_alloc_wait */

gbuf_t *gbuf_alloc(size, pri)
	int size;
	int pri;
{
	return(gbuf_alloc_wait(size, FALSE));
}

void gbuf_set_type(m, mtype)
     gbuf_t *m;
     short mtype;
{
  MBUF_LOCK();
  mbstat.m_mtypes[m->m_type]--;
  m->m_type = mtype;
  mbstat.m_mtypes[mtype]++;
  MBUF_UNLOCK();
}

void gbuf_freeb(m)
	gbuf_t *m;
{
	if (m) {
/*
		kprintf("gbuf_freeb: calling m_free for m=%x\n", m);
*/
		(void)m_free((struct mbuf *)m);
	}
}

/*
 * LD 5/12/97 Added for Rhapsody, defines a m_clattach function that:
 * "Allocates an mbuf structure and attaches an external cluster."
 */

struct mbuf *m_clattach(extbuf, extfree, extsize, extarg, wait)
	caddr_t extbuf;	
	int (*extfree)();
	int extsize;
	int extarg;
	int wait;
{
        struct mbuf *m;

	m = m_gethdr(wait, MSG_DATA);
/*
	kprintf("m_clattach: for mbuf = %x of extbuf =%x extsize=%d extfree=%x\n",
		m, extbuf, extsize, extfree);
*/
        if (m == NULL)
                return (NULL);
        m->m_ext.ext_buf = extbuf;
        m->m_ext.ext_free = extfree;
        m->m_ext.ext_size = extsize;
        m->m_ext.ext_arg = extarg;
        m->m_ext.ext_refs.forward = m->m_ext.ext_refs.backward =
                        &m->m_ext.ext_refs;
        m->m_data = extbuf;
        m->m_flags |= M_EXT;
        m->m_pkthdr.len = extsize;
        m->m_len = extsize;

        return (m);

}
void *atalk_timeout(func, arg, ticks)
	void (*func)();
	void *arg;
	int ticks;
{
/*
	dPrintf(D_M_ATP,D_L_VERBOSE,
			"atalk_timeout: func=%x,arg=%x,time=%d\n", func,arg,ticks, 0, 0);
*/
	timeout(func, arg, ticks);
	return arg;
}

void atalk_untimeout(func, arg, trb)
	void (*func)();
	void *arg;
	void *trb;
{
/*
	dPrintf(D_M_ATP,D_L_VERBOSE,
		"atalk_untimeout: func=%x,arg=%x, trb=%d\n", func,arg,trb, 0, 0);
*/
	untimeout(func, arg);
}

#else  /* This AIX code (to the end of this file) is no longer supported. */

int ATsocket(proto)  /* AIX version */
	int proto;
{
	int err, rc = -1;

	if (sys_ATsocket)
		rc = (*sys_ATsocket)(proto, &err, 0);
	else
		err = ENXIO;
	if (err)
		setuerror(err);
	return rc;
}

int ATgetmsg(fd, ctlptr, datptr, flags)  /* AIX version */
	int fd;
	void *ctlptr;
	void *datptr;
	int *flags;
{
	int err, rc = -1;

	if (sys_ATgetmsg)
		rc = (*sys_ATgetmsg)(fd, ctlptr, datptr, flags, &err, 0);
	else
		err = ENXIO;
	if (err)
		setuerror(err);
	return rc;
}

int ATputmsg(fd, ctlptr, datptr, flags)  /* AIX version */
	int fd;
	void *ctlptr;
	void *datptr;
	int flags;
{
	int err, rc = -1;

	if (sys_ATputmsg)
		rc = (*sys_ATputmsg)(fd, ctlptr, datptr, flags, &err, 0);
	else
		err = ENXIO;
	if (err)
		setuerror(err);
	return rc;
}

int ATPsndreq(fd, buf, len, nowait)   /* AIX version */
	int fd;
	unsigned char *buf;
	int len;
	int nowait;
{
	int err, rc = -1;

	if (sys_ATPsndreq)
		rc = (*sys_ATPsndreq)(fd, buf, len, nowait, &err, 0);
	else
		err = ENXIO;
	if (err)
		setuerror(err);
	return rc;
}

int ATPsndrsp(fd, respbuff, resplen, datalen)   /* AIX version */
	int fd;
	unsigned char *respbuff;
	int resplen;
	int datalen;
{
	int err, rc = -1;

	if (sys_ATPsndrsp)
		rc = (*sys_ATPsndrsp)(fd, respbuff, resplen, datalen, &err, 0);
	else
		err = ENXIO;
	if (err)
		setuerror(err);
	return rc;
}

int ATPgetreq(fd, buf, buflen)  /* AIX version */
	int fd;
	unsigned char *buf;
	int buflen;
{
	int err, rc = -1;

	if (sys_ATPgetreq)
		rc = (*sys_ATPgetreq)(fd, buf, buflen, &err, 0);
	else
		err = ENXIO;
	if (err)
		setuerror(err);
	return rc;
}

int ATPgetrsp(fd, bdsp)  /* AIX version */
	int fd;
	unsigned char *bdsp;
{
	int err, rc = -1;

	if (sys_ATPgetrsp)
		rc = (*sys_ATPgetrsp)(fd, bdsp, &err, 0);
	else
		err = ENXIO;
	if (err)
		setuerror(err);
	return rc;
}

void *atalk_kalloc(size)   /* AIX version */
	int size;
{
     return (void *)xmalloc(size, 2, pinned_heap);
}

void atalk_kfree(buf)   /* AIX version */
	void *buf;
{
	xmfree(buf, pinned_heap);
}

int atalk_closeref(fp, grefp)   /* AIX version */
	struct file *fp;
	gref_t **grefp;
{
	*grefp = (gref_t *)fp->f_data;
	fp->f_data = 0;
	return 0;
}

int atalk_openref(gref, retfd, proc)  /* AIX version */
	gref_t *gref;
	int *retfd;
	void *proc;
{
extern int _ATrw(), _ATioctl(), _ATselect(), _ATclose(), _ATstat();
static struct fileops fileops = {_ATrw, _ATioctl, _ATselect, _ATclose, _ATstat};
	int err, fd;
	struct file *fp;
	void *crp;

	crp = (void *)crref();
#ifdef _AIX
	if ((err = ufdcreate(FREAD|FWRITE,
			&fileops, 0, DTYPE_OTHER, &fd, crp)) != 0)
#else
	if ((err = ufdcreate(FREAD|FWRITE,
			&fileops, 0, DTYPE_ATALK, &fd, crp)) != 0)
#endif
		return err;
	*retfd = fd;
	fp = U.U_ufd[fd].fp;
	fp->f_data = (void *)gref;
	gref->next = (void *)fp;
	return 0;
}

int atalk_getref(fp, fd, grefp, proc)  /* AIX version */
	struct file *fp;
	int fd;
	gref_t **grefp;
	struct proc *proc;
{
	if (fp == 0) {
		if ((fd < 0) || (fd > U.U_maxofile) || ((fp = U.U_ufd[fd].fp) == 0)) {
			*grefp = (gref_t *)0;
			return EBADF;
		}
	}
	if ((*grefp = (gref_t *)fp->f_data) == 0)
		return EBADF;
	return 0;
}

gbuf_t *gbuf_alloc(size, pri)  /* AIX version */
	int size;
	int pri;
{
	gbuf_t *m;

	m = (size > MHLEN) ? (gbuf_t *)m_getclustm(M_DONTWAIT, MSG_DATA, size)
		: (gbuf_t *)m_gethdr(M_DONTWAIT, MSG_DATA);
#ifdef APPLETALK_DEBUG
	kprintf("gbuf_alloc: for size = %d m=%x\n", size, m);
#endif
	gbuf_next(m) = 0;
	gbuf_cont(m) = 0;
	gbuf_wset(m,0);
	return m;
}

void gbuf_freeb(m)  /* AIX version */
	gbuf_t *m;
{
	if (m)
		m_free(m);
}

static struct trb *trb_freehead = 0;
static struct trb *trb_freetail = 0;
static struct trb *trb_pendhead = 0;
static int trb_cnt = 0;
static atlock_t trb_lock;

static void atalk_rem_timeoutcf()  /* AIX version */
{
	register int s;
	register struct trb *trb;
	register struct trb *tmp_freehead, *tmp_pendhead;

	ATDISABLE(s, trb_lock);
	tmp_freehead = trb_freehead;
	trb_freehead = 0;
	tmp_pendhead = trb_pendhead;
	trb_pendhead = 0;
	trb_cnt = 0;
	ATENABLE(s, trb_lock);
	while ((trb = tmp_pendhead) != 0) {
		tmp_pendhead = trb->to_next;
		while (tstop(trb));
		tfree(trb);
	}
	while ((trb = tmp_freehead) != 0) {
		tmp_freehead = trb->to_next;
		tfree(trb);
	}
	dPrintf(D_M_ATP,D_L_ERROR, "atalk: timer stopped!\n",0,0,0,0,0);
}

static void atalk_timeoutcf(cnt)  /* AIX version */
	int cnt;
{
	register int i;
	register struct trb *trb;

	if (trb_freehead == 0) {
		for (i=0; i < cnt-1; i++) {
			trb = (struct trb *)talloc();
			trb->to_next = trb_freehead;
			trb_freehead = trb;
			if (!i) trb_freetail = trb;
			trb_cnt++;
		}
	}
	ATLOCKINIT(trb_lock);
}

static void atalk_clock(trb)  /* AIX version */
	register struct trb *trb;
{
	register int s;
	register struct trb *next;
	void (*tof)();
	void *arg;

	ATDISABLE(s, trb_lock);
	if (trb_pendhead && trb->func) {
		/*
		 * remove the timeout from the pending queue
		 */
		if (trb_pendhead == trb)
			trb_pendhead = trb->to_next;
		else {
			for (next=trb_pendhead; next->to_next; next=next->to_next) {
				if (next->to_next == trb) {
					next->to_next = trb->to_next;
					trb->func = 0;
					break;
				}
			}
			if (trb->func) {
				dPrintf(D_M_ATP,D_L_WARNING,
					"atalk_clock: %d,%x,%x\n", trb_cnt,trb,trb_pendhead,0,0);
				/*
				 * we have not found the trb in the pending list - something
				 * has gone wrong here. maybe the trb has been returned to
				 * the free list; in which case, we should simply ignore
				 * this timeout event!
				 */
				for (next=trb_freehead; next; next=next->to_next) {
					if (next == trb)
					{
						ATENABLE(s, trb_lock);
						return;
					}
				}
				/*
				 * the trb is not in the free list either - something has
				 * really gone wacky here! all we can do now is put the
				 * trb back into the free list and hope that it will be ok.
				 */
				trb->to_next = 0;
				if (trb_freehead)
					trb_freetail->to_next = trb;
				else
					trb_freehead = trb;
				trb_freetail = trb;
				trb_cnt++;
				ATENABLE(s, trb_lock);
				return;
			}
		}

		/*
		 * process the timeout
		 */
		trb->func = 0;
		trb->to_next = 0;
		tof = trb->tof;
		trb->tof = 0;
		arg = (void *)trb->func_data;
		trb->func_data = 999;
		if (trb_freehead)
			trb_freetail->to_next = trb;
		else
			trb_freehead = trb;
		trb_freetail = trb;
		trb_cnt++;
		ATENABLE(s, trb_lock);
		if (tof) {
			dPrintf(D_M_ATP,D_L_VERBOSE, "atalk_clock: func=%x, arg=%x, %d\n",
				tof,arg,trb_cnt,0,0);
			(*tof)(arg);
		} else {
			dPrintf(D_M_ATP,D_L_ERROR, "atalk_clock: func=%x, arg=%x, %d\n",
				tof,arg,trb_cnt,0,0);
		}
	} else
		ATENABLE(s, trb_lock);
}

void *atalk_timeout(func, arg, ticks)  /* AIX version */
	void (*func)();
	void *arg;
	int ticks;
{
	register int s;
	register struct trb *trb;

	dPrintf(D_M_ATP,D_L_VERBOSE,
		"atalk_timeout: func=%x,arg=%x,time=%d, %d,%x\n", func,arg,ticks,trb_cnt,trb_pendhead);
	/*
	 * set up the timeout request
	 */
	ATDISABLE(s, trb_lock);
	if ((trb = trb_freehead) == 0) {
		ATENABLE(s, trb_lock);
		dPrintf(D_M_ATP,D_L_WARNING,
			"atalk_timeout: NO TRB! time=%d, %d\n", ticks,trb_cnt,0,0,0);
		return 0;
	}
	trb_freehead = trb->to_next;
	trb->to_next = trb_pendhead;
	trb_pendhead = trb;
	trb_cnt--;
	trb->timeout.it_value.tv_sec  =  ticks / HZ;
	trb->timeout.it_value.tv_nsec = (ticks % HZ) * (NS_PER_SEC / HZ);
	trb->knext     = 0;
	trb->kprev     = 0;
	trb->flags     = 0;
	trb->tof       = func;
	trb->func      = (void (*)())atalk_clock;
	trb->func_data = (ulong)arg;
	trb->ipri      = PL_IMP;
	trb->id        = -1;

	/*
	 * start the timeout
	 */
	ATENABLE(s, trb_lock);
	tstart(trb);
	return (void *)trb;
}

void atalk_untimeout(func, arg, trb)  /* AIX version */
	void (*func)();
	void *arg;
	register struct trb *trb;
{
	register int s;
	register struct trb *next;

	dPrintf(D_M_ATP,D_L_VERBOSE,
		"atalk_untimeout: func=%x,arg=%x, %d\n", func,arg,trb_cnt,0,0);

	ATDISABLE(s, trb_lock);
	if (trb == 0) {
		for (trb=trb_pendhead; trb; trb=trb->to_next) {
			if ((func == trb->tof) && (arg == (void *)trb->func_data))
				break;
		}
	}
	if (trb && (trb->func == (void (*)())atalk_clock)
			&& (func == trb->tof) && (arg == (void *)trb->func_data)) {
		trb->func_data = 999;
		if (!(trb->flags & T_PENDING))
			{
			trb->tof = 0;
			ATENABLE(s, trb_lock);
			return;
			}
		trb->func = 0;
		while (tstop(trb));
		if (trb_pendhead == trb)
			trb_pendhead = trb->to_next;
		else {
			for (next=trb_pendhead; next->to_next != trb; next=next->to_next) {
				if (next->to_next == 0) {
					ATENABLE(s, trb_lock);
					dPrintf(D_M_ATP,D_L_WARNING,
						"atalk_untimeout: UNKNOWN TRB %x...\n",trb,0,0,0,0);
					return;
				}
			}
			next->to_next = trb->to_next;
		}
		trb->to_next = 0;
		trb_freetail->to_next = trb;
		trb_freetail = trb;
		trb_cnt++;
	}
	ATENABLE(s, trb_lock);
}

int config_atalk(dev, cmd, uiop)  /* AIX only */
dev_t dev;
int cmd;
void *uiop;
{
	static int loaded = 0;
	int err, nest;

	err = 0;
	nest = lockl(&kernel_lock, LOCK_SHORT);

	if (cmd == CFG_INIT) {
		if (loaded)
			goto out;
		vm_protect(0, 4096, 3);
		atalk_timeoutcf(256);
		atalk_load();
		loaded = 1;

	} else if (cmd == CFG_TERM) {
		if (!loaded)
			goto out;
		atalk_rem_timeoutcf();
		atalk_unload();
		loaded = 0;

	} else
		err =  EINVAL;

out:
	if (nest != LOCK_NEST)
		unlockl(&kernel_lock);
	return(err);
}

#endif
