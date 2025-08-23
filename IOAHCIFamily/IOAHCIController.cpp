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

#include <IOKit/ahci/IOAHCIController.h>

const OSSymbol *gIOAHCIPortTypeKey;
const OSSymbol *gIOAHCIHostCapabilitiesKey;

class IOAHCIControllerGlobals {
public:
    IOAHCIControllerGlobals();
    ~IOAHCIControllerGlobals();
};

static IOAHCIControllerGlobals IOAHCIControllerGlobals;

IOAHCIControllerGlobals::IOAHCIControllerGlobals()
{
    gIOAHCIHostCapabilitiesKey = OSSymbol::withCString("AHCI-CAP");
    gIOAHCIPortTypeKey = OSSymbol::withCString("AHCI Port Type");
}

IOAHCIControllerGlobals::~IOAHCIControllerGlobals()
{
    OSSafeReleaseNULL(gIOAHCIPortTypeKey);
}



#define AHCI_DEBUG(fmt) kprintf("[AHCI][IOAHCIController]: %s: " fmt, __PRETTY_FUNCTION__)
#define AHCI_DEBUG_VA(fmt, ...) kprintf("[AHCI][IOAHCIController]: %s: " fmt, __PRETTY_FUNCTION__, __VA_ARGS__)

OSDefineMetaClassAndAbstractStructors(IOAHCIController, IOService);

#define super IOService

bool IOAHCIController::start(IOService *provider)
{
    UInt32 caps;
    OSNumber *number;
    
    if (super::start(provider) == false) {
        AHCI_DEBUG("Superclass start failed!\n");
        return false;
    }
    
    this->_registerLock = IOSimpleLockAlloc();

    caps = this->readRegister(kIOAHCIRegHostCapabilities);

    /* for debugging purposes */
    AHCI_DEBUG("AHCI Capabilities:\n");
    AHCI_DEBUG_VA("\t Supported Ports                : %d", caps & kIOAHCIHostCapabilitiesPortCountMask);
    AHCI_DEBUG_VA("\t Supports External SATA         : %d", ((caps & kIOAHCIHostCapabilitiesExternalSATA) != 0));
    AHCI_DEBUG_VA("\t Supports Enclosure Management  : %d", ((caps & kIOAHCIHostCapabilitiesEnclosureManagement) != 0));
    AHCI_DEBUG_VA("\t Supports Completion Coalescing : %d", ((caps & kIOAHCIHostCapabilitiesCmdCompletionCoalescing) != 0));
    AHCI_DEBUG_VA("\t Number of Command Slots        : %d", ((caps & kIOAHCIHostCapabilitiesNumCommandSlotsMask) >> 8));

    number = OSNumber::withNumber(caps, 32);

    this->setProperty(gIOAHCIHostCapabilitiesKey, number);
    
    OSSafeReleaseNULL(number);

    /* Enumerate the available ports here */
#if 0
    UInt32 ports = this->readRegister(kIOAHCIRegPortsImplemented);
    for (int i = 0; i < kIOAHCIMaximumPorts; i++) {
        if ((ports >> i) & 0x1) {
            AHCI_DEBUG_VA("Creating port %d", i);
            auto port = this->createPort(i);
        }
    }
#endif
    /* ^ disabled the above for now - until Port infrastructure is developed. */

    return true;
}
