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

#ifndef PureAHCIDriver_h
#define PureAHCIDriver_h

#include <IOKit/acpi/IOACPIPlatformDevice.h>
#include <IOKit/ahci/IOAHCIController.h>
#include <IOKit/pci/IOPCIDevice.h>

/*!
 * @class PureAHCIDriver
 *
 * @abstract This class provides basic PCI AHCI support
 *
 * @discussion
 * Controllers with quirks should be handled by subclassing PureAHCIDriver and PureAHCIPort and adding the IDs
 * to an IOKit personality with a higher probe score than PureAHCIDriver.
 */

class PureAHCIDriver : public IOAHCIController {
    OSDeclareDefaultStructors(PureAHCIDriver);

    virtual IOService *probe(IOService *provider, SInt32 *score) override;
    
    virtual bool start(IOService *provider) override;
    
    virtual IOAHCIPort *createPort(UInt32 number) override;
    
    virtual UInt32 readRegister(UInt32 reg) override;
    
    virtual void writeRegister(UInt32 reg, UInt32 value) override;
    
protected:
    IOMemoryMap *_memoryMap;
    IOMemoryDescriptor *_memoryDescriptor;
    IOPCIDevice *_pciDevice;
    IOACPIPlatformDevice *_acpiDevice; /* ACPI power management. I need to integrate this driver with IOPM at some point. */
    UInt32 _memoryOffset;
    bool _memoryInPCIConfig;
    UInt8 _sataCapabilityOffset;
};

#endif /* PureAHCIDriver_h */
