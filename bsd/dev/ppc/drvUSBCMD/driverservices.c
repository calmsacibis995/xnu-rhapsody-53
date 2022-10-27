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
	File:		DriverServices.c

	Contains:	Native Driver support code for Device Manager

	Version:	xxx put version here xxx

	Written by:	Tom Saulpaugh

	Copyright:	© 1994-1997 by Apple Computer, Inc., all rights reserved.

	File Ownership:

		DRI:				xxx put dri here xxx

		Other Contact:		xxx put other contact here xxx

		Technology:			xxx put technology here xxx

	Writers:

		(JM3)	Jim Murphy
		(CSS)	Chas Spillar
		(KW)	Kevin Williams
		(MRN)	Matthew Nelson

	Change History (most recent first):

		<63>   18-Jun-97	JM3		Updated for cleaned up NanoKernelPriv/PPCInfoRecordsPriv.
		<62>	 5/30/97	CSS		Delay() now takes an unsigned long * instead of a long * as its
									second parameter.
		<61>	 3/18/97	KW		DelayFor Hardware was just looking at 5FFF EF80 to get the
									Processor Info structure. Now use the pointer located  at
									ProcessorInfo Ptr to get the pointer to the structure
		<60>	  8/6/96	MRN		Changes to stop using private copy of 64 bit math routines and
									instead link against the real shipping version of the Math64Lib.
		<59>	10/30/95	MRN		Get rid of C version of SynchronizeIO that moved to
									DriverServices.s, and make file buildable with MrC.
		<58>	 4/15/95	tjm		#1238020 - Fix DelayForHardware to handle 601 correctly. Moved
									fix back into DSL.m4
		<57>	 4/14/95	tjm		#1238020 - Fix DelayForHardware to handle 601 correctly.
		<56>	 4/10/95	TS		Fix bug in PBEnqueueLast
		<55>	 4/10/95	TS		Optimize PBEnqueueLast for a queue of 1 or less elements
		<54>	  4/6/95	TS		Fix PBDequeue
		<53>	  4/5/95	TS		Rewrite PBEnqueueLast (Bug #1236338)
		<52>	 3/31/95	TS		Bump preallocated data numbers and comment out debugging names
									before ROM freezes.
		<51>	 3/30/95	TS		Retry CompareAndSwap in PBDequeue if element is at head of queue
		<50>	 3/30/95	TS		Add OSTypes to each Globals Struct and reserve some space here
									as well.
		<49>	 3/29/95	TS		Return last element in PBDequeueLast (Optimized path)
		<48>	 3/28/95	TS		Optimize PBDequeue and PBDequeueLast for the first element
		<47>	 3/27/95	TS		Fix bug #1232734 (IOCommandIsComplete return value)
		<46>	 3/22/95	TS		Optimize fix for bug #1228034
		<45>	 3/14/95	tjm		#1227300 - Insure that PStrNCmp works with all equal strings.
		<44>	 3/14/95	tjm		#1228034 - insure that queueing functions are safe from
									interrupts
		<43>	 2/21/95	tjm		#1220575 - Correct PStrNCat to handle strings > 255.
		<42>	 2/20/95	tjm		#1220560 - Fix PStrCat to correctly handle dest strings > 128.
		<41>	 1/27/95	tjm		#1214980 - PStrNCmp does not return correct value with null
									strings.
		<40>	 1/25/95	tjm		#1214207 - Fix PBDequeueLast to delete if only one item in
									queue.
		<39>	 1/25/95	tjm		#1214730 - Fix PStrCat to only Cat up to 255 chars.
		<38>	 1/19/95	tjm		Break out DSLInit routine to aid in debugging
		<37>	 1/19/95	tjm		#1211193 - Allow Dequeuing from head.
		<36>	 1/18/95	tjm		#1212320 - Insure PBEnqueueLast is re-entrant.
		<35>	 1/18/95	tjm		#1203060 - Make a Global Data Lib for the DSL.
		<34>	 1/16/95	tjm		#1203145 - Add DelayForHardware so drivers can call delay
									routine at InterruptTime.
		<33>	 1/10/95	tjm		#1210156 - Make sure PBQueueDelete correctly deallocated
									preallocated qHdrs.
		<32>	 1/10/95	tjm		#1210154 - return noErr for unaligned qHdr allocation.
		<31>	 1/10/95	tjm		#1207351 - Make sure PStrLen returns correct value.
		<30>	 1/10/95	tjm		#1207081 - Fix the PStr functions to handle lengths correctly
		<29>	  1/9/95	tjm		#1208049 - return paramErr if CSI fails.
		<28>	  1/9/95	tjm		#1209423 - correctly handle empty qHdr when it is empty
		<27>	  1/9/95	MN		Add a call to UpTime in the CFM initialization function to
									initialize the timebase variable in the new timing functions.
		<26>	12/21/94	MN		Restore checkins 18 through 24 and include the fixes checked
									into 25.
		<25>	12/21/94	MN		Added alignment support to PBQueueCreate and PBQueueDelete for Marconi_v16c2
									which would have checked in over the top of checkins 18 through 24.
		<24>	12/20/94	tjm		#1206624 - Fix PBDequeueLastSync to correctly dequeue item.
		<23>	12/19/94	tjm		#1204196 - fix this again.
		<22>	12/16/94	tjm		#1206622 - eliminate reentrantcy problem with PBDequeueSync.
		<21>	12/16/94	tjm		#1203051 - added PBEnqueueLast function
		<20>	12/15/94	tjm		#1204196 - call PBQueueInit when PBQueueCreate is successful.
		<19>	12/15/94	tjm		#1204193 - initialize qFlags in qHeader of PBQueueInit.
		<18>	12/15/94	tjm		#1203057 - increment the number of SWI's to 32
		<17>	 12/5/94	tjm		#1202888 - Fix PBDequeue to not corrupt queue with 2 elements
		<16>	 12/5/94	tjm		#1202776 - Fix PBDequeue to return qErr for nonexistant element.
		<15>	 12/5/94	tjm		#1201314 - correctly queue secondary interrupts
		<14>	11/15/94	tjm		#1195428 - Fix function prototypes to correctly define src ptrs
									as const.
		<13>	 11/3/94	tjm		fix DelayFor to use low end of 64 bit divide, ID#1197350
		<12>	10/25/94	tjm		Call TimerInitialize, SIHInitialize, and MemoryInitialze to fix
									#1192590
		<11>	 9/15/94	MN		Add call in DSL_Init to startup Interrupt Manager and change
									PBQueueInit to return an error code;
		<10>	 8/31/94	TS		Make the DSL_Init check for failures
		 <9>	 8/29/94	TS		Add a call to the CFM init routine to calculate physical RAM
									size using the ROM physical space table
		 <8>	 8/25/94	TS		Add DelayFor and Move BlockCopy to MemoryManagement.c
		 <7>	 8/24/94	TS		Add in checks for correct execution levels
		 <6>	 8/24/94	TS		Make DSL_Init return noErr to library will load
		 <5>	 8/24/94	TS		Build ExecutionLevel Func
		 <4>	 8/23/94	TS		Implement SoftwareInterrupts
		 <3>	 8/18/94	TS		Add more queue functions
		 <2>	 8/17/94	JF		Changes from PCI developer kitchen
		 <1>	  8/5/94	JF		first checked in

*/



//
// Source to the NuDriver Support Services Library
//

//#include <Types.h>
//#include <Strings.h>
//#include <Files.h>
//#include <MixedMode.h>
//#include <LowMem.h>
//#include <Math64.h>

// Get Prototypes of the Exported Services First,...

#include "driverservices.h"
#include "driverservicespriv.h"
#include "PPCInfoRecordsPriv.h"
#include "rhap_local.h"

// The Driver Services Follow,...


OSErr InitSwis (void);

/////////////////////////////////////////////////////////////////////////////////
//
//	Command Processing
//
//

/*-------------------------------------------------------------------------------

Routine:	IOCommandIsComplete	-	Marks I/O Command as Done.

Function:	Returns IODone result from Device Manager

Input:		theID		- ID of the in-progress I/O command.
			theResult	- the result value of the command. (Low 16-bits are placed in IOPB)

Result:		noErr	or 
-------------------------------------------------------------------------------*/
/*naga
enum {
	uppIODoneProcInfo =		kRegisterBased
						|	REGISTER_ROUTINE_PARAMETER(1,kRegisterA1,kFourByteCode)
						|	REGISTER_ROUTINE_PARAMETER(2,kRegisterD0,kFourByteCode)
};


OSErr	IOCommandIsComplete		(	IOCommandID		theID,
									OSErr			theResult)
{
	ParmBlkPtr	 The_Pb		= (ParmBlkPtr)theID;
	OSErr		*The_Result = (OSErr *)(&The_Pb -> ioParam.ioCmdAddr);
	
	if (theID == nil)
		return paramErr;

	*The_Result = theResult;
	
	(void)CallUniversalProc (	LMGetJIODone(),
								uppIODoneProcInfo,
								The_Pb,
								1 );
	return noErr;
}
naga */
/*-------------------------------------------------------------------------------

Routine:	GetIOCommandInfo	-	Returns info about an in-progress I/O command.

Function:	Returns IOCommandContents, IOCommandCode, and IOCommandKind.

Input:		theID	- ID of the in-progress I/O command.

Output:		theContents		- pointer to the IOPB or Initialize/Finalize Contents.
			theCommand		- the command code
			theKind			- the kind of I/O (Sync, Async, or Immed)
			
Result:		noErr	
-------------------------------------------------------------------------------*/
/*naga
OSErr	GetIOCommandInfo		(	IOCommandID				theID,
								    IOCommandContents	*	theContents,
									IOCommandCode		*	theCommand,
									IOCommandKind		*	theKind )
{
	IOCommandContents	The_Contents;
	ParmBlkPtr			The_Pb		= (ParmBlkPtr)theID;

	if (theID == nil)
		return paramErr;

	The_Contents . pb = The_Pb;
	
	*theContents = The_Contents;
	*theCommand  = (The_Pb  -> ioParam.ioTrap & 0x00ff);
	*theKind	 = ((The_Pb -> ioParam.ioTrap & 0xff00) >> 8);
	
	return noErr;
}
*/

/////////////////////////////////////////////////////////////////////////////////
//
//	Queues
//
//

/*-------------------------------------------------------------------------------

Routine:	PBQueueInit	-	Initialize a Queue (@ NON-Interrupt-lvl and SIH-lvl)

-------------------------------------------------------------------------------*/

OSErr PBQueueInit(QHdrPtr qHeader)
{
	qHeader -> qHead = nil;
	qHeader -> qTail = nil;
	qHeader -> qFlags = 0x0000;
	
	return noErr;
}

/*-------------------------------------------------------------------------------

Routine:	PBQueueCreate	-	Initialize a Queue (@ NON-Interrupt lvl)

-------------------------------------------------------------------------------*/

OSErr PBQueueCreate(QHdrPtr *qHeader)
{
	Ptr **theQueue;
	
	if (CurrentExecutionLevel() != kTaskLevel)
	{
		// SysDebugStr ((StringPtr)"\pPBQueueCreate not called at Task Level!");
		// 11/30/98 Adam return -1;
	}
	
	*qHeader = nil;
	theQueue = (Ptr **)PoolAllocateResident (sizeof(QHdr)+sizeof(short)+sizeof(Ptr),true);
	if (theQueue != nil)
	{
		// Store orig ptr in first 4 bytes.
		*theQueue = (Ptr *)theQueue;
		theQueue++;
		
		// Are we not aligned?
		if ( ((UInt32)theQueue & 3) == 0 )
		{
			short *pattern = (short *)theQueue;
			
			*pattern++	= 0x4D4E;
			*qHeader	= (QHdrPtr)pattern;
			return noErr;
		}
		else
		{
			*qHeader	= (QHdrPtr)theQueue;
			return noErr;
		}
	}
	else
		return mFulErr;
}

/*-------------------------------------------------------------------------------

Routine:	PBQueueDelete	-	Finalize a Queue (@ NON-Interrupt lvl)

-------------------------------------------------------------------------------*/

OSErr PBQueueDelete(QHdrPtr qHeader)
{
	Ptr	  **theMemory;
	UInt16 *pattern = (UInt16 *)qHeader;
	
	if (CurrentExecutionLevel() != kTaskLevel)
	{
		// SysDebugStr ((StringPtr)"\pPBQueueDelete not called at Task Level!");
		return -1;
	}
	
	if (*--pattern == 0x4D4E)
		qHeader = (QHdrPtr)((UInt32)(qHeader) - sizeof(UInt16));
		
	theMemory = (Ptr **)qHeader;
	theMemory--;
	
	if (*theMemory != (Ptr *)theMemory)
	{
		// SysDebugStr ((StringPtr)"\pPBQueueDelete not a Queue Header!");
		return -1;
	}

	(void)PoolDeallocate(theMemory);
	return noErr;
}

/*-------------------------------------------------------------------------------

Routine:	PBEnqueueLast	-	Add an IOPB to the end of a Request Queue. (@ Any Exec-lvl)
								(Optimized for queue size of 1 or less)

-------------------------------------------------------------------------------*/

OSErr PBEnqueueLast(QElemPtr qElement, QHdrPtr qHeader)
{
	QElemPtr currElemPtr;
	UInt16 sr;
	
	// New element will be at end of queue.
	qElement -> qLink = nil;

	// Currently an empty queue?
	while ( qHeader -> qHead == nil )
	{
		if (CompareAndSwap ((UInt32) nil, (UInt32) qElement, (UInt32 *) &qHeader->qHead))
			return noErr;
	}
	
	// Multi-element queue,...
	
	// Do it the sure, but slow way.
	sr = Disable68kInterrupts();

	// Entire queue empty?
	if ( qHeader -> qHead == nil )
	{
		// Add at head of queue
		qHeader -> qHead = qElement;
	}
	else
	{
		// Else, Get first element in queue.
		currElemPtr	= (QElemPtr)qHeader->qHead;
		
		// Move toward the end of the queue,...
		while ( currElemPtr -> qLink != nil )
		{	
			currElemPtr	= currElemPtr -> qLink;
		}
	
		currElemPtr -> qLink = qElement;
	}
	
	Restore68kInterrupts(sr);
	return noErr;
}

/*-------------------------------------------------------------------------------

Routine:	PBEnqueue	-	Add an IOPB to Head of Request Queue. (@ Any Exec-lvl)

Function:	Returns (nothing)
			
Result:		void	
-------------------------------------------------------------------------------*/

void PBEnqueue(QElemPtr qElement, QHdrPtr qHeader)
{
	do {
		qElement -> qLink = qHeader->qHead;
	} while (CompareAndSwap ((UInt32) qElement->qLink, (UInt32) qElement,
											(UInt32 *) &qHeader->qHead) == false);
}


/*-------------------------------------------------------------------------------

Routine:	PBDequeue	-	Remove an IOPB from a Request Queue.  (@ Any Exec-lvl)

Function:	Returns (nothing)
			
Result:		void	
-------------------------------------------------------------------------------*/

static OSErr PBDequeueSync(QElemPtr qElement, QHdrPtr qHeader)
{
	QElemPtr prevElemPtr, currElemPtr;
	OSErr	 result = noErr;
	UInt16	sr;
	
	// Validate params.
	if ( (qElement == nil) || (qHeader == nil) )
		return paramErr;
		
	sr = Disable68kInterrupts();
	
	currElemPtr = (QElemPtr)qHeader->qHead;
	prevElemPtr = nil;
	
	// Find the one we want.
	while ( currElemPtr && (currElemPtr != qElement) )
	{
		// Advance the cause.
		prevElemPtr = currElemPtr;
		currElemPtr = currElemPtr -> qLink;
	}
	
	// We find it?
	if ( currElemPtr )
	{
		// Middle of the queue somewhere?
		if ( prevElemPtr )
			prevElemPtr -> qLink = currElemPtr ->qLink;
		else
			qHeader->qHead = currElemPtr ->qLink;
		
		currElemPtr -> qLink = nil;
	}
	else
		result = qErr; // not on the list.
	
	Restore68kInterrupts(sr);
	return result;
}


OSErr PBDequeue(QElemPtr qElement, QHdrPtr qHeader)
{
	// Optimize for common case of 1 element on the queue.
	
	// First one on the queue?
	while ( qHeader -> qHead == qElement )
	{
		// If the element is still at the head of the queue,... Remove it, and we are done.
		if (CompareAndSwap ((UInt32) qElement,
							(UInt32) qElement -> qLink,
							(UInt32 *) &qHeader->qHead) == true)
		{
			qElement -> qLink = nil;

			return noErr;
		}
	}
	
	// Multiple elements on the queue.
	return PBDequeueSync (qElement, qHeader);
}

/*-------------------------------------------------------------------------------

Routine:	PBDequeueFirst	-	Remove First Element on a Queue  (@ Any Exec-lvl)

Result:		noErr or qErr
-------------------------------------------------------------------------------*/

OSErr PBDequeueFirst(QHdrPtr qHeader, QElemPtr *theFirstqElem)
{
	QElemPtr qElement;

	do {
		// Get Queue Head Ptr
		qElement = qHeader -> qHead;
		
		// Empty Queue?
		if ( qElement == nil )
		{
			if ( theFirstqElem )
				*theFirstqElem = nil;
				
			return qErr;	
		}
		
		// If the element is still at the head of the queue,... Remove it, and we are done.
		if (CompareAndSwap ((UInt32) qElement,
							(UInt32) qElement -> qLink,
							(UInt32 *) &qHeader->qHead) == true)
		{
			if ( theFirstqElem )
				*theFirstqElem = qElement;
				
			qElement -> qLink = nil;

			return noErr;
		}
	} while (1);
}

/*-------------------------------------------------------------------------------

Routine:	PBDequeueLast	-	Remove Last Element on a Queue  (@ Any Exec-lvl)

Result:		noErr or qErr
-------------------------------------------------------------------------------*/

OSErr PBDequeueLast(QHdrPtr qHeader, QElemPtr *theLastqElem)
{
	QElemPtr currElemPtr, prevElemPtr;
	OSErr	result = noErr;
	UInt16 sr;
	
	if ( theLastqElem )
		*theLastqElem = nil;
		
	sr = Disable68kInterrupts();

	// Get Queue Head Ptr and our initial previous ptr
	if ((prevElemPtr = qHeader -> qHead) == nil)
		result = qErr; // q is empty
	else
	{
		// Get head of queue
		currElemPtr	= (QElemPtr)qHeader->qHead;
		
		// Move toward the end of the queue,...
		while ( currElemPtr -> qLink != nil )
		{	
			prevElemPtr	= currElemPtr;
			currElemPtr	= currElemPtr ->qLink;
		}
	
		// Remove last one from the queue.
		prevElemPtr->qLink = nil;
		
		// If last was also the first, nil out qheader.
		if ( currElemPtr == qHeader -> qHead )
			qHeader -> qHead = nil;
	
		// Return last element to caller.
		if ( theLastqElem )
			*theLastqElem = currElemPtr;
	}
	
	Restore68kInterrupts(sr);
	return result;
}

/////////////////////////////////////////////////////////////////////////////////
//
//	Strings
//
//

/*-------------------------------------------------------------------------------

Routine:	CStrCopy	-	Copy string of chars from src to dst.

Function:	Copies characters upto and including the null character from 'src' to
			'dst' char strings.

Note:		This routine assumes the two strings do not overlap. It would be bad.
			
Input:		src		- pointer to the source string of char's to copy.

Output:		dst		- pointer to the destination string of char's.
			
Result:		dst		- a pointer to the destination string is returned.
-------------------------------------------------------------------------------*/

char	*CStrCopy	(char *dst,	const char	*src)
{
	char	*pos = dst;
	
	while ( (*pos++ = *src++) != 0)				// copy src to dst
		;
		
	return dst;
}


/*-------------------------------------------------------------------------------

Routine:	PStrCopy	-	Copy string of chars from src to dst.

Function:	Copies characters upto and including the null character from 'src' to
			'dst' char strings.

Note:		This routine assumes the two strings do not overlap. It would be bad.
			
Input:		src		- pointer to the source string of char's to copy.

Output:		dst		- pointer to the destination string of char's.
			
Result:		dst		- a pointer to the destination string is returned.
-------------------------------------------------------------------------------*/

StringPtr	PStrCopy	(StringPtr dst,	ConstStr255Param src)
{
	UInt8 	  len;
	char     *s,*d;
	
	s		= (char *)src;
	d		= (char *)dst;
	len		= *src;
	
	*d++ = *s++;
	while (len-- != 0)
		*d++ = *s++;
	
	return dst;
}


/*-------------------------------------------------------------------------------

Routine:	CStrNCopy	-	Copy at most n characters.

Function:	Copies upto 'max' characters from 'src' to 'dst' char strings.
			If 'src' string is shorter than 'max', the 'dst string will be
			padded with null characters. If 'src' string is longer than 'max'
			the 'dst' string will NOT be null terminated.
			
			See ANSI X3.159-1989 p.106 - "strncpy"


Input:		src		- pointer to the source string of char's to copy.
			max		- maximum number of characters to copy.
			

Output:		dst		- pointer to the destination string of char's.
			
Result:		dst		- a pointer to the destination string is returned.
-------------------------------------------------------------------------------*/

char	*CStrNCopy	(char *dst,	const char	*src,	UInt32	max)
{
	char	*pos = dst;
	
	max++;									// bump up max so we can predecrement in tests
	do {									// copy upto max chars to dst
		if (--max == 0)		return dst;		// exit if we've copied max chars
	} while ( (*pos++ = *src++) != 0);				// copy char & test

	while ( --max )	*pos++ = 0;				// pad remainder of string with nulls
											// unless we have already reached max.
	return dst;
}



/*-------------------------------------------------------------------------------

Routine:	PStrNCopy	-	Copy string of chars from src to dst.

Function:	Copies max characters from 'src' to 'dst' char strings.

Input:		src		- pointer to the source string of char's to copy.

Output:		dst		- pointer to the destination string of char's.
			
Result:		dst		- a pointer to the destination string is returned.
-------------------------------------------------------------------------------*/

StringPtr	PStrNCopy	(StringPtr dst,	ConstStr255Param src, UInt32	max)
{
	UInt8	len;
	char	*s,*d;
	char	count = 0;
	
	s		= (char *)src;
	d		= (char *)dst;
	len		= *s;
	
	do {							
		if ( (max-- == 0) || (len-- == 0))
			break;				

		*++d = *++s;
		count += 1;

	} while ( 1 );
	
	if ( count )
		*dst = count;
		
	return dst;
}


/*-------------------------------------------------------------------------------

Routine:	CStrCat	-	Append 'src' char string onto 'dst'.

Function:	Appends characters from 'src' to 'dst' char strings.
			The initial character of 'src' overwrites the null character at
			the end of 'dst'. A terminating null character is ALWAYS appended.
			

Input:		src		- pointer to the source string of char's to append.

Output:		dst		- pointer to the destination string of char's.
			
			
Result:		dst		- a pointer to the destination string is returned.
-------------------------------------------------------------------------------*/

char	*CStrCat	(char *dst,	const char *src)
{
	char	*pos = dst;
	
	while ( *pos++ )						// find the end of dst string
		;
		
	--pos;									// backup to point to null word

	while ( (*pos++ = *src++) != 0)			// copy src to dst	
		;
		
	return dst;
}




/*-------------------------------------------------------------------------------

Routine:	PStrCat	-	Append 'src' char string onto 'dst'.

Function:	Appends characters from 'src' to 'dst' char strings.			

Input:		src		- pointer to the source string of char's to append.

Output:		dst		- pointer to the destination string of char's.
			
			
Result:		dst		- a pointer to the destination string is returned.
-------------------------------------------------------------------------------*/

StringPtr PStrCat	(StringPtr dst,	ConstStr255Param src)
{
	UInt8	*pos = (UInt8 *)dst;
	UInt8	*d	 = (UInt8 *)dst;
	UInt8	*s	 = (UInt8 *)src;
	UInt8	len  = *s++;
	UInt8	dlen  = *d;


	if (((UInt32)len + (UInt32)dlen) > (UInt32)255)
	{
		len = 255 - dlen;
	}
	pos += (*pos) + 1;						// pos at end of string.
	
	while ( len-- != 0 )
	{
		*pos++ = *s++;						// car src to dst	
		(*d) += 1;							// dump new string length
	}
	
	return dst;
}



/*-------------------------------------------------------------------------------

Routine:	CStrNCat	-	Append at most n characters.

Function:	Appends upto 'max' characters from 'src' to 'dst' char strings.
			The initial character of 'src' overwrites the null character at
			the end of 'dst'. A terminating null character is ALWAYS appended.
			Thus, the maximum length of 'dst' could be CStrLen(dst)+max+1.
			
			See ANSI X3.159-1989 p.164 - "strncat"


Input:		src		- pointer to the source string of char's to append.
			max		- maximum number of characters to append.
			

Output:		dst		- pointer to the destination string of char's.
			
			
Result:		dst		- a pointer to the destination string is returned.
-------------------------------------------------------------------------------*/

char	*CStrNCat	(char *dst,	const char *src,	UInt32	max)
{
	char	*pos = dst;
	
	while ( *pos++ )						// find the end of dst string
		;
		
	--pos;									// backup to point to null char
	++max;									// bump up max so we can predecrement test
	while ( --max && ( (*pos++ = *src++) != 0))	// append upto max chars to dst
		;
		
	if (max == 0)							// if we reached max chars
		*pos = 0;							// append null character
		
	return dst;
}



/*-------------------------------------------------------------------------------

Routine:	PStrNCat	-	Append 'src' char string onto 'dst'.

Function:	Appends characters from 'src' to 'dst' char strings.			

Input:		src		- pointer to the source string of char's to append.

Output:		dst		- pointer to the destination string of char's.
			
			
Result:		dst		- a pointer to the destination string is returned.
-------------------------------------------------------------------------------*/

StringPtr PStrNCat	(StringPtr dst,	ConstStr255Param src, UInt32	max)
{
	UInt8	*pos = (UInt8 *)dst;
	UInt8	*d	 = (UInt8 *)dst;
	UInt8	*s	 = (UInt8 *)src;
	UInt8	len  = *s++;
	

	len = (((UInt32)*src + (UInt32)*dst) < (UInt32)255) ? (UInt32)*src : (UInt32)255 - (UInt32)*dst;
	
	pos += (*pos) + 1;						// pos at end of string.

	while ( (max-- != 0) && (len-- != 0) )
	{
		*pos++ = *s++;						// car src to dst	
		(*d) += 1;							// dump new string length
	}
	
	return dst;
}


/*-------------------------------------------------------------------------------

Routine:	PStrToCStr	-	convert Pascal string 'src' to char string 'dst'.

Function:	Converts 1 byte Pascal string chars to 2 byte char string by prefixing
			the ASCII byte with 0x00. The char string is terminated with a 2 byte
			null character, 0x0000.
			

Input:		src		- pointer to the Pascal source string.

Output:		dst		- pointer to the destination string of char's.
-------------------------------------------------------------------------------*/

void	PStrToCStr	(char	*dst,	ConstStr255Param src)
{
	unsigned char	len;
	
	len = 1 + *src++;							// add 1 so we can predecrement
	while (--len)								// for each char in src...
	{
		*dst++ = (char) *src++;					// cast & copy chars
	}
	
	*dst = 0;									// append null char
}



/*-------------------------------------------------------------------------------

Routine:	CStrToPStr	-	convert char string 'src' to Pascal string 'dst'.

Function:	Converts 2 byte char string chars to 1 byte Pascal string by lopping
			off the high byte. This is NOT considered sound foreign policy, and we
			will have to accomodate our international brethren lest they be displeased
			with us. But, it is useful for development and debugging.
			

Input:		src		- pointer to the char source string.

Output:		dst		- pointer to the Pascal destination string.
-------------------------------------------------------------------------------*/

void	CStrToPStr	(Str255		dst,	const char	*src)
{
	UInt32			 len	= 0;
	const char	*s2	= src;
	
	while (*src++)							// how long is it?
		++len;
	
	if (len > 255)	len = 255;				// truncate to 255 chars
	
	*dst++ = (unsigned char) len++;			// stuff length byte &
											// increment so we can predecrement
	while (--len)
		*dst++ = *s2++;						// copy len chars
}



/////////////////////////////// Compare Routines ////////////////////////////////


/*-------------------------------------------------------------------------------

Routine:	CStrCmp	-	Compare two char strings.

Function:	Compares the char strings s1 and s2 by comparing the values of
			corresponding characters in each string. It does NOT take into
			consideration case, diacriticals, or other localization requirements.


Input:		s1		- pointer to char string.
			s2		- pointer to char string.

			
Result:		-1		- s1 < s2
			 0		- s1 = s2
			 1		- s1 > s2
-------------------------------------------------------------------------------*/

short	CStrCmp	(const char *s1,	const char	*s2)
{
	char		chr;
	
	while ( (chr = *s1++) != 0)
	{ 
		if ( *s2++ != chr )
		{
			if ( chr < *--s2 )
				return -1;
			return 1;
		}
	}
						// here chr==0 and s2 has not advanced
	if(*s2 == 0)
		return 0;
	return -1;
}



/*-------------------------------------------------------------------------------

Routine:	PStrCmp	-	Compare two char strings.

Function:	Compares the char strings s1 and s2 by comparing the values of
			corresponding characters in each string. It does NOT take into
			consideration case, diacriticals, or other localization requirements.


Input:		s1		- pointer to Pascal string.
			s2		- pointer to Pascal string.

			
Result:		-1		- s1 < s2
			 0		- s1 = s2
			 1		- s1 > s2
-------------------------------------------------------------------------------*/

short	PStrCmp	(ConstStr255Param str1, ConstStr255Param str2)
{
	UInt8		s1len,s2len;
	char		chr, *s1, *s2;
	
	s1		= (char *)str1;
	s2		= (char *)str2;
	s1len   = *s1++;
	s2len   = *s2++;
	
	while (s1len && s2len)
	{ 
		s1len--; s2len--;
		chr = *s1++;
		if ( *s2++ != chr )
		{
			if ( chr < *--s2 )
				return -1;
			return 1;
		}
	}
						
	if((s2len == 0) && (s1len == 0))
		return 0;

	if(s1len)
		return 1;
	return -1;
}



/*-------------------------------------------------------------------------------

Routine:	CStrNCmp	-	Compare the first N characters of two char strings.

Function:	Compares the first N char strings s1 and s2 by comparing the values
			of corresponding characters in each string. It does NOT take into
			consideration case, diacriticals, or other localization requirements.


Input:		s1		- pointer to char string.
			s2		- pointer to char string.
			max		- the number of characters to compare.

			
Result:		-1		- s1 < s2
			 0		- s1 = s2
			 1		- s1 > s3
-------------------------------------------------------------------------------*/

short	CStrNCmp	(const char *s1,	const char	*s2,	UInt32	max)
{
	register char c1, c2;		/* s1[] may be shorter s2[] or
										  their length may be equal */

	while (max--)
	{ 
		c1 = *s1++;
		c2 = *s2++;

		if ( c1 != c2 )
		{	
			if ( c1 < c2 )	return -1;
			else			return	1;
		}

		if(c1 == 0)		return	0;
	}
	
	return	0;
}



/*-------------------------------------------------------------------------------

Routine:	PStrNCmp	-	Compare two char strings.

Function:	Compare the first N characters of two char strings.


Input:		s1		- pointer to Pascal string.
			s2		- pointer to Pascal string.
			max		- N chars to compare

			
Result:		-1		- s1 < s2
			 0		- s1 = s2
			 1		- s1 > s2
-------------------------------------------------------------------------------*/

short	PStrNCmp	(ConstStr255Param str1, ConstStr255Param str2, UInt32	max)
{
	char c1, c2;		
	UInt8 l1, l2;
	char *s1,*s2;

	l1 = str1[0]; s1 = (char *)str1; s1++;
	l2 = str2[0]; s2 = (char *)str2; s2++;
	
	if ((l1 == 0) && (l2 == 0))
		return	0;
		
	while (max && l1 && l2)
	{ 
		max--; l1--; l2--;
		c1 = *s1++;
		c2 = *s2++;

		if ( c1 != c2 )
		{	
			if ( c1 < c2 )	return -1;
			else			return	1;
		}
	}
	
	if ((max == 0) || ((l1 == 0) && (l2 == 0)))
		return	0;
		
	if (l1)
		return 1;
	return -1;
}


/*-------------------------------------------------------------------------------

Routine:	CStrLen	-	returns length of char string in characters.

Function:	Returns the length of char string 'src' in characters. This does
			NOT include the terminating null character.


Input:		src		- pointer to a char string.
			
Result:		number of characters in char string 'src'

-------------------------------------------------------------------------------*/

UInt32	CStrLen	(const char	*src)
{
	UInt32 len = 0;
	
	while (*src++)
		++len;
		
	return len;
}

/*-------------------------------------------------------------------------------

Routine:	PStrLen	-	returns length of Pascal string in characters.

Function:	Returns the length of Pascal string 'src' in characters.

Input:		src		- pointer to a Pascal string.
			
Result:		number of characters in Pascal string 'src'

-------------------------------------------------------------------------------*/

UInt32	PStrLen	(ConstStr255Param src)
{
	UInt8	strlen;
	char	*s = (char *)src;
	
	strlen = *s;
	
	return (UInt32)strlen;
}


/////////////////////////////////////////////////////////////////////////////////
//
//	Software Interrupts
//
//

OSErr InitSwis (void)
{
	long	i;
	Swi_Information	*swi;
	
	PBQueueCreate (&CreatedSwis);
	
	PBQueueCreate (&AvailableSwis);
	
	AllSwis = PoolAllocateResident( sizeof(Swi_Information) * N_SWIS, true );
	if ( !AllSwis )
		return memFullErr;
	
	swi = AllSwis;	
	for (i=0;i<N_SWIS;i++)
	{
		// swi->Name = 'SWI!';
		PBEnqueue ( &swi->Link, AvailableSwis);
		swi++;
	}
		
	return noErr;
}

/*-------------------------------------------------------------------------------

Routine:	CurrentTaskID	-	Returns ID of the Current Task. (@NON-Int Lvl)

-------------------------------------------------------------------------------*/

TaskID	CurrentTaskID (void)
{
	if (CurrentExecutionLevel() != kTaskLevel)
	{
		// SysDebugStr ((StringPtr)"\pCurrentTaskID not called at Task Level!");
		return (TaskID)-1;
	}
	
	return (TaskID)0;
}

/*-------------------------------------------------------------------------------

Routine:	DelayFor	-	Block Current Task for a Duration. (@NON-Int Lvl)

-------------------------------------------------------------------------------*/
/*naga
OSStatus DelayFor ( Duration expirationTime )
{
	AbsoluteTime	abt;
	Nanoseconds		nano;
	SInt64			divisor;
	SInt64			remainder;
	SInt64			numTicks;
	unsigned long	finalTicks;
	
	if (CurrentExecutionLevel() != kTaskLevel)
	{
		// SysDebugStr ((StringPtr)"\pDelayFor not called at Task Level!");
		return -1;
	}
	
	abt		= DurationToAbsolute	( expirationTime );
	nano	= AbsoluteToNanoseconds ( abt );
	
	divisor.hi = 0;
	divisor.lo = 16666666;

	numTicks = S64Divide( *(SInt64*)(&nano), divisor, &remainder );
	
	Delay ( numTicks.lo, &finalTicks );
	
	return noErr;
}
naga */
#if 0   //naga
/*-------------------------------------------------------------------------------

Routine:	DelayForHardware	-	Delay for an AbsoluteTime. (Int Lvl)

-------------------------------------------------------------------------------*/

OSStatus DelayForHardware ( AbsoluteTime absoluteTime )
{
	// Obtain the NanoKernel's private pointer to the ProcessorInfo structure.
	// This pointer is contained in the location defined by the private enum
	// 'ProcessorInfoPtr'
	NKProcessorInfo *processorInfo = *((NKProcessorInfo **) nkProcessorInfoPtr);

	if (gDelayForHdwrProc == nil)
	{
		
		// First Time Initialization
		
		if ((processorInfo->ProcessorVersionReg >> 16) == 0x0001)
		{
			// PowerPC 601 Chip (Use RTC, not TimeBase)
			
			gDelayForHdwrProc = DelayForHardware601;
		}
		else
		{
			// Non-PowerPC 601 (Use TimeBase, not RTC)
			
			gDelayForHdwrProc = DelayForHardwarePPC;
		}
	}
	
	return gDelayForHdwrProc(absoluteTime);
}
#endif
/*-------------------------------------------------------------------------------

Routine:	CreateSoftwareInterrupt	-	Makes a SWI. (@any exec-Lvl)

-------------------------------------------------------------------------------*/

OSStatus CreateSoftwareInterrupt	(	SoftwareInterruptHandler	theHandler,
										TaskID						theTask,
										void			*			theParameter,
										Boolean						persistent,
										SoftwareInterruptID *		theSoftwareInterrupt)
{
	Swi_Info	aSwi;
	
	if ( PBDequeueFirst (AvailableSwis, (QElemPtr *)&aSwi) != noErr )
		return paramErr;
		
	aSwi -> Handler		= theHandler;
	aSwi -> The_Id		= (SoftwareInterruptID)aSwi;
	aSwi -> The_Task	= theTask;
	aSwi -> Temporary	= !persistent;
	aSwi -> Sent		= false;
	aSwi -> P1			= (void *)theParameter;
	
	if (theSoftwareInterrupt != nil)
		*theSoftwareInterrupt = (SoftwareInterruptID)aSwi;
		
	PBEnqueue ( &aSwi->Link, CreatedSwis);
	
	return noErr;
}


/*-------------------------------------------------------------------------------

Routine:	SendSoftwareInterrupt	-	Runs a SWI. (@any exec-Lvl)

-------------------------------------------------------------------------------*/

OSStatus SendSoftwareInterrupt		(	SoftwareInterruptID			theSoftwareInterrupt,
										void			*			theParameter)
{
	Swi_Info	aSwi;
	SoftwareInterruptHandler handler;
	Boolean		temp;
	void 		*p1;
	OSErr		Result = PBDequeue ( (QElemPtr)theSoftwareInterrupt, CreatedSwis);
	
	if (Result != noErr)
		return (OSStatus)Result;
		
	aSwi = (Swi_Info)theSoftwareInterrupt;
	
	// Store away swi info in locals.
	handler = aSwi -> Handler;
	p1		= aSwi -> P1;
	temp	= aSwi -> Temporary;
	
	// Return temp swis now.
	if ( temp )
		PBEnqueue ( &aSwi->Link, AvailableSwis );
		
	// Call the handler
	(*handler) (p1, (void *)theParameter);
	
	// Return persistant swis later.
	if ( !temp )
		PBEnqueue ( &aSwi->Link, CreatedSwis );
		
	return noErr;
}

/*-------------------------------------------------------------------------------

Routine:	DeleteSoftwareInterrupt	-	Destroys a SWI. (@any exec-Lvl)

-------------------------------------------------------------------------------*/

OSStatus DeleteSoftwareInterrupt	( SoftwareInterruptID theSoftwareInterrupt )
{
	Swi_Info	aSwi;
	OSErr		Result = PBDequeue ( (QElemPtr)theSoftwareInterrupt, CreatedSwis);

	if (Result != noErr)
		return (OSStatus)Result;
		
	aSwi = (Swi_Info)theSoftwareInterrupt;
	
	PBEnqueue ( &aSwi->Link, AvailableSwis );
	
	return noErr;
}


