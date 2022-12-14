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

/* Copyright (c) 1992 NeXT Computer, Inc.  All rights reserverd
 *
 * PCKeymap.c - Declaration of the PC's default keymap string
 *
 * HISTORY
 * 07 Dec 1992	Joe Pasqua
 *	Created  true PC version.
 */

// NOTE:
// The following keys are multi-byte sequences on the PC keyboard. They get
// mapped into a single scan code for our purposes. Here is the mapping:
//	PC Key		PC Code		NeXT Code
//	Right-Ctrl	E0-1D		0x60
//	Right-Alt	E0-38		0x61
//	Keypad-Enter	E0-1C		0x62
//	Keypad-/	E0-35		0x63
//	Up-Arrow	E0-48		0x64
//	Down-Arrow	E0-50		0x65
//	Left-Arrow	E0-4B		0x66
//	Right-Arrow	E0-4D		0x67
//	Insert		E0-52		0x68
//	Delete		E0-53		0x69
//	Page Up		E0-49		0x6A
//	Page Down	E0-51		0x6B
//	Home		E0-47		0x6C
//	End		E0-4F		0x6D

#import	<bsd/dev/event.h>
#import	<bsd/dev/ev_keymap.h>

#define	CTRL(c)	((c)&037)

//
// NOTE:	Because there is no Command key on the PC keyboard, we are
//		splitting the ALT keys up. We'll use Left-Alt as Command, and
//		Right-Alt as ALT.
//
static const unsigned char PCDefaultKeymap[] = {
0x00, 0x00,     /* char file format */
6,              /* Modifier key definitions */
        0x01, 0x02,     0x2A, 0x36,     /* Shift, 2 keys */
        0x02, 0x02,     0x1D, 0x60,	/* Ctrl, 2 keys */
        0x03, 0x01,     0x61,		/* Alt, 1 key */
        0x04, 0x01,     0x38,		/* Cmd, 1 key */
        0x05, 0x15,     0x52, 0x53, 0x62, 0x4F, 0x50, 0x51, 0x4B, 0x4C, 0x4D,
                        0x4E, 0x47, 0x48, 0x49, 0x45, 0x63, 0x37, 0x4A,
                        0x64, 0x65, 0x66, 0x67,   /* NumPad, 21 key(s) */
	0x06, 0x01,	0x3B,	/* The help key	*/
104,             /* Key Definitions */
        0xff,   /* Key 0x00 unassigned */
        /* Key 0x01 modifier key mask bits (0x02) */
        (1<<NX_MODIFIERKEY_SHIFT),
                NX_ASCIISET,    CTRL('['),      /* no flags */
                NX_ASCIISET,         '~',       /* Shift */
        /* Key 0x02 modifier key mask bits (0x0a) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '1',       /* no flags */
                NX_ASCIISET,         '!',       /* Shift */
                NX_SYMBOLSET,       0xad,       /* Alt */
                NX_ASCIISET,        0xa1,       /* Shift Alt */
        /* Key 0x03 modifier key mask bits (0x0e) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '2',       /* no flags */
                NX_ASCIISET,         '@',       /* Shift */
                NX_ASCIISET,    CTRL('@'),      /* Ctrl */
                NX_ASCIISET,    CTRL('@'),      /* Shift Ctrl */
                NX_ASCIISET,        0xb2,       /* Alt */
                NX_ASCIISET,        0xb3,       /* Shift Alt */
                NX_ASCIISET,    CTRL('@'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('@'),      /* Shift Ctrl Alt */
        /* Key 0x04 modifier key mask bits (0x0a) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '3',       /* no flags */
                NX_ASCIISET,         '#',       /* Shift */
                NX_ASCIISET,        0xa3,       /* Alt */
                NX_SYMBOLSET,       0xba,       /* Shift Alt */
        /* Key 0x05 modifier key mask bits (0x0a) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '4',       /* no flags */
                NX_ASCIISET,         '$',       /* Shift */
                NX_ASCIISET,        0xa2,       /* Alt */
                NX_ASCIISET,        0xa8,       /* Shift Alt */
        /* Key 0x06 modifier key mask bits (0x0a) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '5',       /* no flags */
                NX_ASCIISET,         '%',       /* Shift */
                NX_SYMBOLSET,       0xa5,       /* Alt */
                NX_ASCIISET,        0xbd,       /* Shift Alt */
        /* Key 0x07 modifier key mask bits (0x0e) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '6',       /* no flags */
                NX_ASCIISET,         '^',       /* Shift */
                NX_ASCIISET,    CTRL('^'),      /* Ctrl */
                NX_ASCIISET,    CTRL('^'),      /* Shift Ctrl */
                NX_ASCIISET,        0xb6,       /* Alt */
                NX_ASCIISET,        0xc3,       /* Shift Alt */
                NX_ASCIISET,    CTRL('^'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('^'),      /* Shift Ctrl Alt */
        /* Key 0x08 modifier key mask bits (0x0a) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '7',       /* no flags */
                NX_ASCIISET,         '&',       /* Shift */
                NX_ASCIISET,        0xb7,       /* Alt */
                NX_SYMBOLSET,       0xab,       /* Shift Alt */
        /* Key 0x09 modifier key mask bits (0x0a) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '8',       /* no flags */
                NX_ASCIISET,         '*',       /* Shift */
                NX_SYMBOLSET,       0xb0,       /* Alt */
                NX_ASCIISET,        0xb4,       /* Shift Alt */
        /* Key 0x0A modifier key mask bits (0x0a) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '9',       /* no flags */
                NX_ASCIISET,         '(',       /* Shift */
                NX_ASCIISET,        0xac,       /* Alt */
                NX_ASCIISET,        0xab,       /* Shift Alt */
        /* Key 0x0B modifier key mask bits (0x0a) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '0',       /* no flags */
                NX_ASCIISET,         ')',       /* Shift */
                NX_ASCIISET,        0xad,       /* Alt */
                NX_ASCIISET,        0xbb,       /* Shift Alt */
        /* Key 0x0C modifier key mask bits (0x0e) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '-',       /* no flags */
                NX_ASCIISET,         '_',       /* Shift */
                NX_ASCIISET,    CTRL('_'),      /* Ctrl */
                NX_ASCIISET,    CTRL('_'),      /* Shift Ctrl */
                NX_ASCIISET,        0xb1,       /* Alt */
                NX_ASCIISET,        0xd0,       /* Shift Alt */
                NX_ASCIISET,    CTRL('_'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('_'),      /* Shift Ctrl Alt */
        /* Key 0x0D modifier key mask bits (0x0a) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '=',       /* no flags */
                NX_ASCIISET,         '+',       /* Shift */
                NX_SYMBOLSET,       0xb9,       /* Alt */
                NX_SYMBOLSET,       0xb1,       /* Shift Alt */
        /* Key 0x0E modifier key mask bits (0x02) */
        (1<<NX_MODIFIERKEY_SHIFT),
                NX_ASCIISET,        0x7f,       /* no flags */
                NX_ASCIISET,        '\b',       /* Shift */
        /* Key 0x0f modifier key mask bits (0x02) */
        (1<<NX_MODIFIERKEY_SHIFT),
                NX_ASCIISET,        '\t',       /* no flags */
                NX_ASCIISET,    CTRL('Y'),      /* Shift */
        /* Key 0x10 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'q',       /* no flags */
                NX_ASCIISET,         'Q',       /* AlphaShift */
                NX_ASCIISET,    CTRL('Q'),      /* Ctrl */
                NX_ASCIISET,    CTRL('Q'),      /* AlphaShift Ctrl */
                NX_ASCIISET,        0xfa,       /* Alt */
                NX_ASCIISET,        0xea,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('Q'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('Q'),      /* AlphaShift Ctrl Alt */
        /* Key 0x11 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'w',       /* no flags */
                NX_ASCIISET,         'W',       /* AlphaShift */
                NX_ASCIISET,    CTRL('W'),      /* Ctrl */
                NX_ASCIISET,    CTRL('W'),      /* AlphaShift Ctrl */
                NX_SYMBOLSET,       0xc8,       /* Alt */
                NX_SYMBOLSET,       0xc7,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('W'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('W'),      /* AlphaShift Ctrl Alt */
        /* Key 0x12 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'e',       /* no flags */
                NX_ASCIISET,         'E',       /* AlphaShift */
                NX_ASCIISET,    CTRL('E'),      /* Ctrl */
                NX_ASCIISET,    CTRL('E'),      /* AlphaShift Ctrl */
                NX_ASCIISET,        0xc2,       /* Alt */
                NX_ASCIISET,        0xc5,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('E'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('E'),      /* AlphaShift Ctrl Alt */
        /* Key 0x13 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'r',       /* no flags */
                NX_ASCIISET,         'R',       /* AlphaShift */
                NX_ASCIISET,    CTRL('R'),      /* Ctrl */
                NX_ASCIISET,    CTRL('R'),      /* AlphaShift Ctrl */
                NX_SYMBOLSET,       0xe2,       /* Alt */
                NX_SYMBOLSET,       0xd2,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('R'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('R'),      /* AlphaShift Ctrl Alt */
        /* Key 0x14 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         't',       /* no flags */
                NX_ASCIISET,         'T',       /* AlphaShift */
                NX_ASCIISET,    CTRL('T'),      /* Ctrl */
                NX_ASCIISET,    CTRL('T'),      /* AlphaShift Ctrl */
                NX_SYMBOLSET,       0xe4,       /* Alt */
                NX_SYMBOLSET,       0xd4,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('T'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('T'),      /* AlphaShift Ctrl Alt */
        /* Key 0x15 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'y',       /* no flags */
                NX_ASCIISET,         'Y',       /* AlphaShift */
                NX_ASCIISET,    CTRL('Y'),      /* Ctrl */
                NX_ASCIISET,    CTRL('Y'),      /* AlphaShift Ctrl */
                NX_ASCIISET,        0xa5,       /* Alt */
                NX_SYMBOLSET,       0xdb,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('Y'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('Y'),      /* AlphaShift Ctrl Alt */
        /* Key 0x16 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'u',       /* no flags */
                NX_ASCIISET,         'U',       /* AlphaShift */
                NX_ASCIISET,    CTRL('U'),      /* Ctrl */
                NX_ASCIISET,    CTRL('U'),      /* AlphaShift Ctrl */
                NX_ASCIISET,        0xc8,       /* Alt */
                NX_ASCIISET,        0xcd,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('U'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('U'),      /* AlphaShift Ctrl Alt */
        /* Key 0x17 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'i',       /* no flags */
                NX_ASCIISET,         'I',       /* AlphaShift */
                NX_ASCIISET,        '\t',       /* Ctrl */
                NX_ASCIISET,        '\t',       /* AlphaShift Ctrl */
                NX_ASCIISET,        0xc1,       /* Alt */
                NX_ASCIISET,        0xf5,       /* AlphaShift Alt */
                NX_ASCIISET,        '\t',       /* Ctrl Alt */
                NX_ASCIISET,        '\t',       /* AlphaShift Ctrl Alt */
        /* Key 0x18 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'o',       /* no flags */
                NX_ASCIISET,         'O',       /* AlphaShift */
                NX_ASCIISET,    CTRL('O'),      /* Ctrl */
                NX_ASCIISET,    CTRL('O'),      /* AlphaShift Ctrl */
                NX_ASCIISET,        0xf9,       /* Alt */
                NX_ASCIISET,        0xe9,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('O'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('O'),      /* AlphaShift Ctrl Alt */
        /* Key 0x19 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'p',       /* no flags */
                NX_ASCIISET,         'P',       /* AlphaShift */
                NX_ASCIISET,    CTRL('P'),      /* Ctrl */
                NX_ASCIISET,    CTRL('P'),      /* AlphaShift Ctrl */
                NX_SYMBOLSET,       0x70,       /* Alt */
                NX_SYMBOLSET,       0x50,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('P'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('P'),      /* AlphaShift Ctrl Alt */
        /* Key 0x1A modifier key mask bits (0x0e) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '[',       /* no flags */
                NX_ASCIISET,         '{',       /* Shift */
                NX_ASCIISET,    CTRL('['),      /* Ctrl */
                NX_ASCIISET,    CTRL('['),      /* Shift Ctrl */
                NX_ASCIISET,         '`',       /* Alt */
                NX_ASCIISET,        0xaa,       /* Shift Alt */
                NX_ASCIISET,    CTRL('['),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('['),      /* Shift Ctrl Alt */
        /* Key 0x1B modifier key mask bits (0x0e) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         ']',       /* no flags */
                NX_ASCIISET,         '}',       /* Shift */
                NX_ASCIISET,    CTRL(']'),      /* Ctrl */
                NX_ASCIISET,    CTRL(']'),      /* Shift Ctrl */
                NX_ASCIISET,        '\'',       /* Alt */
                NX_ASCIISET,        0xba,       /* Shift Alt */
                NX_ASCIISET,    CTRL(']'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL(']'),      /* Shift Ctrl Alt */
        /* Key 0x1C modifier key mask bits (0x10) */
        (1<<NX_MODIFIERKEY_COMMAND),
                NX_ASCIISET,        '\r',       /* no flags */
                NX_ASCIISET,    CTRL('C'),      /* Cmd */
        0xff,   /* Key 0x1D unassigned - Left Control */
        /* Key 0x1E modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'a',       /* no flags */
                NX_ASCIISET,         'A',       /* AlphaShift */
                NX_ASCIISET,    CTRL('A'),      /* Ctrl */
                NX_ASCIISET,    CTRL('A'),      /* AlphaShift Ctrl */
                NX_ASCIISET,        0xca,       /* Alt */
                NX_ASCIISET,        0xc7,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('A'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('A'),      /* AlphaShift Ctrl Alt */
        /* Key 0x1F modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         's',       /* no flags */
                NX_ASCIISET,         'S',       /* AlphaShift */
                NX_ASCIISET,    CTRL('S'),      /* Ctrl */
                NX_ASCIISET,    CTRL('S'),      /* AlphaShift Ctrl */
                NX_ASCIISET,        0xfb,       /* Alt */
                NX_ASCIISET,        0xa7,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('S'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('S'),      /* AlphaShift Ctrl Alt */
        /* Key 0x20 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'd',       /* no flags */
                NX_ASCIISET,         'D',       /* AlphaShift */
                NX_ASCIISET,    CTRL('D'),      /* Ctrl */
                NX_ASCIISET,    CTRL('D'),      /* AlphaShift Ctrl */
                NX_SYMBOLSET,       0x44,       /* Alt */
                NX_SYMBOLSET,       0xb6,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('D'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('D'),      /* AlphaShift Ctrl Alt */
        /* Key 0x21 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'f',       /* no flags */
                NX_ASCIISET,         'F',       /* AlphaShift */
                NX_ASCIISET,    CTRL('F'),      /* Ctrl */
                NX_ASCIISET,    CTRL('F'),      /* AlphaShift Ctrl */
                NX_ASCIISET,        0xa6,       /* Alt */
                NX_SYMBOLSET,       0xac,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('F'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('F'),      /* AlphaShift Ctrl Alt */
        /* Key 0x22 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'g',       /* no flags */
                NX_ASCIISET,         'G',       /* AlphaShift */
                NX_ASCIISET,    CTRL('G'),      /* Ctrl */
                NX_ASCIISET,    CTRL('G'),      /* AlphaShift Ctrl */
                NX_ASCIISET,        0xf1,       /* Alt */
                NX_ASCIISET,        0xe1,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('G'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('G'),      /* AlphaShift Ctrl Alt */
        /* Key 0x23 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'h',       /* no flags */
                NX_ASCIISET,         'H',       /* AlphaShift */
                NX_ASCIISET,        '\b',       /* Ctrl */
                NX_ASCIISET,        '\b',       /* AlphaShift Ctrl */
                NX_ASCIISET,        0xe3,       /* Alt */
                NX_ASCIISET,        0xeb,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('@'),      /* Ctrl Alt */
                0x18,   CTRL('@'),      /* AlphaShift Ctrl Alt */
        /* Key 0x24 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'j',       /* no flags */
                NX_ASCIISET,         'J',       /* AlphaShift */
                NX_ASCIISET,        '\n',       /* Ctrl */
                NX_ASCIISET,        '\n',       /* AlphaShift Ctrl */
                NX_ASCIISET,        0xc6,       /* Alt */
                NX_ASCIISET,        0xae,       /* AlphaShift Alt */
                NX_ASCIISET,        '\n',       /* Ctrl Alt */
                NX_ASCIISET,        '\n',       /* AlphaShift Ctrl Alt */
        /* Key 0x25 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'k',       /* no flags */
                NX_ASCIISET,         'K',       /* AlphaShift */
                NX_ASCIISET,    CTRL('K'),      /* Ctrl */
                NX_ASCIISET,    CTRL('K'),      /* AlphaShift Ctrl */
                NX_ASCIISET,        0xce,       /* Alt */
                NX_ASCIISET,        0xaf,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('K'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('K'),      /* AlphaShift Ctrl Alt */
        /* Key 0x26 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'l',       /* no flags */
                NX_ASCIISET,         'L',       /* AlphaShift */
                NX_ASCIISET,        '\f',       /* Ctrl */
                NX_ASCIISET,        '\f',       /* AlphaShift Ctrl */
                NX_ASCIISET,        0xf8,       /* Alt */
                NX_ASCIISET,        0xe8,       /* AlphaShift Alt */
                NX_ASCIISET,        '\f',       /* Ctrl Alt */
                NX_ASCIISET,        '\f',       /* AlphaShift Ctrl Alt */
        /* Key 0x27 modifier key mask bits (0x0a) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         ';',       /* no flags */
                NX_ASCIISET,         ':',       /* Shift */
                NX_SYMBOLSET,       0xb2,       /* Alt */
                NX_SYMBOLSET,       0xa2,       /* Shift Alt */
        /* Key 0x28 modifier key mask bits (0x0a) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,        '\'',       /* no flags */
                NX_ASCIISET,         '"',       /* Shift */
                NX_ASCIISET,        0xa9,       /* Alt */
                NX_SYMBOLSET,       0xae,       /* Shift Alt */
        /* Key 0x29 modifier key mask bits (0x02) */
        (1<<NX_MODIFIERKEY_SHIFT),
                NX_ASCIISET,         '`',	/* no flags */
                NX_ASCIISET,         '~',       /* Shift */
        0xff,   /* Key 0x2A unassigned - Left Shift */
        /* Key 0x2B modifier key mask bits (0x0e) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,        '\\',       /* no flags */
                NX_ASCIISET,         '|',       /* Shift */
                NX_ASCIISET,    CTRL('\\'),     /* Ctrl */
                NX_ASCIISET,    CTRL('\\'),     /* Shift Ctrl */
                NX_ASCIISET,        0xe3,       /* Alt */
                NX_ASCIISET,        0xeb,       /* Shift Alt */
                NX_ASCIISET,    CTRL('\\'),     /* Ctrl Alt */
                NX_ASCIISET,    CTRL('\\'),     /* Shift Ctrl Alt */
        /* Key 0x2C modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'z',       /* no flags */
                NX_ASCIISET,         'Z',       /* AlphaShift */
                NX_ASCIISET,    CTRL('Z'),      /* Ctrl */
                NX_ASCIISET,    CTRL('Z'),      /* AlphaShift Ctrl */
                NX_ASCIISET,        0xcf,       /* Alt */
                NX_SYMBOLSET,       0x57,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('Z'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('Z'),      /* AlphaShift Ctrl Alt */
        /* Key 0x2D modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'x',       /* no flags */
                NX_ASCIISET,         'X',       /* AlphaShift */
                NX_ASCIISET,    CTRL('X'),      /* Ctrl */
                NX_ASCIISET,    CTRL('X'),      /* AlphaShift Ctrl */
                NX_SYMBOLSET,       0xb4,       /* Alt */
                NX_SYMBOLSET,       0xce,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('X'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('X'),      /* AlphaShift Ctrl Alt */
        /* Key 0x2E modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'c',       /* no flags */
                NX_ASCIISET,         'C',       /* AlphaShift */
                NX_ASCIISET,    CTRL('C'),      /* Ctrl */
                NX_ASCIISET,    CTRL('C'),      /* AlphaShift Ctrl */
                NX_SYMBOLSET,       0xe3,       /* Alt */
                NX_SYMBOLSET,       0xd3,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('C'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('C'),      /* AlphaShift Ctrl Alt */
        /* Key 0x2F modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'v',       /* no flags */
                NX_ASCIISET,         'V',       /* AlphaShift */
                NX_ASCIISET,    CTRL('V'),      /* Ctrl */
                NX_ASCIISET,    CTRL('V'),      /* AlphaShift Ctrl */
                NX_SYMBOLSET,       0xd6,       /* Alt */
                NX_SYMBOLSET,       0xe0,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('V'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('V'),      /* AlphaShift Ctrl Alt */
        /* Key 0x30 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'b',       /* no flags */
                NX_ASCIISET,         'B',       /* AlphaShift */
                NX_ASCIISET,    CTRL('B'),      /* Ctrl */
                NX_ASCIISET,    CTRL('B'),      /* AlphaShift Ctrl */
                NX_SYMBOLSET,       0xe5,       /* Alt */
                NX_SYMBOLSET,       0xf2,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('B'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('B'),      /* AlphaShift Ctrl Alt */
        /* Key 0x31 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'n',       /* no flags */
                NX_ASCIISET,         'N',       /* AlphaShift */
                NX_ASCIISET,    CTRL('N'),      /* Ctrl */
                NX_ASCIISET,    CTRL('N'),      /* AlphaShift Ctrl */
                NX_ASCIISET,        0xc4,       /* Alt */
                NX_SYMBOLSET,       0xaf,       /* AlphaShift Alt */
                NX_ASCIISET,    CTRL('N'),      /* Ctrl Alt */
                NX_ASCIISET,    CTRL('N'),      /* AlphaShift Ctrl Alt */
        /* Key 0x32 modifier key mask bits (0x0d) */
        (1<<NX_MODIFIERKEY_ALPHALOCK)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         'm',       /* no flags */
                NX_ASCIISET,         'M',       /* AlphaShift */
                NX_ASCIISET,        '\r',       /* Ctrl */
                NX_ASCIISET,        '\r',       /* AlphaShift Ctrl */
                NX_SYMBOLSET,       0x6d,       /* Alt */
                NX_SYMBOLSET,       0xd8,       /* AlphaShift Alt */
                NX_ASCIISET,        '\r',       /* Ctrl Alt */
                NX_ASCIISET,        '\r',       /* AlphaShift Ctrl Alt */
        /* Key 0x33 modifier key mask bits (0x0a) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         ',',       /* no flags */
                NX_ASCIISET,         '<',       /* Shift */
                NX_ASCIISET,        0xcb,       /* Alt */
                NX_SYMBOLSET,       0xa3,       /* Shift Alt */
        /* Key 0x34 modifier key mask bits (0x0a) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '.',       /* no flags */
                NX_ASCIISET,         '>',       /* Shift */
                NX_ASCIISET,        0xbc,       /* Alt */
                NX_SYMBOLSET,       0xb3,       /* Shift Alt */
        /* Key 0x35 modifier key mask bits (0x0a) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '/',       /* no flags */
                NX_ASCIISET,         '?',       /* Shift */
                NX_SYMBOLSET,       0xb8,       /* Alt */
                NX_ASCIISET,        0xbf,       /* Shift Alt */
        0xff,   /* Key 0x36 unassigned - Right Shift */
        /* Key 0x37 modifier key mask bits (0x00) */
        0,
                NX_ASCIISET,         '*',       /* all */
        0xff,   /* Key 0x38 unassigned - Left Alt */
        /* Key 0x39 modifier key mask bits (0x0c) */
        (1<<NX_MODIFIERKEY_CONTROL)|(1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         ' ',       /* no flags */
                NX_ASCIISET,    CTRL('@'),      /* Ctrl */
                NX_ASCIISET,        0x80,       /* Alt */
                NX_ASCIISET,    CTRL('@'),      /* Ctrl Alt */
        0xff,   /* Key 0x3A unassigned - CAPS LOCK */
        0xff,   /* Key 0x3B unassigned - F1	*/
        0xff,   /* Key 0x3C unassigned - F2	*/
        0xff,   /* Key 0x3D unassigned - F3	*/
        0xff,   /* Key 0x3E unassigned - F4	*/
        0xff,   /* Key 0x3F unassigned - F5	*/
        0xff,   /* Key 0x40 unassigned - F6	*/
        0xff,   /* Key 0x41 unassigned - F7	*/
        0xff,   /* Key 0x42 unassigned - F8	*/
        0xff,   /* Key 0x43 unassigned - F9	*/
        0xff,   /* Key 0x44 unassigned - F10	*/
        /* Key 0x45 modifier key mask bits (0x0a) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '`',       /* no flags */
                NX_ASCIISET,         '~',       /* Shift */
                NX_ASCIISET,         '`',       /* Alt */
                NX_SYMBOLSET,       0xbb,       /* Shift Alt */
        0xff,   /* Key 0x46 unassigned */
        /* Key 0x47 modifier key mask bits (0x00) */
        0,
                NX_ASCIISET,         '7',       /* all */
        /* Key 0x48 modifier key mask bits (0x00) */
        0,
                NX_ASCIISET,         '8',       /* all */
        /* Key 0x49 modifier key mask bits (0x00) */
        0,
                NX_ASCIISET,         '9',       /* all */
        /* Key 0x4A modifier key mask bits (0x00) */
        0,
                NX_SYMBOLSET,       0x2d,       /* all */
        /* Key 0x4B modifier key mask bits (0x00) */
        0,
                NX_ASCIISET,         '4',       /* all */
        /* Key 0x4C modifier key mask bits (0x00) */
        0,
                NX_ASCIISET,         '5',       /* all */
        /* Key 0x4D modifier key mask bits (0x00) */
        0,
                NX_ASCIISET,         '6',       /* all */
        /* Key 0x4E modifier key mask bits (0x00) */
        0,
                NX_ASCIISET,         '+',       /* all */
        /* Key 0x4F modifier key mask bits (0x00) */
        0,
                NX_ASCIISET,         '1',       /* all */
        /* Key 0x50 modifier key mask bits (0x00) */
        0,
                NX_ASCIISET,         '2',       /* all */
        /* Key 0x51 modifier key mask bits (0x00) */
        0,
                NX_ASCIISET,         '3',       /* all */
        /* Key 0x52 modifier key mask bits (0x00) */
        0,
                NX_ASCIISET,         '0',       /* all */
        /* Key 0x53 modifier key mask bits (0x00) */
        0,
                NX_ASCIISET,         '.',       /* all */
        0xff,   /* Key 0x54 unassigned */
        0xff,   /* Key 0x55 unassigned */
        0xff,   /* Key 0x56 unassigned */
        0xff,   /* Key 0x57 unassigned - F11	*/
        0xff,   /* Key 0x58 unassigned - F12	*/
        0xff,   /* Key 0x59 unassigned */
        0xff,   /* Key 0x5A unassigned */
        0xff,   /* Key 0x5B unassigned */
        0xff,   /* Key 0x5C unassigned */
        0xff,   /* Key 0x5D unassigned */
        0xff,   /* Key 0x5E unassigned */
        0xff,   /* Key 0x5F unassigned */
        0xff,   /* Key 0x60 unassigned - Right Ctrl	*/
        0xff,   /* Key 0x61 unassigned - Right Alt	*/
        /* Key 0x62 modifier key mask bits (0x00) */
        0,
                NX_ASCIISET,    CTRL('C'),      /* all */
        /* Key 0x63 modifier key mask bits (0x0e) */
        (1<<NX_MODIFIERKEY_SHIFT)|(1<<NX_MODIFIERKEY_CONTROL)|
        (1<<NX_MODIFIERKEY_ALTERNATE),
                NX_ASCIISET,         '/',       /* no flags */
                NX_ASCIISET,        '\\',       /* Shift */
                NX_ASCIISET,         '/',       /* Ctrl */
                NX_ASCIISET,    CTRL('\\'),     /* Shift Ctrl */
                NX_ASCIISET,         '/',       /* Alt */
                NX_ASCIISET,        '\\',       /* Shift Alt */
                NX_ASCIISET,    CTRL('@'),      /* Ctrl Alt */
                0x0a,   CTRL('@'),      /* Shift Ctrl Alt */
        /* Key 0x64 modifier key mask bits (0x00) */
        0,
                NX_SYMBOLSET,       0xad,       /* all */
        /* Key 0x65 modifier key mask bits (0x00) */
        0,
                NX_SYMBOLSET,       0xaf,       /* all */
        /* Key 0x66 modifier key mask bits (0x00) */
        0,
                NX_SYMBOLSET,       0xac,       /* all */
        /* Key 0x67 modifier key mask bits (0x00) */
        0,
                NX_SYMBOLSET,       0xae,       /* all */
0,      /* Sequence Definitions */
9,      /* special keys */
        0x00, 0x68,     /* Sound Up */
        0x01, 0x69,     /* Sound Down */
        0x02, 0x6A,     /* Brightness Up */
        0x03, 0x6B,     /* Brightness Down */
        0x04, 0x3A,     /* Caps Lock */
	0x05, 0x3B,	/* Help Key */
        0x06, 0x47,     /* Power Key */
        0x07, 0x48,     /* Up Arrow */
        0x08, 0x50      /* Down Arrow */
};



