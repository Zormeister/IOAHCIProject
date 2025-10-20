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

#include "IOAHCIFamilyDebug.h"
#include <IOKit/ahci/IOAHCIController.h>
#include <IOKit/ahci/IOAHCIPort.h>

#define TRACE_BEGIN(func, a, b, c, d) IOAHCITraceBegin(Controller, func, a, b, c, d)
#define TRACE_END(func, a, b, c, d) IOAHCITraceEnd(Controller, func, a, b, c, d)
#define DBG(fmt, args...) IOAHCIDebugLog(Controller, fmt, ##args)
#define LOG(fmt, args...) IOAHCILog(Controller, fmt, ##args)

OSDefineMetaClassAndAbstractStructors(IOAHCIController, IOService);

#define super IOService

bool IOAHCIController::start(IOService *provider)
{
    UInt32 caps;
    UInt32 cntl;
    OSNumber *number;
    TRACE_BEGIN(Start, provider, this->getMetaClass()->getInstanceCount(), 0, 0);

    if (super::start(provider) == false) {
        DBG("Superclass start failed!");
        return false;
    }

    fRegisterLock = IOSimpleLockAlloc();
    fPortArray = OSArray::withCapacity(kIOAHCIMaximumPorts);

    cntl = readRegister(kIOAHCIRegHostControl);

    if (!(cntl & kIOAHCIHostControlAHCIEnable)) {
        DBG("Enabling AHCI mode!");

        cntl |= kIOAHCIHostControlAHCIEnable;

        writeRegister(kIOAHCIRegHostControl, cntl);
    }

    caps = readRegister(kIOAHCIRegHostCapabilities);

    /* for debugging purposes */
    DBG("AHCI Capabilities:");
    DBG("\t Supported Ports                : %d", caps & kIOAHCIHostCapabilitiesPortCountMask);
    DBG("\t Supports External SATA         : %d", ((caps & kIOAHCIHostCapabilitiesExternalSATA) != 0));
    DBG("\t Supports Enclosure Management  : %d", ((caps & kIOAHCIHostCapabilitiesEnclosureManagement) != 0));
    DBG("\t Supports Completion Coalescing : %d", ((caps & kIOAHCIHostCapabilitiesCmdCompletionCoalescing) != 0));
    DBG("\t Number of Command Slots        : %d", ((caps & kIOAHCIHostCapabilitiesNumCommandSlotsMask) >> 8));

    number = OSNumber::withNumber(caps, 32);

    this->setProperty(gIOAHCIHostCapabilitiesKey, number);
    
    OSSafeReleaseNULL(number);

    /* Enumerate the available ports here */
    UInt32 ports = this->readRegister(kIOAHCIRegPortsImplemented);
    for (int i = 0; i < kIOAHCIMaximumPorts; i++) {
        if ((ports >> i) & 0x1) {
            DBG("Creating port %d", i);
            auto port = this->createPort(i);
        }
    }
    /* ^ disabled the above for now - until Port infrastructure is developed. */

    TRACE_END(Start, provider, this->getMetaClass()->getInstanceCount(), 0, 0);
    return true;
}

//
// This performs a HBA reset, as per section 10.4.3 of the AHCI specification.
//
void IOAHCIController::reset()
{
    /* The AHCI spec specifies that a single second is the maximum limit before we declare the HBA hung. */
    int timeout = 1000;
    
    UInt32 ghc = readRegister(kIOAHCIRegHostControl);
    ghc |= kIOAHCIHostControlHBAReset;
    writeRegister(kIOAHCIRegHostControl, ghc);

    while ((readRegister(kIOAHCIRegHostControl) & kIOAHCIHostControlHBAReset)) {
        if (timeout == 0) {
            panic("HBA is hung and we don't know how to recover.\n");
        }
        IOSleep(1);
        timeout--;
    }

    /* Hopefully should message the IOAHCIPort classes? */
    messageClients(kIOAHCIMessageHBAHasBeenReset);
}

bool IOAHCIController::filterInterrupt(IOFilterInterruptEventSource *sender)
{
    if (readRegister(kIOAHCIRegInterruptStatus) == 0) {
        return false;
    } else {
        return true;
    }
}

void IOAHCIController::handleInterrupt(IOInterruptEventSource *sender, int count)
{
    UInt32 irq = readRegister(kIOAHCIRegInterruptStatus);
    UInt32 portNum = 0;
    while (portNum < fPortArray->getCount()) {
        /*
         * This assumes that we've already initialised ourself; which should be the case if we're dispatching IRQs.
         */
        if ((irq =>> 1) & 0x1) {
            IOAHCIPort *port = OSDynamicCast(IOAHCIPort, fPortArray->getObject(portNum));
            port->handleInterrupt();
        }
        portNum++;
    }
}
