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
 * Copyright (c) 1997 Apple Computer, Inc.  All rights reserved.
 * Copyright (c) 1994 NeXT Computer, Inc.  All rights reserved.
 *
 * machdep/ppc/miniMonMachdep.c
 *
 * Machine-dependent code for Remote Debugging Protocol
 *
 * March, 1997	Created.	Umesh Vaishampayan [umeshv@NeXT.com]
 *
 */

#include <sys/reboot.h>
#include <kern/miniMonPrivate.h>


static boolean_t ishex(char c)
{
    if ((c >= '0' && c <= '9') ||
        (c >= 'a' && c <= 'f') ||
	(c >= 'A' && c <= 'F'))
	    return TRUE;
    return FALSE;
}

static int xx(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return 0;
}

static unsigned int
xtoi(char *str)
{
    unsigned int num = 0;

    while (*str && ishex(*str)) {
        num = (16 * num) + xx(*str);
        str++;
    }
    return num;
}

static boolean_t
miniMonDump(char *line)
{
    int addr, count;
    static unsigned int *ptr = 0x10000;
    static unsigned int old_count = 8;
    int text;

    text = ((*line) == 't');

    /* Skip command name */
    while (*line && (*line != ' ' && *line != '\t'))
	line++;
    /* Skip whitespace */
    while (*line && (*line == ' ' || *line == '\t'))
	line++;
    addr = xtoi(line);
    if (addr != 0)
	ptr = (unsigned int *)addr;
    /* Skip address */
    while (*line && (*line != ' ' && *line != '\t'))
	line++;
    /* Skip whitespace */
    while (*line && (*line == ' ' || *line == '\t'))
	line++;
    if (*line == 's') {
	safe_prf(ptr);
	return TRUE;
    }
    count = xtoi(line);
    if (count == 0)
	count = old_count;
    else
	old_count = count;
    if (count > 1024) count = 1024;
    safe_prf("%08x : ", ptr);
    while (count > 0)
	if( text) {
	   miniMonPutchar(*((char *)ptr)++);
	   count--;
	} else {
	   safe_prf("%08x ",*ptr++);
	   count -= 4;
	}
    safe_prf("\n");
    return TRUE;
}

miniMonCommand_t miniMonMDCommands[] =
{
    {"dump", miniMonDump, "Dump memory"},
    {"text", miniMonDump, "Dump text"},
    {0,0,0}
};


/*
 * Reboot without sync.
 */
boolean_t
miniMonReboot(char *line)
{
  boot(RB_BOOT, RB_AUTOBOOT|RB_NOSYNC, "");
  return FALSE;
}

/*
 * Halt with sync.
 */
boolean_t
miniMonHalt(char *line)
{
  reboot_mach(RB_HALT);
  return FALSE;
}

boolean_t
miniMonGdb(char *line)
{
    call_kdp();
  return FALSE;
}

int
miniMonTryGetchar(void)
{
  return kmtrygetc();
}

int
miniMonGetchar(void)
{
  register int c;
  do {
    c = kmtrygetc();
  } while (c == NO_CHAR);
  switch (c) {
  case '\177':
    c = '\b';
    break;
  case '\r':
    kmputc(0,'\r');
    c = '\n';
    break;
  case 'u'&037:
    c = '\025';
    break;
  default:
    break;
  }
  kmputc(0,c);
  return c;
}

void
miniMonPutchar(int ch)
{
    kmputc(0,ch);
};

