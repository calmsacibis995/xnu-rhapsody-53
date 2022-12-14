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

/*-
 * Copyright (c) 1992, 1993
 *	The Regents of the University of California.  All rights reserved.
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
 *	@(#)libkern.h	8.1 (Berkeley) 6/10/93
 */

#ifndef _LIBKERN_LIBKERN_H_
#define _LIBKERN_LIBKERN_H_

#include <sys/types.h>

static inline int
imax(a, b)
	int a, b;
{
	return (a > b ? a : b);
}
static inline int
imin(a, b)
	int a, b;
{
	return (a < b ? a : b);
}
static inline long
lmax(a, b)
	long a, b;
{
	return (a > b ? a : b);
}
static inline long
lmin(a, b)
	long a, b;
{
	return (a < b ? a : b);
}
static inline u_int
max(a, b)
	u_int a, b;
{
	return (a > b ? a : b);
}
static inline u_int
min(a, b)
	u_int a, b;
{
	return (a < b ? a : b);
}
static inline u_long
ulmax(a, b)
	u_long a, b;
{
	return (a > b ? a : b);
}
static inline u_long
ulmin(a, b)
	u_long a, b;
{
	return (a < b ? a : b);
}

/* Prototypes for non-quad routines. */

int		bcmp __P((const void *, const void *, size_t));

int		ffs __P((int));
int		locc __P((int, char *, u_int));
u_long	random __P((void));
int		scanc __P((u_int, u_char *, u_char *, int));
int		skpc __P((int, int, char *));

char	*rindex __P((const char *, int));
char	*strcat __P((char *, const char *));
char	*strncat __P((char *, const char *, size_t));
char	*strchr __P((const char *, int));
char	*strrchr __P((const char *, int));
int		strcmp __P((const char *, const char *));
int		strncmp __P((const char *, const char *, size_t));
char	*strcpy __P((char *, const char *));
char	*strncpy __P((char *, const char *, size_t));
size_t	strlen __P((const char *));
char	*strstr __P((const char *, const char *));
long	strtol __P((const char*, char **, int));
u_long	strtoul __P((const char *, char **, int));

void	*memchr __P((const void *, int, size_t));
void	*memcpy __P((void *, const void *, size_t));
void	*memmove __P((void *, const void *, size_t));
int		memcmp __P((const void *, const void *, size_t));
void	*memset __P((void *, int, size_t));

#endif /* _LIBKERN_LIBKERN_H_ */
