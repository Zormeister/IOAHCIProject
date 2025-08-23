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

#include "PureAHCIDriver.h"

OSDefineMetaClassAndStructors(PureAHCIDriver, IOAHCIController);

#define super IOAHCIController

IOService *PureAHCIDriver::probe(IOService *provider, SInt32 *score)
{
    UInt8 capabilityOffset;
    IOAHCIPCICapabilityRegister capabilityReg;
    UInt32 bar = kIOPCIConfigBaseAddress5;
    
    if (super::probe(provider, score) == NULL) {
        return NULL;
    }
    
    this->_memoryOffset = 0;
    
    this->_pciDevice = OSDynamicCast(IOPCIDevice, provider);
    
    this->_pciDevice->findPCICapability(kIOAHCIPCICapabilityID, &capabilityOffset);

    if (capabilityOffset) {
        capabilityReg.regs.SATACR0 = this->_pciDevice->configRead32(capabilityOffset);
        capabilityReg.regs.SATACR1 = this->_pciDevice->configRead32(capabilityOffset + 0x4);

        /* ZORMEISTER: Why does the AHCI specification even allow this? */
        if (capabilityReg.bits.barLocation == kIOAHCICapabilityBARLocationPCIConfig) {
            this->_memoryInPCIConfig = true;
        }
        
        bar = (capabilityReg.bits.barLocation * 4);
        
        this->_memoryOffset = capabilityReg.bits.offset * 4;
    }
    
    if (!this->_memoryInPCIConfig) {
        this->_memoryMap = this->_pciDevice->mapDeviceMemoryWithRegister(bar, kIOMapInhibitCache);
        this->_memoryDescriptor = this->_memoryMap->getMemoryDescriptor();
    }
    
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
    
    IOSimpleLockLock(this->_registerLock);
    
    if (this->_memoryInPCIConfig) {
        /* "Index-Data Pair is implemented in Dwords directly following SATACR1 in the PCI configuration space" */
        value = this->_pciDevice->configRead32(((this->_sataCapabilityOffset + 4) + _memoryOffset + reg));
    } else {
        this->_memoryDescriptor->readBytes((this->_memoryOffset + reg), &value, sizeof(UInt32));
    }
    
    IOSimpleLockUnlock(this->_registerLock);
    
    return value;
}

void PureAHCIDriver::writeRegister(UInt32 reg, UInt32 value)
{
    IOSimpleLockLock(this->_registerLock);
    
    if (this->_memoryInPCIConfig) {
        /* "Index-Data Pair is implemented in Dwords directly following SATACR1 in the PCI configuration space" */
        this->_pciDevice->configWrite32(((this->_sataCapabilityOffset + 4) + _memoryOffset + reg), value);
    } else {
        this->_memoryDescriptor->writeBytes((this->_memoryOffset + reg), &value, sizeof(UInt32));
    }
    
    IOSimpleLockUnlock(this->_registerLock);
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
