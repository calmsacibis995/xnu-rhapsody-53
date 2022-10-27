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
 * Copyright (c) 1997 by Apple Computer, Inc., all rights reserved
 * Copyright (c) 1993 NeXT Computer, Inc.
 *
 * UNIX Device switch tables.
 *
 * HISTORY
 *
 * 30 July 1997 Umesh Vaishampayan (umeshv@apple.com)
 * 	enabled file descriptor pseudo-device.
 * 18 June 1993 ? at NeXT
 *	Cleaned up a lot of crap in this file.
 */

#import <sys/param.h>
#import <sys/systm.h>
#import <sys/buf.h>
#import <sys/ioctl.h>
#import <sys/tty.h>
#import <sys/conf.h>


extern int	nulldev();

__private_extern__ int	sdopen(), sdclose(), sdread();
__private_extern__ int	sdwrite(), sdioctl(), sdsize();
__private_extern__ void	sdstrategy();

struct bdevsw	bdevsw[] =
{
	/*
	 *	For block devices, every other block of 8 slots is 
	 *	reserved to NeXT.  The other slots are available for
	 *	the user.  This way we can both add new entries without
	 *	running into each other.  Be sure to fill in NeXT's
	 *	8 reserved slots when you jump over us -- we'll do the
	 *	same for you.
	 */

	/* 0 - 7 are reserved to NeXT */

	NO_BDEVICE,							/* 0*/
	NO_BDEVICE,							/* 1*/
	NO_BDEVICE,							/* 2*/
	NO_BDEVICE,							/* 3*/
	NO_BDEVICE,							/* 4*/
	NO_BDEVICE,							/* 5*/
	{ sdopen,	sdclose,	sdstrategy,	sdioctl,	/* 6*/
	  enodev,	sdsize,		D_DISK },
	NO_BDEVICE,							/* 7*/

	/* 8 - 15 are reserved to the user */
	NO_BDEVICE,							/* 8*/
	NO_BDEVICE,							/* 9*/
	NO_BDEVICE,							/*10*/
	NO_BDEVICE,							/*11*/
	NO_BDEVICE,							/*12*/
	NO_BDEVICE,							/*13*/
	NO_BDEVICE,							/*14*/
	NO_BDEVICE,							/*15*/

	/* 16 - 23 are reserved to NeXT */
	NO_BDEVICE,							/*16*/
	NO_BDEVICE,							/*17*/
	NO_BDEVICE,							/*18*/
	NO_BDEVICE,							/*18*/
	NO_BDEVICE,							/*20*/
	NO_BDEVICE,							/*21*/
	NO_BDEVICE,							/*22*/
	NO_BDEVICE,							/*23*/
};

int	nblkdev = sizeof (bdevsw) / sizeof (bdevsw[0]);

extern struct tty *km_tty[];
extern int	cnopen(),cnclose(),cnread(),cnwrite(),cnioctl(),
		cnselect(),cngetc(), cnputc(dev_t dev, char c);
extern int	kmopen(),kmclose(),kmread(),kmwrite(),kmioctl(),
		kmgetc(), kmputc(dev_t dev, char c);
extern int	sgopen(),sgclose(), sgioctl();

#import <vol.h>
#if NVOL > 0
extern int	volopen(),volclose(),volioctl();
#else
#define	volopen		eno_opcl
#define	volclose	eno_opcl
#define	volioctl	eno_ioctl
#endif

extern int	cttyopen(), cttyread(), cttywrite(), cttyioctl(), cttyselect();

extern int 	mmread(),mmwrite();
#define	mmselect	seltrue
#define mmmmap		eno_mmap

#import <pty.h>
#if NPTY > 0
extern struct tty *pt_tty[];
extern int	ptsopen(),ptsclose(),ptsread(),ptswrite(),ptsstop(),ptsputc();
extern int	ptcopen(),ptcclose(),ptcread(),ptcwrite(),ptcselect(),
		ptyioctl();
#else
#define ptsopen		eno_opcl
#define ptsclose	eno_opcl
#define ptsread		eno_rdwrt
#define ptswrite	eno_rdwrt
#define	ptsstop		nulldev
#define ptsputc		nulldev

#define ptcopen		eno_opcl
#define ptcclose	eno_opcl
#define ptcread		eno_rdwrt
#define ptcwrite	eno_rdwrt
#define	ptcselect	eno_select
#define ptyioctl	eno_ioctl
#endif

extern int	logopen(),logclose(),logread(),logioctl(),logselect();
extern int	fdesc_open(), fdesc_read(), fdesc_write(),
		fdesc_ioctl(), fdesc_select();

extern int	seltrue();

struct cdevsw	cdevsw[] =
{
	/*
	 *	For character devices, every other block of 16 slots is
	 *	reserved to NeXT.  The other slots are available for
	 *	the user.  This way we can both add new entries without
	 *	running into each other.  Be sure to fill in NeXT's
	 *	16 reserved slots when you jump over us -- we'll do the
	 *	same for you.
	 */

	/* 0 - 15 are reserved to NeXT */

    {
	cnopen,		cnclose,	cnread,		cnwrite,	/* 0*/
	cnioctl,	nulldev,	nulldev,	0,		cnselect,
	eno_mmap,	eno_strat,	cngetc,		cnputc, 	D_TTY
    },
    NO_CDEVICE,								/* 1*/
    {
	cttyopen,	nulldev,	cttyread,	cttywrite,	/* 2*/
	cttyioctl,	nulldev,	nulldev,	0,		cttyselect,
	eno_mmap,	eno_strat,	eno_getc,	eno_putc,	D_TTY
    },
    {
	nulldev,	nulldev,	mmread,		mmwrite,	/* 3*/
	eno_ioctl,	nulldev,	nulldev,	0,		mmselect,
	mmmmap,		eno_strat,	eno_getc,	eno_putc,	0
    },
    {
	ptsopen,	ptsclose,	ptsread,	ptswrite,	/* 4*/
	ptyioctl,	ptsstop,	nulldev,	pt_tty,		ttselect,
	eno_mmap,	eno_strat,	eno_getc,	eno_putc,	D_TTY
    },
    {
	ptcopen,	ptcclose,	ptcread,	ptcwrite,	/* 5*/
	ptyioctl,	nulldev,	nulldev,	0,		ptcselect,
	eno_mmap,	eno_strat,	eno_getc,	eno_putc,	D_TTY
    },
    {
	logopen,	logclose,	logread,	eno_rdwrt,	/* 6*/
	logioctl,	eno_stop,	nulldev,	0,		logselect,
	eno_mmap,	eno_strat,	eno_getc,	eno_putc,	0
    },
    NO_CDEVICE,								/* 7*/
    NO_CDEVICE,								/* 8*/
    NO_CDEVICE,								/* 9*/
    NO_CDEVICE,								/*10*/
    NO_CDEVICE,								/*11*/
    {
	kmopen,		kmclose,	kmread,		kmwrite,	/*12*/
	kmioctl,	nulldev,	nulldev,	km_tty,		ttselect,
	eno_mmap,	eno_strat,	kmgetc,		kmputc,		0
    },
    NO_CDEVICE,								/*13*/
    {
	sdopen,		sdclose,	sdread,		sdwrite,	/*14*/
	sdioctl,	eno_stop,	nulldev,	0,		eno_select,
	eno_mmap,	eno_strat,	eno_getc,	eno_putc,	D_TAPE
    },
    NO_CDEVICE,								/*15*/

	/* 16 - 31 are reserved to the user */
    NO_CDEVICE,								/*16*/
    NO_CDEVICE,								/*17*/
    NO_CDEVICE,								/*18*/
    NO_CDEVICE,								/*19*/
    NO_CDEVICE,								/*20*/
    NO_CDEVICE,								/*21*/
    NO_CDEVICE,								/*22*/
    NO_CDEVICE,								/*23*/
    NO_CDEVICE,								/*24*/
    NO_CDEVICE,								/*25*/
    NO_CDEVICE,								/*26*/
    NO_CDEVICE,								/*27*/
    NO_CDEVICE,								/*28*/
    NO_CDEVICE,								/*29*/
    NO_CDEVICE,								/*30*/
    NO_CDEVICE,								/*31*/

	/* 32 - 47 are reserved to NeXT */
    {
	fdesc_open,	eno_opcl,	fdesc_read,	fdesc_write,	/*32*/
	fdesc_ioctl,	eno_stop,	eno_reset,	0,		fdesc_select,
	eno_mmap,	eno_strat,	eno_getc,	eno_putc,	0
    },
    {
	sgopen,		sgclose,	eno_rdwrt,	eno_rdwrt,	/*33*/
	sgioctl,	eno_stop,	eno_reset,	0,		eno_select,
	eno_mmap,	eno_strat,	eno_getc,	eno_putc,	D_TAPE
    },
    NO_CDEVICE,								/*34*/
    NO_CDEVICE,								/*35*/
    NO_CDEVICE,								/*36*/
    NO_CDEVICE,								/*37*/
    NO_CDEVICE,								/*38*/
    NO_CDEVICE,								/*39*/
    NO_CDEVICE,								/*40*/
    NO_CDEVICE,								/*41*/
    {
	volopen,	volclose,	eno_rdwrt,	eno_rdwrt,	/*42*/
	volioctl,	eno_stop,	eno_reset,	0,		seltrue,
	eno_mmap,	eno_strat,	eno_getc,	eno_putc,	0
    },
};
int	nchrdev = sizeof (cdevsw) / sizeof (cdevsw[0]);


#import	<sys/vnode.h> /* for VCHR and VBLK */
/*
 * return true if a disk
 */
int
isdisk(dev, type)
	dev_t dev;
	int type;
{
	switch (major(dev)) {
	case 1:	/* fd: floppy */
	case 6:	/* sd: scsi disk */
	case 3:	/* ide:  */
		if (type == VBLK)
			return(1);
		break;
	case 14:	 /* sd: scsi disk */
	case 41: /* fd: floppy */
		if (type == VCHR)
			return(1);
		break;
	}
	return(0);
}

static int chrtoblktab[] = {
	/* CHR*/	/* BLK*/	/* CHR*/	/* BLK*/
	/*  0 */	NODEV,		/*  1 */	NODEV,
	/*  2 */	NODEV,		/*  3 */	NODEV,
	/*  4 */	NODEV,		/*  5 */	NODEV,
	/*  6 */	NODEV,		/*  7 */	NODEV,
	/*  8 */	NODEV,		/*  9 */	NODEV,
	/* 10 */	NODEV,		/* 11 */	NODEV,
	/* 12 */	NODEV,		/* 13 */	NODEV,
	/* 14 */	6,		/* 15 */	NODEV,
	/* 16 */	NODEV,		/* 17 */	NODEV,
	/* 18 */	NODEV,		/* 19 */	NODEV,
	/* 20 */	NODEV,		/* 21 */	NODEV,
	/* 22 */	NODEV,		/* 23 */	NODEV,
	/* 24 */	NODEV,		/* 25 */	NODEV,
	/* 26 */	NODEV,		/* 27 */	NODEV,
	/* 28 */	NODEV,		/* 29 */	NODEV,
	/* 30 */	NODEV,		/* 31 */	NODEV,
	/* 32 */	NODEV,		/* 33 */	NODEV,
	/* 34 */	NODEV,		/* 35 */	NODEV,
	/* 36 */	NODEV,		/* 37 */	NODEV,
	/* 38 */	NODEV,		/* 39 */	NODEV,
	/* 40 */	NODEV,		/* 41 */	1,
	/* 42 */	NODEV,		/* 43 */	NODEV,
	/* 44 */	NODEV,
};

/*
 * convert chr dev to blk dev
 */
dev_t
chrtoblk(dev)
	dev_t dev;
{
	int blkmaj;

	if (major(dev) >= nchrdev)
		return(NODEV);
	blkmaj = chrtoblktab[major(dev)];
	if (blkmaj == NODEV)
		return(NODEV);
	return(makedev(blkmaj, minor(dev)));
}

/*
 * Returns true if dev is /dev/mem or /dev/kmem.
 */
int iskmemdev(dev)
	dev_t dev;
{

	return (major(dev) == 3 && minor(dev) < 2);
}
