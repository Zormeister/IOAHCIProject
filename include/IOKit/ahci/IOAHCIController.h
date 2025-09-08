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

#ifndef _IOKIT_AHCI_IOAHCICONTROLLER_H_
#define _IOKIT_AHCI_IOAHCICONTROLLER_H_

#include <IOKit/IOFilterInterruptEventSource.h>
#include <IOKit/IOService.h>
#include <IOKit/IOWorkLoop.h>
#include <IOKit/ahci/IOAHCITypes.h>

class IOAHCIPort;

class IOAHCIController : public IOService {
    OSDeclareAbstractStructors(IOAHCIController);

    friend class IOAHCIPort;

    virtual bool start(IOService *provider) override;

protected:
    /*!
     * @function createPort
     *
     * @abstract Creates IOAHCIPort subclassed objects and places them in the IORegistry.
     */
    virtual IOAHCIPort *createPort(UInt32 number) = 0;

    /*!
     * @function readRegister
     *
     * @abstract Reads the AHCI register space
     */
    virtual UInt32 readRegister(UInt32 reg) = 0;

    /*!
     * @function writeRegister
     *
     * @abstract Writes to the AHCI register space
     */
    virtual void writeRegister(UInt32 reg, UInt32 value) = 0;
    
    /*!
     * @function reset
     * Resets the HBA, simple in practice.
     */
    virtual void reset(void);

public:
    /* ZORMEISTER: These have to be pubilc or else PureAHCIDriver will not build. */
    virtual bool filterInterrupt(IOFilterInterruptEventSource *sender);
    virtual void handleInterrupt(IOInterruptEventSource *sender, int count);

protected:
    OSArray *fPortArray;
    IOSimpleLock *fRegisterLock;
    IOWorkLoop *fWorkLoop;
    IOFilterInterruptEventSource *fInterrupt;
};

#endif /* _IOKIT_AHCI_IOAHCICONTROLLER_H_ */
