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

/* Copyright (c) 1997 Apple Computer, Inc. All Rights Reserved */
/*-
 * Copyright (c) 1982, 1986, 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)tty_conf.c	8.4 (Berkeley) 1/21/94
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/tty.h>
#include <sys/conf.h>

#ifndef MAXLDISC
#define MAXLDISC 8
#endif

#ifndef NeXT
static l_open_t		l_noopen;
static l_close_t	l_noclose;
static l_ioctl_t	l_nullioctl;
static l_rint_t		l_norint;
static l_start_t	l_nostart;
#else /* NeXT */
#define	l_noopen	((int (*) __P((dev_t, struct tty *)))enodev)
#define	l_noclose	((int (*) __P((struct tty *, int flags)))enodev)
#define	l_noread	((int (*) __P((struct tty *, struct uio *, int)))enodev)
#define	l_nowrite	l_noread
#define	l_norint	((int (*) __P((int c, struct tty *)))enodev)
#define	l_nostart	((int (*) __P((struct tty *)))enodev)
static	int
l_nullioctl(struct tty *tp, u_long cmd, caddr_t data, int flag, struct proc *p);
#endif /* !NeXT */

/*
 * XXX it probably doesn't matter what the entries other than the l_open
 * entry are here.  The l_nullioctl and ttymodem entries still look fishy.
 * Reconsider the removal of nullmodem anyway.  It was too much like
 * ttymodem, but a completely null version might be useful.
 */
#define NODISC(n) \
	{ l_noopen,	l_noclose,	l_noread,	l_nowrite, \
	  l_nullioctl,	l_norint,	l_nostart,	ttymodem }

struct	linesw linesw[MAXLDISC] =
{
				/* 0- termios */
	{ ttyopen,	ttylclose,	ttread,		ttwrite,
	  l_nullioctl,	ttyinput,	ttstart,	ttymodem },
	NODISC(1),		/* 1- defunct */
	  			/* 2- NTTYDISC */
#ifdef COMPAT_43
	{ ttyopen,	ttylclose,	ttread,		ttwrite,
	  l_nullioctl,	ttyinput,	ttstart,	ttymodem },
#else
	NODISC(2),
#endif
	NODISC(3),		/* TABLDISC */
	NODISC(4),		/* SLIPDISC */
	NODISC(5),		/* PPPDISC */
	NODISC(6),		/* loadable */
	NODISC(7),		/* loadable */
};

int	nlinesw = sizeof (linesw) / sizeof (linesw[0]);

static struct linesw nodisc = NODISC(0);

#define LOADABLE_LDISC 6
/*
 * ldisc_register: Register a line discipline.
 *
 * discipline: Index for discipline to load, or LDISC_LOAD for us to choose.
 * linesw_p:   Pointer to linesw_p.
 *
 * Returns: Index used or -1 on failure.
 */
int
ldisc_register(discipline, linesw_p)
	int discipline;
	struct linesw *linesw_p;
{
	int slot = -1;

	if (discipline == LDISC_LOAD) {
		int i;
		for (i = LOADABLE_LDISC; i < MAXLDISC; i++)
			if (bcmp(linesw + i, &nodisc, sizeof(nodisc)) == 0) {
				slot = i;
			}
	}
	else if (discipline >= 0 && discipline < MAXLDISC) {
		slot = discipline;
	}

	if (slot != -1 && linesw_p)
		linesw[slot] = *linesw_p;

	return slot;
}

/*
 * ldisc_deregister: Deregister a line discipline obtained with
 * ldisc_register.  Can only deregister "loadable" ones now.
 *
 * discipline: Index for discipline to unload.
 */
void
ldisc_deregister(discipline)
	int discipline;
{
	if (discipline >= LOADABLE_LDISC && discipline < MAXLDISC) {
		linesw[discipline] = nodisc;
	}
}

#ifndef NeXT
static int
l_noopen(dev, tp)
	dev_t dev;
	struct tty *tp;
{

	return (ENODEV);
}

static int
l_noclose(tp, flag)
	struct tty *tp;
	int flag;
{

	return (ENODEV);
}

int
l_noread(tp, uio, flag)
	struct tty *tp;
	struct uio *uio;
	int flag;
{

	return (ENODEV);
}

int
l_nowrite(tp, uio, flag)
	struct tty *tp;
	struct uio *uio;
	int flag;
{

	return (ENODEV);
}

static int
l_norint(c, tp)
	int c;
	struct tty *tp;
{

	return (ENODEV);
}

static int
l_nostart(tp)
	struct tty *tp;
{

	return (ENODEV);
}
#endif /* !NeXT */

/*
 * Do nothing specific version of line
 * discipline specific ioctl command.
 */
static int
l_nullioctl(tp, cmd, data, flags, p)
	struct tty *tp;
	u_long cmd;
	caddr_t data;
	int flags;
	struct proc *p;
{

	return (-1);
}
