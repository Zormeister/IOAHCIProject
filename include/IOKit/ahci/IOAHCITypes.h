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

#ifndef sub_iokit_ahci
#define sub_iokit_ahci err_sub(12)
#endif

/*!
 * @const kIOAHCIMaximumPorts
 * The maximum number of ports an AHCI controller can have.
 */
enum {
    kIOAHCIMaximumPorts = 32,
};

/*!
 * @const kIOAHCIPCICapabilityID
 * The PCI capability ID of the SATA capability
 */
enum {
    kIOAHCIPCICapabilityID = 0x12,
};

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

#endif /* _IOKIT_AHCI_IOAHCITYPES_H_ */
