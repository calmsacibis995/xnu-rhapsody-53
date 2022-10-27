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
 	File:		DriverServicesPriv.h
 
 	Contains:	Defs for private DriverServices interfaces.
 
 	Version:	
 
 	DRI:		Jonathan Sand
 
 	Copyright:	© 1985-1998 by Apple Computer, Inc., all rights reserved
 
 	Warning:	*** APPLE INTERNAL USE ONLY ***
 				This file contains unreleased SPI's
 
 	BuildInfo:	Built by:			Naga Pappireddi
 				With Interfacer:	3.0d9 (PowerPC native)
 				From:				DriverServicesPriv.i
 					Revision:		32
 					Dated:			1/22/98
 					Last change by:	ngk
 					Last comment:	Change Types.i to MacTypes.i
 
 	Bugs:		Report bugs to Radar component "System Interfaces", "Latest"
 				List the version information (from above) in the Problem Description.
 
*/
/*  naga
#ifndef __DRIVERSERVICESPRIV__
#define __DRIVERSERVICESPRIV__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif
#ifndef __MACTYPES__
#include <MacTypes.h>
#endif
#ifndef __OSUTILS__
#include <OSUtils.h>
#endif
#ifndef __MIXEDMODE__
#include <MixedMode.h>
#endif
#ifndef __INTERRUPTSPRIV__
#include <InterruptsPriv.h>
#endif
#ifndef __DRIVERSERVICES__
#include <DriverServices.h>
#endif
#ifndef __KERNEL__
#include <Kernel.h>
#endif
#ifndef __TIMER__
#include <Timer.h>
#endif



#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if PRAGMA_IMPORT
#pragma import on
#endif

#if PRAGMA_STRUCT_ALIGN
	#pragma options align=mac68k
#elif PRAGMA_STRUCT_PACKPUSH
	#pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
	#pragma pack(2)
#endif
*/
/*
**
** Our private types and constants...
**
** We Make sure all longs that we compare and swap on are aligned on long boundaries.
**
** Swis, SIHs, and Timers are preallocated arrays of structures.  Each of these structs
** must start on a long word boundary for CompareAndSwap to work.
**
*/
/*
  //////////////////////////////////////////////////////////////////////////
   Secondary Interrupts 
*/

/* # preallocated SIHs from PoolAllocateResident*/
#define SIHTaskPoolSize			40

struct SIH_Routine {
	QElem 							qElement;					/* on either available OR*/
																/* in-use lists*/
																/*	OSType						name;						// 'SIHT' (Macsbug ready)*/
	SecondaryInterruptHandler2 		theHandler;					/* -> handler*/
//naga	ExceptionHandler 				theExceptionHandler;		/* not used in this implementation*/
	void *							p1;							/* first param for handler*/
	void *							p2;							/* second param for handler*/
	UInt32 							sihSequenceNumber;			/* counts queued sihs*/
	void *							reserved;					/* room to grow*/
};
typedef struct SIH_Routine				SIH_Routine;
typedef SIH_Routine *					SIH_RoutinePtr;

struct SIHGlobalsStruct {
																/*	OSType						globalsName;				// 'SIHG' (Macsbug ready)*/
	SIH_RoutinePtr 					taskPool;					/* pre-allocated queue records*/
	UInt32 							sihSequenceNumber;			/* counts queued sihs*/
	UInt32 							sihLastExecSequenceNumber;	/* last sequence number executed.*/
	void *							reserved;					/* room to grow*/

};
typedef struct SIHGlobalsStruct			SIHGlobalsStruct;

typedef SIHGlobalsStruct *				SIHGlobalsPtr;
/*
  //////////////////////////////////////////////////////////////////////////
   Memory Management
*/

enum {
	kCacheLineSize				= 32
};

typedef UInt32 							LogicalAddressLval;
typedef UInt32 							PhysicalAddressLval;
/*
  //////////////////////////////////////////////////////////////////////////
   Software Interrupts
*/
/* # preallocated Swis from PoolAllocateResident*/
#define N_SWIS	24

struct Swi_Information {
	QElem 							Link;						/* next SWI sent to this task*/
																/*	OSType						Name;						// 'SWI!' (Macsbug ready)*/
	SoftwareInterruptHandler 		Handler;					/* &TVector of handler routine*/
	SoftwareInterruptID 			The_Id;
	TaskID 							The_Task;					/* task to which SWI was or will be sent*/
	Boolean 						Privileged;					/* whether SWI will run on master stack*/
	Boolean 						Temporary;					/* whether SWI will be deleted after delivery*/
	Boolean 						Sent;						/* whether SWI has been sent*/
	Boolean 						Overrun;					/* whether non-temporary SWI has been sent twice before delivery*/
	void *							P1;							/* set at CreateSwi time*/
	void *							P2;							/* set at SendSwi time*/
	void *							reserved;					/* room to grow*/
};
typedef struct Swi_Information			Swi_Information;

typedef Swi_Information *				Swi_Info;
/*
  //////////////////////////////////////////////////////////////////////////
   Timer
*/
/* # preallocated TimerTasks from PoolAllocateResident*/
#define TimerTaskPoolSize 32

struct TimerTaskStruct {
	//nagaTMTask 							tmTask;						/* Time Manager Task Elements */
	UInt16 							Pad;						/* align to long*/
																/*	OSType						name;			// 'TIMT' (Macsbug ready)*/
	struct TimerTaskStruct *		pNextTask;					/* Timer list elements */
	TimerID 						timerID;					/* Timer elements */
	SecondaryInterruptHandler2 		theHandler;					/* -> SIH to queue*/
	void *							p1;							/* == param to SIH*/
	long 							frequency;					/* persistence frequency*/
	Boolean 						persistent;					/* if true, task is persistent*/
};
typedef struct TimerTaskStruct			TimerTaskStruct;

typedef TimerTaskStruct *				TimerTaskPtr;

struct TimerGlobalsStruct {
																/*	OSType			globalsName;		// 'TIMG' (Macsbug ready)*/
																/* Timer list elements */
	TimerTaskPtr 					taskPool;					/* pool of pre-allocated */
																/* task records */
	TimerTaskPtr 					taskFreeList;				/* list of free tasks */
	TimerTaskPtr 					taskList;					/* list of pending tasks */
	TimerID 						nextUniqueID;				/* next unique timer ID */

																/* Time Manager elements */
	//nagaTimerUPP 						tmTaskHandler;				/* Timer Manager interrupt */
																/* handler */
	void *							reserved;					/* room to grow*/
};
typedef struct TimerGlobalsStruct		TimerGlobalsStruct;

typedef TimerGlobalsStruct *			TimerGlobalsPtr;

/*
**
** Our private globals and external routines...
**
*/
/*
** Device Probe
*/
extern OSStatus			ProbeSuccess;
/*
** InterruptManager.c
*/
//naga
#define extern
//nagaextern InterruptSetObject	*theRootSetOfAllTrees;
extern UInt32				gInterruptCount;
extern SInt32				inNativeIntDispatcher;
EXTERN_API_C( void )
_SIHDeferredTaskHandler			(void);

extern void				(*nullInterruptHook)( void );
/*
** SynchronizationUnAligned.c
*/
extern UInt32			UnAlignAddAtomicCount;
extern UInt32			AddAtomicCount;
extern UInt32			UnAlignIncrementAtomicCount;
extern UInt32			IncrementAtomicCount;
extern UInt32			UnAlignDecrementAtomicCount;
extern UInt32			DecrementAtomicCount;
extern UInt32			UnAlignBitOrAtomicCount;
extern UInt32			BitOrAtomicCount;
extern UInt32			UnAlignBitAndAtomicCount;
extern UInt32			BitAndAtomicCount;
extern UInt32			UnAlignBitXorAtomicCount;
extern UInt32			BitXorAtomicCount;
extern UInt32			UnAlignCompareAndSwapCount;
extern UInt32			CompareAndSwapCount;
extern UInt32			UnAlignAddByteAtomicCount;
extern UInt32			AddByteAtomicCount;
extern UInt32			UnAlignIncrementByteAtomicCount;
extern UInt32			IncrementByteAtomicCount;
extern UInt32			UnAlignDecrementByteAtomicCount;
extern UInt32			DecrementByteAtomicCount;
extern UInt32			UnAlignAddHalfwordAtomicCount;
extern UInt32			AddHalfwordAtomicCount;
extern UInt32			UnAlignIncrementHalfwordAtomicCount;
extern UInt32			IncrementHalfwordAtomicCount;
extern UInt32			UnAlignDecrementHalfwordAtomicCount;
extern UInt32			DecrementHalfwordAtomicCount;
extern UInt32			UnAlignBitAndAtomic8Count;
extern UInt32			BitAndAtomic8Count;
extern UInt32			UnAlignBitOrAtomic8Count;
extern UInt32			BitOrAtomic8Count;
extern UInt32			UnAlignBitXorAtomic8Count;
extern UInt32			BitXorAtomic8Count;
extern UInt32			UnAlignBitAndAtomic16Count;
extern UInt32			BitAndAtomic16Count;
extern UInt32			UnAlignBitOrAtomic16Count;
extern UInt32			BitOrAtomic16Count;
extern UInt32			UnAlignBitXorAtomic16Count;
extern UInt32			BitXorAtomic16Count;
extern UInt32			UnAlignTestAndSetCount;
extern UInt32			TestAndSetCount;
extern UInt32			UnAlignTestAndClearCount;
extern UInt32			TestAndClearCount;
/*
** Timer.c
*/
extern TimerGlobalsPtr  gTimer;
/*
** Don't know
*/
EXTERN_API_C( void )
FlushRange						(LogicalAddressLval 	theLogicalAddress,
								 ByteCount 				theLength);

EXTERN_API_C( void )
InvalidateRange					(LogicalAddressLval 	theLogicalAddress,
								 ByteCount 				theLength);

/*
** DriverServices.c
*/
extern QHdrPtr				CreatedSwis;
extern QHdrPtr				AvailableSwis;
extern Swi_Information		*AllSwis;
/*
** InterruptsControl.c
*/
EXTERN_API_C( UInt16 )
Disable68kInterrupts			(void);

EXTERN_API_C( void )
Restore68kInterrupts			(UInt16 				currentSR);

/*
** InterruptManagerSAL.c
*/
//nagaextern Vector *			vectorTable;
/*
** MemoryManagement.c
*/
extern Ptr 				gDeallocateQueueHead;
extern Ptr  				gDeallocateQueueTail;
/*
** SecondaryInterrupt.c
*/
extern SIHGlobalsPtr		gSIH;
extern UInt32				atSecondaryInterruptLevel;
extern QHdrPtr				Available_SIHsPtr;
extern QHdrPtr				Deferred_SIHsPtr;
/*
** Timing.c
*/
typedef AbsoluteTime	(*UpTimeProcPtr) (void);
extern	UpTimeProcPtr	gUpTimeProc;
extern	AbsoluteTime	gElapsedTime;
extern	Boolean			gProcessorConversionChanged;
extern	UInt32			gMinAbsoluteTimeDelta;
extern	UInt32			gAbsoluteTimeToNanosecondsNumerator;
extern	UInt32			gAbsoluteTimeToNanosecondsDenominator;
extern	UInt32			gProcessorToAbsoluteTimeNumerator;
extern	UInt32			gProcessorToAbsoluteTimeDenominator;
EXTERN_API_C( AbsoluteTime )
UpTime601						(void);

EXTERN_API_C( AbsoluteTime )
UpTimePowerPC					(void);

EXTERN_API_C( void )
InitializeTiming601				(void);

EXTERN_API_C( void )
InitializeTimingPowerPC			(void);

EXTERN_API_C( void )
SetTimeBaseInfo					(UInt32 				minAbsoluteTimeDelta,
								 UInt32 				theAbsoluteTimeToNanosecondNumerator,
								 UInt32 				theAbsoluteTimeToNanosecondDenominator,
								 UInt32 				theProcessorToAbsoluteTimeNumerator,
								 UInt32 				theProcessorToAbsoluteTimeDenominator);

typedef OSStatus		(*DelayForHwdrProcPtr)(AbsoluteTime theAbsolute);
extern	DelayForHwdrProcPtr	gDelayForHdwrProc;
EXTERN_API_C( UInt32 )
DSLGetProcessorVersion			(void);

EXTERN_API_C( OSStatus )
DelayForHardwarePPC				(AbsoluteTime 			theAbsolute);

EXTERN_API_C( OSStatus )
DelayForHardware601				(AbsoluteTime 			theAbsolute);

EXTERN_API_C( UInt32 )
DeviceProbe32					(UInt32 *				theAddress);

EXTERN_API_C( UInt16 )
DeviceProbe16					(UInt16 *				theAddress);

EXTERN_API_C( UInt8 )
DeviceProbe8					(UInt8 *				theAddress);
//naga
#undef extern
/*naga
#if PRAGMA_STRUCT_ALIGN
	#pragma options align=reset
#elif PRAGMA_STRUCT_PACKPUSH
	#pragma pack(pop)
#elif PRAGMA_STRUCT_PACK
	#pragma pack()
#endif

#ifdef PRAGMA_IMPORT_OFF
#pragma import off
#elif PRAGMA_IMPORT
#pragma import reset
#endif

#ifdef __cplusplus
}
#endif

#endif  __DRIVERSERVICESPRIV__ */
