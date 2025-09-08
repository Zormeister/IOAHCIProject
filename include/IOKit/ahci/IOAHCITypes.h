/*
 * Copyright (c) 2007-Present The PureDarwin Project.
 * All rights reserved.
 *
 * @LICENSE_HEADER_START@
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * @LICENSE_HEADER_END@
 */

#ifndef _IOKIT_AHCI_IOAHCITYPES_H_
#define _IOKIT_AHCI_IOAHCITYPES_H_

#include <IOKit/IOTypes.h>
#include <libkern/c++/OSSymbol.h>

#ifndef sub_iokit_ahci
#define sub_iokit_ahci err_sub(12)
#endif

/* #define iokit_family_msg(sub, message)      (UInt32)(sys_iokit|sub|message) */
#define ioahci_family_msg(msg) iokit_family_msg(sub_iokit_ahci, msg)

extern const OSSymbol *gIOAHCIPortTypeKey;
extern const OSSymbol *gIOAHCIHostCapabilitiesKey;

/*!
 * @const kIOAHCIMessageHBAHasBeenReset
 * Indicates that the HBA has, infact, been reset.
 *
 * This should be used by IOAHCIPort to reset itself and re-initialize after the HBA has been reset.
 */
#define kIOAHCIMessageHBAHasBeenReset ioahci_family_msg(0x1)

/*!
 * @const kIOAHCIMaximumPorts
 * The maximum number of ports an AHCI controller can have.
 */
enum {
    kIOAHCIMaximumPorts = 32,
};

enum {
    kIOAHCIPortMaxCommandListEntries = 32,
};

/*!
 * @const kIOAHCIPCICapabilityID
 * The PCI capability ID of the SATA capability
 */
enum {
    kIOAHCIPCICapabilityID = 0x12,
};

#define kIOAHCICommandListAddressMask64 0xFFFFFFFFFFFFFC00
#define kIOAHCICommandListAddressMask32 0x00000000FFFFFC00

#define kIOAHCICommandTableAddressMask64 0xFFFFFFFFFFFFFF00
#define kIOAHCICommandTableAddressMask32 0x00000000FFFFFF00

// 2.4 - Serial ATA Capability
struct IOAHCIPCICapabilityRegister {
    // SATACR0
    union {
        struct {
            UInt8 capabilityID;
            UInt16 nextCapability;
            UInt8 minorRev:4;
            UInt8 majorRev:4;
            UInt16 reserved1;
            // SATACR1
            UInt8 barLocation:4;
            UInt32 offset:20;
            UInt8 reserved2;
        } bits;
        struct {
            UInt32 SATACR0;
            UInt32 SATACR1;
        } regs;
    };
};

// 2.4.2 - SATACR1
enum {
    kIOAHCICapabilityBARLocation0 = 0x4,
    kIOAHCICapabilityBARLocation1 = 0x5,
    kIOAHCICapabilityBARLocation2 = 0x6,
    kIOAHCICapabilityBARLocation3 = 0x7,
    kIOAHCICapabilityBARLocation4 = 0x8,
    kIOAHCICapabilityBARLocation5 = 0x9,
    kIOAHCICapabilityBARLocationPCIConfig = 0xF,
};

#pragma mark - AHCI specification registers and bit fields

#pragma mark - 0x0000 - 0x0100

// 3 - HBA Memory Registers
enum {
    kIOAHCIPortControlBase = 0x100,
    
    kIOAHCIPortControlSize = 0x80,
};

// 3.1 - Generic Host Control
enum {
    kIOAHCIRegHostCapabilities               = 0x00,
    kIOAHCIRegHostControl                    = 0x04,
    kIOAHCIRegInterruptStatus                = 0x08,
    kIOAHCIRegPortsImplemented               = 0x0C,
    kIOAHCIRegVersion                        = 0x10,
    kIOAHCIRegCmdCompletionCoalescingControl = 0x14,
    kIOAHCIRegCmdCompletionCoalescingPorts   = 0x18,
    kIOAHCIRegEnclosureManagementLocation    = 0x1C,
    kIOAHCIRegEnclosureManagementControl     = 0x20,
    kIOAHCIRegExtendedHostCapabilities       = 0x24,
    kIOAHCIRegHandoffControlAndStatus        = 0x28,
};

// 3.1.1 - HBA Capabilities
enum {
    kIOAHCIHostCapabilitiesPortCountMask = 0x1F,
    kIOAHCIHostCapabilitiesExternalSATA = 0x20,
    kIOAHCIHostCapabilitiesEnclosureManagement = 0x40,
    kIOAHCIHostCapabilitiesCmdCompletionCoalescing = 0x80,
    kIOAHCIHostCapabilitiesNumCommandSlotsMask = 0x1F80, // 12:8
    kIOAHCIHostCapabilitiesPartialStateCapable = 0x2000,
    kIOAHCIHostCapabilitiesSlumberStateCapable = 0x4000,
    kIOAHCIHostCapabilitiesPIOMultipleDRQBlock = 0x8000,
    kIOAHCIHostCapabilitiesFISBasedSwitchingSupported = 0x10000,
    kIOAHCIHostCapabilitiesPortMultiplierSupported = 0x20000,
    kIOAHCIHostCapabilitiesAHCIModeOnly = 0x40000,
    kIOAHCIHostCapabilitiesInterfaceSpeedMask = 0xF80000, // 23:20
    kIOAHCIHostCapabilitiesCommandListOverrideSupported = 0x1000000,
    kIOAHCIHostCapabilitiesActivityLEDSupported = 0x2000000,
    kIOAHCIHostCapabilitiesSupportsALPM = 0x4000000,
    kIOAHCIHostCapabilitiesSupportsStaggeredSpinUp = 0x8000000,
    kIOAHCIHostCapabilitiesSupportsMechanicalPresenceSwitch = 0x10000000,
    kIOAHCIHostCapabilitiesSupportsSNotificationRegister = 0x20000000,
    kIOAHCIHostCapabilitiesSupportsNativeCmdQueueing = 0x40000000,
    kIOAHCIHostCapabilitiesSupports64BitAddressing = 0x80000000,
};

// 3.1.2 - Global HBA Control
enum {
    kIOAHCIHostControlHBAReset     = 0x00000001,
    kIOAHCIHostControlIRQEnable    = 0x00000002,
    kIOAHCIHostControlMSISingleMsg = 0x00000004,
    kIOAHCIHostControlAHCIEnable   = 0x80000000,
};

// 3.1.5 - AHCI Version
enum {
    kIOAHCIVersion0_95 = 0x00000905,
    kIOAHCIVersion1_0  = 0x00010000,
    kIOAHCIVersion1_1  = 0x00010100,
    kIOAHCIVersion1_2  = 0x00010200,
    kIOAHCIVersion1_3  = 0x00010300,
    kIOAHCIVersion1_31 = 0x00010301,
};

// 3.1.6 - Command Completion Coalescing Control
enum {
    kIOAHCICCCControlEnable = 0x00000001,
    kIOAHCICCCControlInterruptMask = 0x000000F8,
    kIOAHCICCCControlCompletionsMask = 0x0000FF00,
    kIOAHCICCCControlTimeoutValueMask = 0xFFFF0000,
};

// 3.1.8 - Enclosure Management Location
enum {
    kIOAHCIEMLocationBufferSizeMask = 0x0000FFFF,
    kIOAHCIEMLocationOffsetMask = 0xFFFF0000,
};

// 3.1.9 - Enclosure Management Control
enum {
    kIOAHCIEMControlMessageRecieved = 0x00000001,
    kIOAHCIEMControlTransmitMessage = 0x00000100,
    kIOAHCIEMControlReset = 0x00000200,
    kIOAHCIEMControlSupportsLEDMessages = 0x00010000,
    kIOAHCIEMControlSupportsSAFTEMessages = 0x00020000,
    kIOAHCIEMControlSupportsSES2Messages = 0x00040000,
    kIOAHCIEMControlSupportsSGPIOMessages = 0x00080000,
    kIOAHCIEMControlSingleMessageBuffer = 0x001000000,
    kIOAHCIEMControlTransmitOnly = 0x002000000,
    kIOAHCIEMControlLEDHardwareDriven = 0x004000000,
    kIOAHCIEMControlPortMultiplierSupport = 0x008000000,
};

// 3.1.10 -  HBA Capabilities Extended
enum {
    kIOAHCIExtentedHostCapsBIOSHandoff = 0x00000001,
    kIOAHCIExtentedHostCapsNVMHCIPresent = 0x00000002,
    kIOAHCIExtentedHostCapsAutoPartialToSlumber = 0x00000004,
    kIOAHCIExtentedHostCapsSupportsDevSleep = 0x00000008,
    kIOAHCIExtentedHostCapsSupportsAggressiveDevSleep = 0x00000010,
    kIOAHCIExtentedHostCapsDevSleepFromSlumberOnly = 0x00000020,
};

// 3.1.11 BIOS/OS Handoff Control and Status
enum {
    kIOAHCIHandoffControlBIOSOwned = 0x00000001,
    kIOAHCIHandoffControlOSOwned = 0x00000002,
    kIOAHCIHandoffControlSMIOnChange = 0x00000004,
    kIOAHCIHandoffControlOwnershipChange = 0x00000008,
    kIOAHCIHandoffControlBIOSBusy = 0x00000010,
};


#pragma mark - Per port registers

// 3.3 - Port Registers (one set per port)
enum {
    kIOAHCIPortRegCommandListAddress = 0x00,
    kIOAHCIPortRegCommandListAddressHigh = 0x04,
    kIOAHCIPortRegFISAddress = 0x08,
    kIOAHCIPortRegFISAddressHigh = 0x0C,
    kIOAHCIPortRegIRQStatus = 0x10,
    kIOAHCIPortRegIRQEnable = 0x14,
    kIOAHCIPortRegCommandAndStatus = 0x18,
    kIOAHCIPortRegTaskFileData = 0x20,
    kIOAHCIPortRegSignature = 0x24,
    kIOAHCIPortRegSATAStatus = 0x28,
    kIOAHCIPortRegSATAControl = 0x2C,
    kIOAHCIPortRegSATAError = 0x30,
    kIOAHCIPortRegSATAActive = 0x34,
    kIOAHCIPortRegCommandIssue = 0x38,
    kIOAHCIPortRegSATANotification = 0x3C,
    kIOAHCIPortRegFISBaseSwitchingControl = 0x40,
    kIOAHCIPortRegDeviceSleep = 0x44,
};

enum {
    kIOAHCIPortIRQStatusDHRS = 0x00000001,
};

enum {
    kIOAHCISignatureSATA = 0x00000101,
    kIOAHCISignatureATAPI = 0xEB140101,
    kIOAHCISignatureEnclosureManagement = 0xC33C0101,
    kIOAHCISignaturePortMultiplier = 0x96690101,
};

#endif /* _IOKIT_AHCI_IOAHCITYPES_H_ */
