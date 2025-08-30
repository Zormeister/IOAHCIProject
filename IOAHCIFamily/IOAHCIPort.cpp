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

#include <AssertMacros.h>
#include "IOAHCIFamilyDebug.h"
#include <IOKit/ahci/IOAHCIPort.h>
#include <IOKit/IOBufferMemoryDescriptor.h>

#define TRACE_BEGIN(func, a, b, c, d) IOAHCITraceBegin(Port, func, a, b, c, d)
#define TRACE_END(func, a, b, c, d) IOAHCITraceEnd(Port, func, a, b, c, d)
#define DBG(fmt, args...) IOAHCIDebugLog(Port, fmt, ##args)

OSDefineMetaClassAndAbstractStructors(IOAHCIPort, IOService);

#define super IOService

bool IOAHCIPort::initWithControllerAndPortNumber(IOAHCIController *controller, UInt32 portNumber)
{
    TRACE_BEGIN(Init, controller, portNumber, getMetaClass()->getInstanceCount(), 0);
    
    if (!super::init()) {
        DBG("Superclass failed to initialize!");
        TRACE_END(Init, controller, portNumber, getMetaClass()->getInstanceCount(), 0);
        return false;
    }
    
    if (controller == nullptr) {
        DBG("No controller?!?.");
        TRACE_END(Init, controller, portNumber, getMetaClass()->getInstanceCount(), 1);
        return false;
    }
    
    DBG("Port %d is now initialising.", portNumber);
    
    fController = controller;
    fPortNumber = portNumber;

    initCapabilities();
    
    fPortMemoryBase = (80 * portNumber);
    
    /* AHCI requires that the command list be aligned on a 1K address. */
    fCommandListBuffer = IOBufferMemoryDescriptor::inTaskWithPhysicalMask(kernel_task, kIOMapInhibitCache, 1024, fCommandListAddrMask);

    /* Allocate the incoming FIS buffer */
    fIncomingFISBuffer = IOBufferMemoryDescriptor::inTaskWithPhysicalMask(kernel_task, kIOMapInhibitCache, 256, fCommandTableAddrMask);

    DBG("Command List address: 0x%llX", fCommandListBuffer->getPhysicalAddress());
    DBG("Incoming FIS address: 0x%llX", fIncomingFISBuffer->getPhysicalAddress());

    TRACE_END(Init, controller, portNumber, getMetaClass()->getInstanceCount(), 2);
    return true;
}

/* Is there legitimately any way to use AssertMacros without the code looking ugly? */
void IOAHCIPort::initCapabilities()
{
    TRACE_BEGIN(InitCaps, fController, fPortNumber, getMetaClass()->getInstanceCount(), 0);
    OSNumber *caps = OSDynamicCast(OSNumber, fController->getProperty(gIOAHCIHostCapabilitiesKey));
    UInt32 raw = caps->unsigned32BitValue();

    if (raw & kIOAHCIHostCapabilitiesSupports64BitAddressing) {
        fIs64bit = true;
        fCommandListAddrMask = kIOAHCICommandListAddressMask64;
        fCommandTableAddrMask = kIOAHCICommandTableAddressMask64;
    } else {
        fIs64bit = false;
        fCommandListAddrMask = kIOAHCICommandListAddressMask32;
        fCommandTableAddrMask = kIOAHCICommandTableAddressMask32;
    }
    
    if (raw & kIOAHCIHostCapabilitiesSupportsNativeCmdQueueing) {
        fUsesNCQ = true;
    }
    
    TRACE_END(InitCaps, fController, fPortNumber, getMetaClass()->getInstanceCount(), 1);
}

UInt32 IOAHCIPort::readRegister(UInt32 reg)
{
    return fController->readRegister((fPortMemoryBase + reg));
}

void IOAHCIPort::writeRegister(UInt32 reg, UInt32 value)
{
    fController->writeRegister((fPortMemoryBase + reg), value);
}

