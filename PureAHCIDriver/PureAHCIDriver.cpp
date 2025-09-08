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

#include <IOKit/IOFilterInterruptEventSource.h>
#include "PureAHCIDriver.h"

OSDefineMetaClassAndStructors(PureAHCIDriver, IOAHCIController);

#define super IOAHCIController

IOService *PureAHCIDriver::probe(IOService *provider, SInt32 *score)
{
    UInt8 capabilityOffset;
    IOAHCIPCICapabilityRegister capabilityReg;
    UInt32 bar = kIOPCIConfigBaseAddress5;
    int irqType;
    int irqSrc = 0;
    
    if (super::probe(provider, score) == NULL) {
        return NULL;
    }
    
    fMemoryOffset = 0;
    
    fPCIDevice = OSDynamicCast(IOPCIDevice, provider);

    fPCIDevice->findPCICapability(kIOAHCIPCICapabilityID, &capabilityOffset);

    if (capabilityOffset) {
        capabilityReg.regs.SATACR0 = this->fPCIDevice->configRead32(capabilityOffset);
        capabilityReg.regs.SATACR1 = this->fPCIDevice->configRead32(capabilityOffset + 0x4);

        /* ZORMEISTER: Why does the AHCI specification even allow this? */
        if (capabilityReg.bits.barLocation == kIOAHCICapabilityBARLocationPCIConfig) {
            fMemoryInPCIConfig = true;
        }
        
        bar = (capabilityReg.bits.barLocation * 4);
        
        fMemoryOffset = capabilityReg.bits.offset * 4;
    }
    
    if (!fMemoryInPCIConfig) {
        fMMIOMap = fPCIDevice->mapDeviceMemoryWithRegister(bar, kIOMapInhibitCache);
        fMMIODescriptor = fMMIOMap->getMemoryDescriptor();
    }
    
    /* Locate the MSI interrupt if it's available */
    for (int i = 0; ; i++) {
        if (fPCIDevice->getInterruptType(i, &irqType) == kIOReturnSuccess) {
            if ((irqType & kIOInterruptTypePCIMessaged) || (irqType & kIOInterruptTypePCIMessagedX)) {
                irqSrc = i;
            }
        } else {
            break;
        }
    }
    
    /* Register our IRQ handler. */
    fIRQEventSource = IOFilterInterruptEventSource::filterInterruptEventSource(this,
                                                        OSMemberFunctionCast(IOInterruptEventAction,
                                                                             this,
                                                                             &super::handleInterrupt),
                                                        OSMemberFunctionCast(IOFilterInterruptAction,
                                                                             this,
                                                                             &super::filterInterrupt),
                                                        fPCIDevice,
                                                        irqSrc);
    /* If ANYBODY wants to make the above lines cleaner feel free to do so. */
    
    return this;
}

bool PureAHCIDriver::start(IOService *provider)
{
    if (!super::start(provider)) {
        return false;
    }
    
    return true;
}

UInt32 PureAHCIDriver::readRegister(UInt32 reg)
{
    UInt32 value;
    
    IOSimpleLockLock(this->fRegisterLock);
    
    if (fMemoryInPCIConfig) {
        /* "Index-Data Pair is implemented in Dwords directly following SATACR1 in the PCI configuration space" */
        value = this->fPCIDevice->configRead32(((this->fSATACapabilityOffset + 4) + fMemoryOffset + reg));
    } else {
        this->fMMIODescriptor->readBytes((this->fMemoryOffset + reg), &value, sizeof(UInt32));
    }
    
    IOSimpleLockUnlock(this->fRegisterLock);
    
    return value;
}

void PureAHCIDriver::writeRegister(UInt32 reg, UInt32 value)
{
    IOSimpleLockLock(this->fRegisterLock);
    
    if (fMemoryInPCIConfig) {
        /* "Index-Data Pair is implemented in Dwords directly following SATACR1 in the PCI configuration space" */
        this->fPCIDevice->configWrite32(((this->fSATACapabilityOffset + 4) + fMemoryOffset + reg), value);
    } else {
        this->fMMIODescriptor->writeBytes((this->fMemoryOffset + reg), &value, sizeof(UInt32));
    }
    
    IOSimpleLockUnlock(this->fRegisterLock);
}

IOAHCIPort *PureAHCIDriver::createPort(UInt32 number) {
    
    /*
     * Pseudocode:
     *
     * port = OSTypeAlloc(PureAHCIPort)
     * port->attach(this)
     * port->start(this)
     *
     */
    
    return NULL;
}
