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
 	File:		PPCInfoRecordsPriv.h
 
 	Contains:	PowerPC information records created by hardware intialization and maintained by the NanoKernel.
 
 	Version:	Mac OS
 
 	DRI:		Jim Murphy
 
 	Copyright:	© 1997-1998 by Apple Computer, Inc., all rights reserved.
 
 	Warning:	*** APPLE INTERNAL USE ONLY ***
 				This file contains unreleased SPI's
 
 	BuildInfo:	Built by:			Naga Pappireddi
 				With Interfacer:	3.0d9 (PowerPC native)
 				From:				PPCInfoRecordsPriv.i
 					Revision:		18
 					Dated:			9/9/98
 					Last change by:	RAV
 					Last comment:	Set nanokernel info rev to 1.0.4. I'm reserving 2.0.0 for the
 
 	Bugs:		Report bugs to Radar component "System Interfaces", "Latest"
 				List the version information (from above) in the Problem Description.
 
*/
#ifndef __PPCINFORECORDSPRIV__
#define __PPCINFORECORDSPRIV__

#ifndef __MACTYPES__
#include <MacTypes.h>
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


/*
  _______________________________________________________________________
  	Configuration Info Record
  	Used to pass Configuration information from the Boot Program to the
  	NanoKernel for data structure and address mapping initialization.
  _______________________________________________________________________
*/

struct NKConfigurationInfo {
																/*	Physical ROM Info*/
	UInt32 							ROMCheckSumByte0;			/* ROM Checksum - Byte Lane 0*/
	UInt32 							ROMCheckSumByte1;			/* ROM Checksum - Byte Lane 1*/
	UInt32 							ROMCheckSumByte2;			/* ROM Checksum - Byte Lane 2*/
	UInt32 							ROMCheckSumByte3;			/* ROM Checksum - Byte Lane 3*/
	UInt32 							ROMCheckSumByte4;			/* ROM Checksum - Byte Lane 4*/
	UInt32 							ROMCheckSumByte5;			/* ROM Checksum - Byte Lane 5*/
	UInt32 							ROMCheckSumByte6;			/* ROM Checksum - Byte Lane 6*/
	UInt32 							ROMCheckSumByte7;			/* ROM Checksum - Byte Lane 7*/
	UInt64 							ROMCheckSum64;				/* ROM Checksum - 64 bit sum of doublewords*/

	UInt32 							ROMImageBaseOffset;			/* Offset of Base of total ROM image*/
	UInt32 							ROMImageSize;				/* Number of bytes in ROM image*/
	UInt32 							ROMImageVersion;			/* ROM Version number for entire ROM*/

																/*	ROM component Info (offsets are from base of ConfigInfo page)*/
	UInt32 							Mac68KROMOffset;			/* Offset of base of Macintosh 68K ROM*/
	UInt32 							Mac68KROMSize;				/* Number of bytes in Macintosh 68K ROM*/

	UInt32 							HWInitCodeOffset;			/* Offset of base of Hardware Init Code*/
	UInt32 							HWInitCodeSize;				/* Number of bytes in Hardware Init Code*/

	UInt32 							DiagPEFBundleOffset;		/* Offset of base of Diag PEF Bundle*/
	UInt32 							DiagPEFBundleSize;			/* Number of bytes in Diag PEF Bundle*/

	UInt32 							KernelCodeOffset;			/* Offset of base of NanoKernel Code*/
	UInt32 							KernelCodeSize;				/* Number of bytes in NanoKernel Code*/

	UInt32 							EmulatorCodeOffset;			/* Offset of base of Emulator Code*/
	UInt32 							EmulatorCodeSize;			/* Number of bytes in Emulator Code*/

	UInt32 							OpcodeTableOffset;			/* Offset of base of Opcode Table*/
	UInt32 							OpcodeTableSize;			/* Number of bytes in Opcode Table*/

																/*	Offsets within the Emulator Data Page.*/
	UInt8 							BootstrapVersion[16];		/* Bootstrap loader version info*/
	UInt32 							BootVersionOffset;			/* offset within EmulatorData of BootstrapVersion*/
	UInt32 							ECBOffset;					/* offset within EmulatorData of ECB*/
	UInt32 							IplValueOffset;				/* offset within EmulatorData of IplValue*/

																/*	Offsets within the Emulator Code.*/
	UInt32 							EmulatorEntryOffset;		/* offset within Emulator Code of entry point*/
	UInt32 							KernelTrapTableOffset;		/* offset within Emulator Code of KernelTrapTable*/

																/*	Interrupt Passing Masks.*/
	UInt32 							TestIntMaskInit;			/* initial value for test interrupt mask*/
	UInt32 							ClearIntMaskInit;			/* initial value for clear interrupt mask*/
	UInt32 							PostIntMaskInit;			/* initial value for post interrupt mask*/
	UInt32 							LA_InterruptCtl;			/* logical address of Interrupt Control I/O page*/
	UInt8 							InterruptHandlerKind;		/* kind of handler to use*/
	UInt8 							filler1;
	UInt16 							filler2;

	UInt32 							LA_InfoRecord;				/* logical address of InfoRecord page*/
	UInt32 							LA_KernelData;				/* logical address of KernelData page*/
	UInt32 							LA_EmulatorData;			/* logical address of EmulatorData page*/
	UInt32 							LA_DispatchTable;			/* logical address of Dispatch Table*/
	UInt32 							LA_EmulatorCode;			/* logical address of Emulator Code*/

	UInt32 							MacLowMemInitOffset;		/* offset to list of LowMem addr/data values*/

																/*	Address Space Mapping.*/
	UInt32 							PageAttributeInit;			/* default WIMG/PP settings for PTE creation*/
	UInt32 							PageMapInitSize;			/* size of page mapping info*/
	UInt32 							PageMapInitOffset;			/* offset to page mapping info (from base of ConfigInfo)*/
	UInt32 							PageMapIRPOffset;			/* offset of InfoRecord map info (from base of PageMap)*/
	UInt32 							PageMapKDPOffset;			/* offset of KernelData map info (from base of PageMap)*/
	UInt32 							PageMapEDPOffset;			/* offset of EmulatorData map info (from base of PageMap)*/

	UInt32 							SegMap32SupInit[32];		/* 32 bit mode Segment Map Supervisor space*/
	UInt32 							SegMap32UsrInit[32];		/* 32 bit mode Segment Map User space*/
	UInt32 							SegMap32CPUInit[32];		/* 32 bit mode Segment Map CPU space*/
	UInt32 							SegMap32OvlInit[32];		/* 32 bit mode Segment Map Overlay mode*/

	UInt32 							BATRangeInit[32];			/* BAT mapping ranges*/

	UInt32 							BatMap32SupInit;			/* 32 bit mode BAT Map Supervisor space*/
	UInt32 							BatMap32UsrInit;			/* 32 bit mode BAT Map User space*/
	UInt32 							BatMap32CPUInit;			/* 32 bit mode BAT Map CPU space*/
	UInt32 							BatMap32OvlInit;			/* 32 bit mode BAT Map Overlay mode*/

																/*	Only needed for Smurf*/
	UInt32 							SharedMemoryAddr;			/* physical address of Mac/Smurf shared message mem*/

	UInt32 							PA_RelocatedLowMemInit;		/* physical address of RelocatedLowMem*/

	UInt32 							OpenFWBundleOffset;			/* Offset of base of OpenFirmware PEF Bundle*/
	UInt32 							OpenFWBundleSize;			/* Number of bytes in OpenFirmware PEF Bundle*/

	UInt32 							LA_OpenFirmware;			/* logical address of Open Firmware*/
	UInt32 							PA_OpenFirmware;			/* physical address of Open Firmware*/
	UInt32 							LA_HardwarePriv;			/* logical address of HardwarePriv callback*/

	UInt32 							filler3[2];					/* pad structure size out to a cache block*/
};
typedef struct NKConfigurationInfo		NKConfigurationInfo;

/*
  _______________________________________________________________________
  	System Info Record
  	Used to pass System information from the NanoKernel to user mode
  	software.
  _______________________________________________________________________
*/


enum {
	nkSystemInfoPtr				= 0x5FFFEFF0,					/* logical address of SystemInfo record*/
	nkSystemInfoVer				= 0x5FFFEFF4,					/* version number of SystemInfo record*/
	nkSystemInfoLen				= 0x5FFFEFF6					/* length of SystemInfo record*/
};


struct NKSystemInfo {
	UInt32 							PhysicalMemorySize;			/* Number of bytes in Physical RAM*/
	UInt32 							UsableMemorySize;			/* Number of bytes in Usable RAM*/
	UInt32 							LogicalMemorySize;			/* Number of bytes in Logical RAM*/
	UInt32 							HashTableSize;				/* Number of bytes in Memory Hash Table*/

	UInt32 							L2DataCacheTotalSize;		/* number of bytes in the L2 Data Cache*/
	UInt32 							L2InstCacheTotalSize;		/* number of bytes in the L2 Instruction Cache*/
	UInt16 							L2CombinedCaches;			/* 1 <- combined or no cache, 0 <- split cache*/
	UInt16 							L2InstCacheBlockSize;		/* number of bytes in a Block of the L2 Instruction Cache*/
	UInt16 							L2DataCacheBlockSize;		/* number of bytes in a Block of the L2 Data Cache*/
	UInt16 							L2InstCacheAssociativity;	/* Associativity of the L2 Instruction Cache*/
	UInt16 							L2DataCacheAssociativity;	/* Associativity of the L2 Data Cache*/
	UInt16 							unused_1;					/* unused*/

	UInt16 							unused_2;					/* unused*/
	UInt8 							FlashManufacturerCode;		/* Flash ROM Manufacturer code*/
	UInt8 							FlashDeviceCode;			/* Flash ROM Device code*/
	UInt32 							FlashStart;					/* Starting address of Flash ROM*/
	UInt32 							FlashSize;					/* Number of bytes in  Flash ROM*/

	UInt32 							Bank0Start;					/* Starting address of RAM bank 0*/
	UInt32 							Bank0Size;					/* Number of bytes in  RAM bank 0*/
	UInt32 							Bank1Start;					/* Starting address of RAM bank 1*/
	UInt32 							Bank1Size;					/* Number of bytes in  RAM bank 1*/
	UInt32 							Bank2Start;					/* Starting address of RAM bank 2*/
	UInt32 							Bank2Size;					/* Number of bytes in  RAM bank 2*/
	UInt32 							Bank3Start;					/* Starting address of RAM bank 3*/
	UInt32 							Bank3Size;					/* Number of bytes in  RAM bank 3*/
	UInt32 							Bank4Start;					/* Starting address of RAM bank 4*/
	UInt32 							Bank4Size;					/* Number of bytes in  RAM bank 4*/
	UInt32 							Bank5Start;					/* Starting address of RAM bank 5*/
	UInt32 							Bank5Size;					/* Number of bytes in  RAM bank 5*/
	UInt32 							Bank6Start;					/* Starting address of RAM bank 6*/
	UInt32 							Bank6Size;					/* Number of bytes in  RAM bank 6*/
	UInt32 							Bank7Start;					/* Starting address of RAM bank 7*/
	UInt32 							Bank7Size;					/* Number of bytes in  RAM bank 7*/
	UInt32 							Bank8Start;					/* Starting address of RAM bank 8*/
	UInt32 							Bank8Size;					/* Number of bytes in  RAM bank 8*/
	UInt32 							Bank9Start;					/* Starting address of RAM bank 9*/
	UInt32 							Bank9Size;					/* Number of bytes in  RAM bank 9*/
	UInt32 							Bank10Start;				/* Starting address of RAM bank 10*/
	UInt32 							Bank10Size;					/* Number of bytes in  RAM bank 10*/
	UInt32 							Bank11Start;				/* Starting address of RAM bank 11*/
	UInt32 							Bank11Size;					/* Number of bytes in  RAM bank 11*/
	UInt32 							Bank12Start;				/* Starting address of RAM bank 12*/
	UInt32 							Bank12Size;					/* Number of bytes in  RAM bank 12*/
	UInt32 							Bank13Start;				/* Starting address of RAM bank 13*/
	UInt32 							Bank13Size;					/* Number of bytes in  RAM bank 13*/
	UInt32 							Bank14Start;				/* Starting address of RAM bank 14*/
	UInt32 							Bank14Size;					/* Number of bytes in  RAM bank 14*/
	UInt32 							Bank15Start;				/* Starting address of RAM bank 15*/
	UInt32 							Bank15Size;					/* Number of bytes in  RAM bank 15*/
	UInt32 							Bank16Start;				/* Starting address of RAM bank 16*/
	UInt32 							Bank16Size;					/* Number of bytes in  RAM bank 16*/
	UInt32 							Bank17Start;				/* Starting address of RAM bank 17*/
	UInt32 							Bank17Size;					/* Number of bytes in  RAM bank 17*/
	UInt32 							Bank18Start;				/* Starting address of RAM bank 18*/
	UInt32 							Bank18Size;					/* Number of bytes in  RAM bank 18*/
	UInt32 							Bank19Start;				/* Starting address of RAM bank 19*/
	UInt32 							Bank19Size;					/* Number of bytes in  RAM bank 19*/
	UInt32 							Bank20Start;				/* Starting address of RAM bank 20*/
	UInt32 							Bank20Size;					/* Number of bytes in  RAM bank 20*/
	UInt32 							Bank21Start;				/* Starting address of RAM bank 21*/
	UInt32 							Bank21Size;					/* Number of bytes in  RAM bank 21*/
	UInt32 							Bank22Start;				/* Starting address of RAM bank 22*/
	UInt32 							Bank22Size;					/* Number of bytes in  RAM bank 22*/
	UInt32 							Bank23Start;				/* Starting address of RAM bank 23*/
	UInt32 							Bank23Size;					/* Number of bytes in  RAM bank 23*/
	UInt32 							Bank24Start;				/* Starting address of RAM bank 24*/
	UInt32 							Bank24Size;					/* Number of bytes in  RAM bank 24*/
	UInt32 							Bank25Start;				/* Starting address of RAM bank 25*/
	UInt32 							Bank25Size;					/* Number of bytes in  RAM bank 25*/

																/*	Interrupt Support Data*/
	UInt32 							IntCntrBaseAddr;			/* Interrupt Controller Base Address  (variable is used since this is a PCI Dev and address is relocatable)*/
	UInt32 							IntPendingReg[2];			/* Data of current interrupts pending register*/

																/* These fields were added to report information about tightly-coupled L2 caches.*/
																/* The inline L2 information should be used in situations where there is a CPU*/
																/* card L2 cache that can coexist with a motherboard L2.*/

	UInt32 							InlineL2DSize;				/* Size of in-line L2 Dcache*/
	UInt32 							InlineL2ISize;				/* Size of in-line L2 Icache*/
	UInt16 							InlineL2Combined;			/* 1 <- combined or no cache, 0 <- split cache*/
	UInt16 							InlineL2IBlockSize;			/* Block size of in-line I L2 cache*/
	UInt16 							InlineL2DBlockSize;			/* Block size of in-line D L2 cache*/
	UInt16 							InlineL2IAssoc;				/* Associativity of L2 I*/
	UInt16 							InlineL2DAssoc;				/* Associativity of L2 D*/
	UInt16 							filler1;					/* pad*/

																/*	More Interrupt Support Data*/
	UInt32 							IntsCompleted[2];			/* completed interrupts*/

	UInt32 							filler2[6];					/* pad to nice cache block alignment*/
};
typedef struct NKSystemInfo				NKSystemInfo;

enum {
	kMaxBanks					= 26,							/* 6 banks, 0É25*/
	kSystemInfoVersion			= 0x0105						/* 1.0.5*/
};


/*
  _______________________________________________________________________
  	Diagnostic Info Record
  	Used to pass Diagnostic information from the power on Diagnostics to
  	the NanoKernel, and from the NanoKernel to user mode software.
  _______________________________________________________________________
*/


enum {
	nkDiagnosticInfoPtr			= 0x5FFFEFE8,					/* logical address of DiagnosticInfo record*/
	nkDiagnosticInfoVer			= 0x5FFFEFEC,					/* version number of DiagnosticInfo record*/
	nkDiagnosticInfoLen			= 0x5FFFEFEE					/* length of DiagnosticInfo record*/
};


struct NKDiagnosticInfo {
	UInt32 							BankMBFailOffset;			/* Mother Board RAM failure code*/
	UInt32 							BankAFailOffset;			/* Bank A RAM failure code*/
	UInt32 							BankBFailOffset;			/* Bank B RAM failure code*/
	UInt32 							BankCFailOffset;			/* Bank C RAM failure code*/

	UInt32 							BankDFailOffset;			/* Bank D RAM failure code*/
	UInt32 							BankEFailOffset;			/* Bank E RAM failure code*/
	UInt32 							BankFFailOffset;			/* Bank F RAM failure code*/
	UInt32 							BankGFailOffset;			/* Bank G RAM failure code*/

	UInt32 							BankHFailOffset;			/* Bank H RAM failure code*/
	UInt32 							CacheFailOffset;			/* cache failure code*/
	UInt32 							LongBootParamOffset;		/* on longBoot this is where the params will be*/
	UInt32 							POSTTraceOffset;			/* this tells us what route the POST took*/

	UInt32 							POSTOldWarmOffset;			/* logged address of old warmstart flag*/
	UInt32 							POSTOldLongOffset;			/* logged address of old long boot flag*/
	UInt32 							POSTOldGlobbOffset;			/* logged address of old Diagnostic Info Record*/
	UInt32 							POSTOldParamOffset;			/* the params from the old diag globb*/

	UInt32 							POSTStartRTCUOffset;		/* PPC Real Time Clock Upper at start of POST*/
	UInt32 							POSTStartRTCLOffset;		/* PPC Real Time Clock Lower at start of POST*/
	UInt32 							POSTEndRTCUOffset;			/* PPC Real Time Clock Upper at end of POST*/
	UInt32 							POSTEndRTCLOffset;			/* PPC Real Time Clock Lower at end of POST*/

	UInt32 							POSTTestTypeOffset;			/* when long RAM tests fail test type which failed is put here*/
	UInt32 							POSTError2Offset;			/* result codes from tests*/
	UInt32 							POSTError3Offset;			/* result codes from tests*/
	UInt32 							POSTError4Offset;			/* result codes from tests*/

	UInt8 							RegistersStore[140];		/* store all 60x registers here, still fit into 256 bytes size.*/

	UInt32 							DiagPOSTResult2;			/* POST results*/
	UInt32 							DiagPOSTResult1;			/* POST results*/
	UInt32 							DiagLongBootSig;			/* Burn in restart flag*/
	UInt32 							DiagWarmStartHigh;			/* First long of native warm start  (WLSC)		<SM44>*/
	UInt32 							DiagWarmStartLow;			/* Second long of native warm start (SamB)		<SM44>*/
};
typedef struct NKDiagnosticInfo			NKDiagnosticInfo;

enum {
	kDiagnosticInfoVersion		= 0x0100						/* 1.0.0*/
};


/*
  _______________________________________________________________________
  	NanoKernel Info Record
  	Used to pass NanoKernel statistics from the NanoKernel to user mode
  	software.
  _______________________________________________________________________
*/


enum {
	nkNanoKernelInfoPtr			= 0x5FFFEFE0,					/* logical address of NanoKernelInfo record*/
	nkNanoKernelInfoVer			= 0x5FFFEFE4,					/* version number of NanoKernelInfo record*/
	nkNanoKernelInfoLen			= 0x5FFFEFE6					/* length of NanoKernelInfo record*/
};


struct NKNanoKernelInfo {
	UInt32 							ExceptionCauseCounts[32];	/* counters per exception cause*/
	UInt32 							NanoKernelCallCounts[16];	/* counters per NanoKernel call*/
	UInt32 							ExternalIntCount;			/* count of External Interrupts*/
	UInt32 							MisalignmentCount;			/* count of Misalignment Interrupts*/
	UInt32 							FPUReloadCount;				/* count of FPU reloads on demand*/
	UInt32 							DecrementerIntCount;		/* count of Decrementer Interrupts*/
	UInt32 							QuietWriteCount;			/* count of Writes to Quiet Read-Only memory*/
	UInt32 							HashTableCreateCount;		/* count of Hash Table Entry creations*/
	UInt32 							HashTableDeleteCount;		/* count of Hash Table Entry deletions*/
	UInt32 							HashTableOverflowCount;		/* count of Hash Table Entry overflows*/
	UInt32 							EmulatedUnimpInstCount;		/* count of Emulated unimplemented instructions*/
	UInt32 							NCBPtrCacheMissCount;		/* count of NCB Pointer cache misses*/
	UInt32 							ExceptionPropagateCount;	/* count of Exceptions propagated to system*/
	UInt32 							ExceptionForcedCount;		/* count of Exceptions forced to system*/
	UInt64 							SysContextCpuTime;			/* CPU Time used by System Context*/
	UInt64 							AltContextCpuTime;			/* CPU Time used by Alternate Context*/

	UInt32 							blueProcessID;				/* ID of the blue process.*/
	UInt32 							blueTaskID;					/* ID of the blue task.*/
	UInt32 							pageQueueID;				/* ID of the page fault queue.*/
	UInt32 							TaskCount;					/* Number of tasks.*/
	UInt32 							FreePoolExtendCount;		/* Number of pages given to the nanokernel.*/
	UInt32 							rsrv1[3];
};
typedef struct NKNanoKernelInfo			NKNanoKernelInfo;

enum {
	kNanoKernelInfoVersion		= 0x0104						/* 1.0.4*/
};



/*
  _______________________________________________________________________
  	Processor Info Record
  	Used to pass Processor information from the NanoKernel to user mode
  	software.
  _______________________________________________________________________
*/


enum {
	nkProcessorInfoPtr			= 0x5FFFEFD8,					/* logical address of ProcessorInfo record*/
	nkProcessorInfoVer			= 0x5FFFEFDC,					/* version number of ProcessorInfo record*/
	nkProcessorInfoLen			= 0x5FFFEFDE					/* length of ProcessorInfo record*/
};


struct NKProcessorInfo {
	UInt32 							ProcessorVersionReg;		/* contents of the PVR special purpose register*/
	UInt32 							CpuClockRateHz;				/* CPU Clock frequency*/
	UInt32 							BusClockRateHz;				/* Bus Clock frequency*/
	UInt32 							DecClockRateHz;				/* Decrementer Clock frequency*/
	UInt32 							PageSize;					/* number of bytes in a memory page*/
	UInt32 							DataCacheTotalSize;			/* number of bytes in the Data Cache*/
	UInt32 							InstCacheTotalSize;			/* number of bytes in the Instruction Cache*/
	UInt16 							CoherencyBlockSize;			/* number of bytes in a Coherency Block*/
	UInt16 							ReservationGranuleSize;		/* number of bytes in a Reservation Granule*/
	UInt16 							CombinedCaches;				/* 1 <- combined or no cache, 0 <- split cache*/
	UInt16 							InstCacheLineSize;			/* number of bytes in a Line of the Instruction Cache*/
	UInt16 							DataCacheLineSize;			/* number of bytes in a Line of the Data Cache*/
	UInt16 							DataCacheBlockSizeTouch;	/* number of bytes in a Block for DCBT DCBTST*/
	UInt16 							InstCacheBlockSize;			/* number of bytes in a Block of the Instruction Cache*/
	UInt16 							DataCacheBlockSize;			/* number of bytes in a Block of the Data Cache*/
	UInt16 							InstCacheAssociativity;		/* Associativity of the Instruction Cache*/
	UInt16 							DataCacheAssociativity;		/* Associativity of the Data Cache*/

	UInt16 							TransCacheTotalSize;		/* number of entries in the Translation Cache*/
	UInt16 							TransCacheAssociativity;	/* Associativity of the Translation Cache*/

																/* These fields were added to report information about back-side L2 caches*/

	UInt32 							ProcessorL2DSize;			/* Size of back-side L2 Dcache*/
	UInt32 							ProcessorL2ISize;			/* Size of back-side L2 Icache*/
	UInt16 							ProcessorL2Combined;		/* 1 <- combined or no cache, 0 <- split cache*/
	UInt16 							ProcessorL2IBlockSize;		/* Block size of back-side I L2 cache*/
	UInt16 							ProcessorL2DBlockSize;		/* Block size of back-side D L2 cache*/
	UInt16 							ProcessorL2IAssoc;			/* Associativity of L2 I*/
	UInt16 							ProcessorL2DAssoc;			/* Associativity of L2 D*/

	UInt16 							filler1;					/* align to long*/

	UInt32 							ProcessorFlags;				/* flags to specify processor features*/
	UInt32 							filler2[5];					/* pad to nice cache block alignment*/
};
typedef struct NKProcessorInfo			NKProcessorInfo;
/*
   ProcessorFlags - Definitions for the processor flags field. These are bit positions,
  					as in Ò1 << hasVMXÓ, and not masks.
*/

enum {
	hasL2CR						= 0,
	hasPLRUL1					= 1,
	hasTAU						= 2,
	hasVMX						= 3
};



enum {
	kProcessorInfoVersion		= 0x0101						/* 1.0.1*/
};


/*
  _______________________________________________________________________
  	Hardware Info Record
  	Used to pass hardware information from the NanoKernel to user mode
  	software.
  _______________________________________________________________________
*/


enum {
	nkHWInfoPtr					= 0x5FFFEFD0,					/* logical address of HWInfo record*/
	nkHWInfoVer					= 0x5FFFEFD4,					/* version number of HWInfo record*/
	nkHWInfoLen					= 0x5FFFEFD6					/* length of HWInfo record*/
};


enum {
	nkHWInfoFlagSlowMESH		= 1,							/* set if fast MESH doesn't work on this box*/
	nkHWInfoFlagAsynchMESH		= 2,							/* set if Synchronous MESH doesn't work on this box*/
	nkHWInfoFlagNoCopySWTLB		= 4								/* set if the software TLB walk code for 603 should NOT be copied*/
};


struct NKHWInfo {
	UInt32 							MacROM_Base;				/* base address (physical) of Mac ROM*/
	UInt32 							DeviceTreeBase;				/* base address of the copied device tree properties*/
	UInt32 							UniversalInfoTableBase;		/* base address of the Universal Info Table*/
	UInt32 							ConfigInfoTableBase;		/* base address of the Config Info Table*/
	UInt16 *						VectorLookupTable;			/* base address of the interrupt vector lookup table*/
	UInt32 *						VectorMaskTable;			/* base address of the interrupt vector mask table*/

	UInt32 							OpenPICBaseAddr;			/* OpenPIC base address*/

	UInt8 *							ISAMaster8259;				/* ISA Master 8259 ports*/
	UInt8 *							ISASlave8259;				/* ISA Slave 8259 ports*/
	UInt32 *						InterruptAck8259;			/* address to read to ack 8259 interrupt*/

																/* interrupt pending bits (actively changing)*/

	UInt32 							PendingInts[2];				/* 64 bits of pending interrupts*/

																/* some Mac I/O device base addresses*/

	UInt32 							ADB_Base;					/* base address of ADB*/
	UInt32 							SCSI_DMA_Base;				/* base address of SCSI DMA registers*/

																/* RTAS related stuff*/

	UInt32 							RTAS_PrivDataArea;			/* RTAS private data area */
	UInt32 							MacOS_NVRAM_Offset;			/* offset into nvram to MacOS data*/

	UInt32 							RTAS_NVRAM_Fetch;			/* token for RTAS NVRAM fetch*/
	UInt32 							RTAS_NVRAM_Store;			/* token for RTAS NVRAM store*/
	UInt32 							RTAS_Get_Clock;				/* token for RTAS clock get*/
	UInt32 							RTAS_Set_Clock;				/* token for RTAS clock set*/
	UInt32 							RTAS_Restart;				/* token for RTAS Restart*/
	UInt32 							RTAS_Shutdown;				/* token for RTAS Shutdown*/
	UInt32 							RTAS_Restart_At;			/* token for RTAS system startup at specified time*/
	UInt32 							RTAS_EventScan;				/* token for RTAS event scan*/
	UInt32 							RTAS_Check_Exception;		/* token for RTAS check exception*/
	UInt32 							RTAS_Read_PCI_Config;		/* token for RTAS read PCI config*/
	UInt32 							RTAS_Write_PCI_Config;		/* token for RTAS write PCI config*/

																/* SIO interrupt source numbers for the MPIC*/

	SInt16 							SIOIntVect;					/* SIO (8259 cascade vector) vector number*/
	SInt16 							SIOIntBit;					/* SIO (8259 cascade vector) bit number*/

	SInt32 							Signature;					/* signature for this record ('Hnfo')*/

																/* more interrupt source numbers*/

	SInt16 							SpuriousIntVect;			/* spurious vector number*/

	SInt16 							CPU_ID;						/* the ID of this CPU (universal-tables-related)*/

	SInt16 							SCCAIntVect;				/* SCC A (non-DMA) vector number*/
	SInt16 							SCCBIntVect;				/* SCC B (non-DMA) vector number*/
	SInt16 							SCSIIntVect;				/* SCSI vector number*/
	SInt16 							SCSIDMAIntVect;				/* SCSI DMA vector number*/
	SInt16 							VIAIntVect;					/* VIA vector number*/
	SInt16 							VIAIntBit;					/* VIA bit number*/
	SInt16 							ADBIntVect;					/* ADB vector number*/
	SInt16 							NMIIntVect;					/* NMI vector number*/
	SInt16 							NMIIntBit;					/* NMI bit number*/

																/* current (actively changing) interrupt handling variables*/

	SInt16 							ISAPendingInt;				/* currently pending ISA/8259 interrupt*/
	SInt8 							CompletedInts[8];			/* completed interrupts*/

	SInt32 							HardwareInfoFlags;			/* 32 bits of flags (see enum above)*/

	UInt32 							RTAS_Get_PowerOn_Time;		/* token for RTAS getting time for system startup*/

	UInt32 							filler1[1];					/* pad to cache block alignment*/
};
typedef struct NKHWInfo					NKHWInfo;

enum {
	kHWInfoVersion				= 0x0103						/* 1.0.3*/
};


/*
  _______________________________________________________________________
  	Processor State Record
  	Used to save the state of the processor across sleep.
  _______________________________________________________________________
*/


enum {
	nkProcessorStatePtr			= 0x5FFFEFC8,					/* logical address of ProcessorState record*/
	nkProcessorStateVer			= 0x5FFFEFCC,					/* version number of ProcessorState record*/
	nkProcessorStateLen			= 0x5FFFEFCE					/* length of ProcessorState record*/
};


struct NKProcessorState {
	UInt32 							saveDBAT0u;					/* place to store DBAT0U*/
	UInt32 							saveDBAT0l;					/* place to store DBAT0L*/
	UInt32 							saveDBAT1u;					/* place to store DBAT1U*/
	UInt32 							saveDBAT1l;					/* place to store DBAT1L*/
	UInt32 							saveDBAT2u;					/* place to store DBAT2U*/
	UInt32 							saveDBAT2l;					/* place to store DBAT2L*/
	UInt32 							saveDBAT3u;					/* place to store DBAT3U*/
	UInt32 							saveDBAT3l;					/* place to store DBAT3L*/

	UInt32 							saveIBAT0u;					/* place to store IBAT0U*/
	UInt32 							saveIBAT0l;					/* place to store IBAT0L*/
	UInt32 							saveIBAT1u;					/* place to store IBAT1U*/
	UInt32 							saveIBAT1l;					/* place to store IBAT1L*/
	UInt32 							saveIBAT2u;					/* place to store IBAT2U*/
	UInt32 							saveIBAT2l;					/* place to store IBAT2L*/
	UInt32 							saveIBAT3u;					/* place to store IBAT3U*/
	UInt32 							saveIBAT3l;					/* place to store IBAT3L*/

	UInt32 							saveSPRG0;					/* place to store SPRG0*/
	UInt32 							saveSPRG1;					/* place to store SPRG1*/
	UInt32 							saveSPRG2;					/* place to store SPRG2*/
	UInt32 							saveSPRG3;					/* place to store SPRG3*/

	UInt32 							saveL2CR;					/* place to store Arthur's L2CR*/

	UInt32 							saveSRR0;					/* place to store SRR0*/
	UInt32 							saveSRR1;					/* place to store SRR1*/
	UInt32 							saveTBU;					/* place to store TBU*/
	UInt32 							saveTBL;					/* place to store TBL*/
	UInt32 							saveHID0;					/* place to store HID0*/
	UInt32 							saveDEC;					/* place to store DEC*/
	UInt32 							saveMSR;					/* place to store MSR*/
	UInt32 							saveSDR1;					/* place to store SDR1*/

																/* saveKernelDataPtr needs to always be right after saveReturnAddr*/
																/* because of how the code works.  DO NOT CHANGE THIS ORDERING!*/

	UInt32 							saveReturnAddr;				/* place to store the addr to jump to.*/
	UInt32 							saveKernelDataPtr;			/* place to store the KernelDataPtr*/
	UInt32 							saveContextPtr;				/* place to store the ContextPtr*/
};
typedef struct NKProcessorState			NKProcessorState;

enum {
	kProcessorStateVersion		= 0x0100						/* 1.0.0*/
};


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

#endif /* __PPCINFORECORDSPRIV__ */

