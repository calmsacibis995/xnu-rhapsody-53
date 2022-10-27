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

/* Copyright (c) 1995 NeXT Computer, Inc. All Rights Reserved */
/*
 * Mach Operating System
 * Copyright (c) 1987 Carnegie-Mellon University
 * All rights reserved.  The CMU software License Agreement specifies
 * the terms and conditions for use and redistribution.
 */
/* 
 * HISTORY
 * 11 Dec 1997 Umesh Vaishampayan (umeshv@apple.com)
 *	Modified execve() to do the allocation for arguments before locking the inode. Cleaned up
 *	warnings.
 *
 * 20 Jun 1995 Mac Gillon (mgillon) at NeXT
 *	4.4 based version
 *	
 * 18 May 1992 ? at NeXT
 *	Expanded fat file support to handle little endian machines.
 *
 *  6-Dec-91  Peter King (king) at NeXT
 *	NeXT: Fat file support.
 *
 *  7-Nov-90  Gregg Kellogg (gk) at NeXT
 *	NeXT: reset signal interrupt bits on exec.
 *
 * 25-Sep-89  Morris Meyer (mmeyer) at NeXT
 *	NFS 4.0 Changes:  Changed SPAGI to SPAGV.  Removed #include dir.h.
 *
 * 24-Oct-88  Steve Stone (steve) at NeXT
 *	Added Attach trace support.
 *
 * 22-Jul-88  Avadis Tevanian, Jr. (avie) at NeXT
 *	Support for Mach-O files.
 *
 * 19-Apr-88  Avadis Tevanian, Jr. (avie) at NeXT
 *	Removed old history, purged lots of dead code.
 */
 
#include <cputypes.h>

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
 *	from: @(#)kern_exec.c	8.1 (Berkeley) 6/10/93
 */
#include <mach_nbc.h>
#include <machine/reg.h>

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/filedesc.h>
#include <sys/kernel.h>
#include <sys/proc.h>
#include <sys/user.h>
#include <sys/buf.h>
#include <sys/socketvar.h>
#include <sys/malloc.h>
#include <sys/namei.h>
#include <sys/mount.h>
#include <sys/vnode.h>		
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/acct.h>
#include <sys/exec.h>
#include <mach/exception.h>

#include <sys/signal.h>
#include <kern/task.h>
#include <kern/thread.h>

#include <mach/vm_param.h>
#include <vm/vm_map.h>
#include <vm/vm_object.h>
#include <vm/vnode_pager.h>
#include <vm/vm_kern.h>
#include <vm/vm_user.h>
#include <kern/mapfs.h>
#include <kern/zalloc.h>

#include <kern/parallel.h>
#include <kern/mach_loader.h>

#include <mach-o/fat.h>
#include <mach-o/loader.h>

#include <machine/vmparam.h>
#include <kernserv/c_utils.h>

static int load_return_to_errno(load_return_t lrtn);
int execve(struct proc *p, struct execve_args *uap, register_t *retval);

int
execv(p, args, retval)
	struct proc *p;
	void *args;
	int *retval;
{
	((struct execve_args *)args)->envp = NULL;
	return (execve(p, args, retval));
}

/* ARGSUSED */
int
execve(p, uap, retval)
	register struct proc *p;
	register struct execve_args *uap;
	register_t *retval;
{
	register struct ucred *cred = p->p_ucred;
	register struct filedesc *fdp = p->p_fd;
	register nc;
	register char *cp;
	int na, ne, ucp, ap, cc;
	unsigned len;
	int indir;
	char *sharg;
	char *execnamep;
	struct vnode *vp;
	struct vattr vattr;
	struct vattr origvattr;
	vm_offset_t execargs;
	struct nameidata nd;
	struct ps_strings ps;
#define	SHSIZE	512
	char cfarg[SHSIZE];
	boolean_t		is_fat;
	struct mach_header	*mach_header;
	struct fat_header	*fat_header;
	struct fat_arch		fat_arch;
	load_return_t		lret;
	load_result_t		load_result;
	struct uthread		*uthread;
	int i;
	union {
		/* #! and name of interpreter */
		char			ex_shell[SHSIZE];
		/* Mach-O executable */
		struct mach_header	mach_header;
		/* Fat executable */
		struct fat_header	fat_header;
		char	pad[512];
	} exdata;
	int resid, error;

	/* Start with an allocation */
	execargs = kmem_alloc_wait(kernel_pageable_map, NCARGS);

	uthread = current_thread()->_uthread;
	NDINIT(&nd, LOOKUP, FOLLOW | LOCKLEAF | SAVENAME,
					UIO_USERSPACE, uap->fname, p);
	if ((error = namei(&nd)))
		goto bad1;
	vp = nd.ni_vp;
	VOP_LEASE(vp, p, p->p_ucred, LEASE_READ);

	if ((error = VOP_GETATTR(vp, &origvattr, p->p_ucred, p)))
		goto bad;

	/* Check mount point */
	if (vp->v_mount->mnt_flag & MNT_NOEXEC) {
		error = EACCES;
		goto bad;
	}

	indir = 0;
	if ((vp->v_mount->mnt_flag & MNT_NOSUID) || (p->p_flag & P_TRACED))
		origvattr.va_mode &= ~(VSUID | VSGID);
		
	*(&vattr) = *(&origvattr);

again:
	error = check_exec_access(p, vp, &vattr);
	if (error)
		goto bad;

	/*
	 * Read in first few bytes of file for segment sizes, magic number:
	 *	407 = plain executable
	 *	410 = RO text
	 *	413 = demand paged RO text
	 * Also an ASCII line beginning with #! is
	 * the file name of a ``shell'' and arguments may be prepended
	 * to the argument list if given here.
	 *
	 * SHELL NAMES ARE LIMITED IN LENGTH.
	 *
	 * ONLY ONE ARGUMENT MAY BE PASSED TO THE SHELL FROM
	 * THE ASCII LINE.
	 */
#if MACH_NBC
	/*
	 * Can't hold an excluisve lock that was returned by namei as 
	 * we depend on pageins to fill the data; there is no need to
	 * hold the lock on LEAF anymore
	 */
	VOP_UNLOCK(vp, 0, p);
#endif /* MACH_NBC */

	exdata.ex_shell[0] = '\0';	/* for zero length files */
#if MACH_NBC
	/* Should not pass with IO_NODELOCKED; but 
	 * the check is ignored in vn_rdwr() if MACH_NBC is on
	 * Also if we  pass with IO_NODELOCKED turned off
	 * vn_rdwr would try to take lock, which is incorrect
	 */

	/*
	 * because exec doesn't go through open and close
	 * we need to map the vnode for proper coherency
	 * and then unmap it to release the kernel map
	 */
	map_vnode(vp, p);
#endif /* MACH_NBC */

	error = vn_rdwr(UIO_READ, vp, (caddr_t)&exdata, sizeof (exdata), 0,
			UIO_SYSSPACE, IO_NODELOCKED, p->p_ucred, &resid, p);
#if MACH_NBC
	unmap_vnode(vp, p);
#endif /* MACH_NBC */

	if (error)
		goto bad;

#ifndef lint
	if (resid > sizeof(exdata) - min(sizeof(exdata.mach_header),
					 sizeof(exdata.fat_header))
	    && exdata.ex_shell[0] != '#') {
		error = ENOEXEC;
		goto bad;
	}
#endif /* lint */
	mach_header = &exdata.mach_header;
	fat_header = &exdata.fat_header;
	if (mach_header->magic == MH_MAGIC)
	    is_fat = FALSE;
	else if (fat_header->magic == FAT_MAGIC ||
		 fat_header->magic == FAT_CIGAM)
	    is_fat = TRUE;
	else if (mach_header->magic == MH_CIGAM) {
	    error = EBADARCH;
	    goto bad;
	} else {
		if (exdata.ex_shell[0] != '#' ||
		    exdata.ex_shell[1] != '!' ||
		    indir) {
			error = ENOEXEC;
			goto bad;
		}
		cp = &exdata.ex_shell[2];		/* skip "#!" */
		while (cp < &exdata.ex_shell[SHSIZE]) {
			if (*cp == '\t')
				*cp = ' ';
			else if (*cp == '\n') {
				*cp = '\0';
				break;
			}
			cp++;
		}
		if (*cp != '\0') {
			error = ENOEXEC;
			goto bad;
		}
		cp = &exdata.ex_shell[2];
		while (*cp == ' ')
			cp++;
		execnamep = cp;
		while (*cp && *cp != ' ')
			cp++;
		cfarg[0] = '\0';
		if (*cp) {
			*cp++ = '\0';
			while (*cp == ' ')
				cp++;
			if (*cp)
				bcopy((caddr_t)cp, (caddr_t)cfarg, SHSIZE);
		}
		indir = 1;
		vput(vp);
		nd.ni_cnd.cn_nameiop = LOOKUP;
		nd.ni_cnd.cn_flags = (nd.ni_cnd.cn_flags & HASBUF) |
						(FOLLOW | LOCKLEAF | SAVENAME);
		nd.ni_segflg = UIO_SYSSPACE;
		nd.ni_dirp = execnamep;
		if ((error = namei(&nd)))
			goto bad1;
		vp = nd.ni_vp;
		VOP_LEASE(vp, p, cred, LEASE_READ);
		if ((error = VOP_GETATTR(vp, &vattr, p->p_ucred, p)))
			goto bad;
		goto again;
	}

	/*
	 * Collect arguments on "file" in swap space.
	 */
	na = 0;
	ne = 0;
	nc = 0;
	cc = 0;
	/* execargs gets allocated at the begining */
	cp = (char *) execargs;	/* running pointer for copy */
	cc = NCARGS;			/* size of execargs */
	/*
	 * Copy arguments into file in argdev area.
	 */
	if (uap->argp) for (;;) {
		ap = NULL;
		sharg = NULL;
		if (indir && na == 0) {
			sharg = nd.ni_cnd.cn_nameptr;
			ap = (int)sharg;
			uap->argp++;		/* ignore argv[0] */
		} else if (indir && (na == 1 && cfarg[0])) {
			sharg = cfarg;
			ap = (int)sharg;
		} else if (indir && (na == 1 || (na == 2 && cfarg[0])))
			ap = (int)uap->fname;
		else if (uap->argp) {
			ap = fuword((caddr_t)uap->argp);
			uap->argp++;
		}
		if (ap == NULL && uap->envp) {
			uap->argp = NULL;
			if ((ap = fuword((caddr_t)uap->envp)) != NULL)
				uap->envp++, ne++;
		}
		if (ap == NULL)
			break;
		na++;
		if (ap == -1) {
			error = EFAULT;
			break;
		}
		do {
			if (nc >= NCARGS-1) {
				error = E2BIG;
				break;
			}
			if (sharg) {
				error = copystr(sharg, cp, (unsigned)cc, &len);
				sharg += len;
			} else {
				error = copyinstr((caddr_t)ap, cp, (unsigned)cc,
				    &len);
				ap += len;
			}
			cp += len;
			nc += len;
			cc -= len;
		} while (error == ENAMETOOLONG);
		if (error) {
			goto bad;
		}
	}
	nc = (nc + NBPW-1) & ~(NBPW-1);

	/*
	 * If we have a fat file, find "our" executable.
	 */
	if (is_fat) {
		/*
		 * Look up our architecture in the fat file.
		 */
		lret = fatfile_getarch(vp, (vm_offset_t)fat_header, &fat_arch);
		if (lret != LOAD_SUCCESS) {
			error = load_return_to_errno(lret);
			goto bad;
		}
		/* Read the Mach-O header out of it */
#if MACH_NBC
		/*
		 * because exec doesn't go through open and close
		 * we need to map the vnode for proper coherency
		 * and then unmap it to release the kernel map
		 */
		map_vnode(vp, p);
#endif /* MACH_NBC */
		error = vn_rdwr(UIO_READ, vp, (caddr_t)&exdata.mach_header,
				sizeof (exdata.mach_header),
				fat_arch.offset,
				UIO_SYSSPACE, (IO_UNIT|IO_NODELOCKED), cred, &resid, p);
#if MACH_NBC
		unmap_vnode(vp, p);
#endif /* MACH_NBC */

		if (error) {
			goto bad;
		}

		/* Did we read a complete header? */
		if (resid) {
			error = EBADEXEC;
			goto bad;
		}

		/* Is what we found a Mach-O executable */
		if (mach_header->magic != MH_MAGIC) {
			error = ENOEXEC;
			goto bad;
		}

		/*
		 *	Load the Mach-O file.
		 */
#if !MACH_NBC
        VOP_UNLOCK(vp, 0, p);
#endif /* !MACH_NBC */
		lret = load_machfile(vp, mach_header, fat_arch.offset,
				    fat_arch.size, &load_result);
	} else {
		/*
		 *	Load the Mach-O file.
		 */
#if !MACH_NBC
		VOP_UNLOCK(vp, 0, p);
#endif /* !MACH_NBC */
		lret = load_machfile(vp, mach_header, 0,
				    (u_long)vattr.va_size, &load_result);
	}

	if (lret != LOAD_SUCCESS) {
		error = load_return_to_errno(lret);
		goto bad;
	}

	/*
	 * deal with set[ug]id.
	 */
	p->p_flag &= ~P_SUGID;
	if (((origvattr.va_mode & VSUID) != 0 &&
	    p->p_ucred->cr_uid != origvattr.va_uid)
	    || (origvattr.va_mode & VSGID) != 0 &&
	    p->p_ucred->cr_gid != origvattr.va_gid) {
		p->p_ucred = crcopy(cred);
#if KTRACE
		/*
		 * If process is being ktraced, turn off - unless
		 * root set it.
		 */
		if (p->p_tracep && !(p->p_traceflag & KTRFAC_ROOT)) {
			vrele(p->p_tracep);
			p->p_tracep = NULL;
			p->p_traceflag = 0;
		}
#endif
		if (origvattr.va_mode & VSUID)
			p->p_ucred->cr_uid = origvattr.va_uid;
		if (origvattr.va_mode & VSGID)
			p->p_ucred->cr_gid = origvattr.va_gid;
		p->p_flag |= P_SUGID;
		/* Radar 2261856; setuid security hole fix */
		/* Patch from OpenBSD: A. Ramesh */
		/*
		 * XXX For setuid processes, attempt to ensure that
		 * stdin, stdout, and stderr are already allocated.
		 * We do not want userland to accidentally allocate
		 * descriptors in this range which has implied meaning
		 * to libc.
		 */
		for (i = 0; i < 3; i++) {
			extern struct fileops vnops;
			struct nameidata nd1;
			struct file *fp;
			int indx;

			if (p->p_fd->fd_ofiles[i] == NULL) {
				if ((error = falloc(p, &fp, &indx)) != 0)
					continue;
				NDINIT(&nd1, LOOKUP, FOLLOW, UIO_SYSSPACE,
				    "/dev/null", p);
				if ((error = vn_open(&nd1, FREAD, 0)) != 0) {
					ffree(fp);
					p->p_fd->fd_ofiles[indx] = NULL;
					break;
				}
				fp->f_flag = FREAD;
				fp->f_type = DTYPE_VNODE;
				fp->f_ops = &vnops;
				fp->f_data = (caddr_t)nd1.ni_vp;
				VOP_UNLOCK(nd1.ni_vp, 0, p);
			}
		}



	}
	p->p_cred->p_svuid = p->p_ucred->cr_uid;
	p->p_cred->p_svgid = p->p_ucred->cr_gid;

	if (p->p_flag & P_TRACED)
		exception_from_kernel(EXC_BREAKPOINT, 0, 0);

	/*
	 *	Temp hack until ld changes... allocate page 0 if its not
	 *	already allocated.
	 *
	 *	FIXME: ld has changed, can we remove this yet? -PK
	 */
	{
		vm_offset_t	addr = 0;
		kern_return_t	ret;

		ret = vm_allocate(current_task()->map, &addr, PAGE_SIZE,
				  FALSE);
		if (ret == KERN_SUCCESS) {
			(void)vm_protect(current_task()->map, 0, PAGE_SIZE,
				FALSE, VM_PROT_NONE);
		}
	}
	if (error) {
	/*
	 *	NOTE: to prevent a race condition, getxfile had
	 *	to temporarily unlock the inode.  If new code needs to
	 *	be inserted here before the iput below, and it needs
	 *	to deal with the inode, keep this in mind.
	 */
		goto bad;
	}
	VOP_LOCK(vp,  LK_EXCLUSIVE | LK_RETRY, p);
	vput(vp);
	vp = NULL;
	
	if (load_result.unixproc &&
		create_unix_stack(current_task()->map,
					load_result.user_stack, p)) {
		error = load_return_to_errno(LOAD_NOSPACE);
		goto bad;
	}

	/*
	 * Copy back arglist if necessary.
	 */

	ucp = p->user_stack;
	if (load_result.unixproc) {
#if	STACK_GROWTH_UP
		/* for stacks that grow up we:
		 *	place storage for the ps_strings stuff
		 *	put a pointer to the end of the string area at
		 *	the beginning of the stack (used by table()),
		 *	put a pointer to the arg area where we start the
		 *	process stack pointer.
		 */
		ap = ucp + NBPW;		// leave word for string ptr
		(void) suword((caddr_t)ap, load_result.mach_header); // dyld
		ap += NBPW;
		ucp = ap + na*NBPW + 3*NBPW;
		(void) suword((caddr_t)(ap - 2 * NBPW), ucp + nc);
		uthread->uu_ar0[SP] = ucp + nc;
		(void) suword((caddr_t)uthread->uu_ar0[SP], 0);
		uthread->uu_ar0[SP] += NBPW;
		(void) suword((caddr_t)uthread->uu_ar0[SP], ap);
#else	STACK_GROWTH_UP
		ucp = ucp - nc - NBPW;
		ap = ucp - na*NBPW - 3*NBPW;
		uthread->uu_ar0[SP] = ap;
#endif	/* STACK_GROWTH_UP */
		(void) suword((caddr_t)ap, na-ne);
		nc = 0;
		cc = 0;
		cp = (char *) execargs;
		cc = NCARGS;
		ps.ps_argvstr = (char *)ucp;	/* first argv string */
		ps.ps_nargvstr = na - ne;		/* argc */
		for (;;) {
			ap += NBPW;
			if (na == ne) {
				(void) suword((caddr_t)ap, 0);
				ap += NBPW;
				ps.ps_envstr = (char *)ucp;
				ps.ps_nenvstr = ne;
			}
			if (--na < 0)
				break;
			(void) suword((caddr_t)ap, ucp);
			do {
				error = copyoutstr(cp, (caddr_t)ucp,
						   (unsigned)cc, &len);
				ucp += len;
				cp += len;
				nc += len;
				cc -= len;
			} while (error == ENOENT);
			if (error == EFAULT)
				break;	/* bad stack - user's problem */
		}
		(void) suword((caddr_t)ap, 0);
//		(void) copyout((caddr_t)&ps, (caddr_t)PS_STRINGS, sizeof(ps));
	}
	
#if STACK_GROWTH_UP
#else STACK_GROWTH_UP
	if (load_result.dynlinker) {
		ap = uthread->uu_ar0[SP] -= 4;
		(void) suword((caddr_t)ap, load_result.mach_header);
	}
#endif /* STACK_GROWTH_UP */

/*
 * This is GROSS XXX FIXME
 */
#if defined(i386) || defined(ppc)
 	uthread->uu_ar0[PC] = load_result.entry_point;
#else
#error architecture not implemented!
#endif	

	/*
	 * Reset signal state.
	 */
	execsigs(p);

	/*
	 * Close file descriptors
	 * which specify close-on-exec.
	 */
	fdexec(p);

	/*
	 * Remember file name for accounting.
	 */
	p->p_acflag &= ~AFORK;
	if (nd.ni_cnd.cn_namelen > MAXCOMLEN)
		nd.ni_cnd.cn_namelen = MAXCOMLEN;
	bcopy((caddr_t)nd.ni_cnd.cn_nameptr, (caddr_t)p->p_comm,
	    (unsigned)nd.ni_cnd.cn_namelen);
	p->p_comm[nd.ni_cnd.cn_namelen] = '\0';

	/*
	 * mark as execed, wakeup the process that vforked (if any) and tell
	 * it that it now has it's own resources back
	 */
	p->p_flag |= P_EXEC;
	if (p->p_pptr && (p->p_flag & P_PPWAIT)) {
		p->p_flag &= ~P_PPWAIT;
		wakeup((caddr_t)p->p_pptr);
	}

bad:
	FREE_ZONE(nd.ni_cnd.cn_pnbuf, nd.ni_cnd.cn_pnlen, M_NAMEI);
	if (vp)
		vput(vp);
bad1:
	if (execargs)
		kmem_free_wakeup(kernel_pageable_map, execargs, NCARGS);
	return(error);
}


#define	unix_stack_size(p)	(p->p_rlimit[RLIMIT_STACK].rlim_cur)

kern_return_t create_unix_stack(map, user_stack, p)
	vm_map_t	map;
	vm_offset_t	user_stack;
	struct proc	*p;
{
	vm_size_t	size;
	vm_offset_t	addr;

	p->user_stack = user_stack;
	size = round_page(unix_stack_size(p));
#if	STACK_GROWTH_UP
	// stack always points to first address for stacks
	addr = user_stack;
#else	STACK_GROWTH_UP
	addr = trunc_page(user_stack - size);
#endif	/* STACK_GROWTH_UP */
	return(vm_map_find(map, VM_OBJECT_NULL, (vm_offset_t) 0,
			&addr, size, FALSE));
}

#include <sys/reboot.h>

char		init_program_name[128] = "/sbin/mach_init\0";

char		init_args[128] = "-D\0";

struct execve_args	init_exec_args;
int		init_attempts = 0;


void load_init_program(p)
	struct proc *p;
{
	vm_offset_t	init_addr;
	int		*old_ap;
	char		*argv[3];
	int		error;
	register_t retval[2];


	unix_master();

	error = 0;

	/* init_args are copied in string form directly from bootstrap */
	
	do {
		if (boothowto & RB_INITNAME) {
			printf("init program? ");
			gets(init_program_name, init_program_name);
		}

		if (error && ((boothowto & RB_INITNAME) == 0) &&
					(init_attempts == 1)) {
			static char other_init[] = "/etc/mach_init";
			printf("Load of %s, errno %d, trying %s\n",
				init_program_name, error, other_init);
			error = 0;
			bcopy(other_init, init_program_name,
							sizeof(other_init));
		}

		init_attempts++;

		if (error) {
			printf("Load of %s failed, errno %d\n",
					init_program_name, error);
			error = 0;
			boothowto |= RB_INITNAME;
			continue;
		}

		/*
		 *	Copy out program name.
		 */

		init_addr = VM_MIN_ADDRESS;
		(void) vm_allocate(current_task()->map, &init_addr,
				PAGE_SIZE, TRUE);
		if (init_addr == 0)
			init_addr++;
		(void) copyout((caddr_t) init_program_name,
				(caddr_t) (init_addr),
				(unsigned) sizeof(init_program_name)+1);

		argv[0] = (char *) init_addr;
		init_addr += sizeof(init_program_name);
		init_addr = (vm_offset_t)ROUND_PTR(char, init_addr);

		/*
		 *	Put out first (and only) argument, similarly.
		 *	Assumes everything fits in a page as allocated
		 *	above.
		 */

		(void) copyout((caddr_t) init_args,
				(caddr_t) (init_addr),
				(unsigned) sizeof(init_args));

		argv[1] = (char *) init_addr;
		init_addr += sizeof(init_args);
		init_addr = (vm_offset_t)ROUND_PTR(char, init_addr);

		/*
		 *	Null-end the argument list
		 */

		argv[2] = (char *) 0;
		
		/*
		 *	Copy out the argument list.
		 */
		
		(void) copyout((caddr_t) argv,
				(caddr_t) (init_addr),
				(unsigned) sizeof(argv));

		/*
		 *	Set up argument block for fake call to execve.
		 */

		init_exec_args.fname = argv[0];
		init_exec_args.argp = (char **) init_addr;
		init_exec_args.envp = 0;

		old_ap = current_thread()->_uthread->uu_ap;
		current_thread()->_uthread->uu_ap = (int *) &init_exec_args;
		error = execve(p,&init_exec_args,retval);
		current_thread()->_uthread->uu_ap = old_ap;
	} while (error);

	unix_release();
}

/*
 * Convert a load_return_t to an errno.
 */
static int load_return_to_errno(load_return_t lrtn)
{
	switch (lrtn) {
	    case LOAD_SUCCESS:
		return 0;
	    case LOAD_BADARCH:
	    	return EBADARCH;
	    case LOAD_BADMACHO:
	    	return EBADMACHO;
	    case LOAD_SHLIB:
	    	return ESHLIBVERS;
	    case LOAD_NOSPACE:
	    	return ENOMEM;
	    case LOAD_PROTECT:
	    	return EACCES;
	    case LOAD_RESOURCE:
	    case LOAD_FAILURE:
	    default:
	    	return EBADEXEC;
	}
}

/*
 * exec_check_access()
 */
int
check_exec_access(p, vp, vap)
	struct proc  *p;
	struct vnode *vp;
	struct vattr *vap;
{
	int flag;
	int error;

	if (error = VOP_ACCESS(vp, VEXEC, p->p_ucred, p))
		return (error);
	flag = p->p_flag;
	if (flag & P_TRACED) {
		if (error = VOP_ACCESS(vp, VREAD, p->p_ucred, p))
			return (error);
	}
	if (vp->v_type != VREG ||
	    (vap->va_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) == 0)
		return (EACCES);
	return (0);
}

