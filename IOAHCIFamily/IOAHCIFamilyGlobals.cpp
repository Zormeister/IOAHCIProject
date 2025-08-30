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

#include <IOKit/ahci/IOAHCITypes.h>
#include <pexpert/pexpert.h>
#include "IOAHCIFamilyDebug.h"

const OSSymbol *gIOAHCIPortTypeKey;
const OSSymbol *gIOAHCIHostCapabilitiesKey;

/* The default whilst this is in development */
#define kIOAHCIDefaultDebugFlags \
        kIOAHCIDebugPrint | \
        kIOAHCIDebugController | \
        kIOAHCIDebugPort | \
        kIOAHCIDebugDevice

UInt32 gIOAHCIDebugFlags = kIOAHCIDefaultDebugFlags;

class IOAHCIFamilyGlobals {
public:
    IOAHCIFamilyGlobals();
    ~IOAHCIFamilyGlobals();
};

static IOAHCIFamilyGlobals IOAHCIFamilyGlobals;

IOAHCIFamilyGlobals::IOAHCIFamilyGlobals()
{
    gIOAHCIHostCapabilitiesKey = OSSymbol::withCString("AHCI-CAP");
    gIOAHCIPortTypeKey = OSSymbol::withCString("AHCI Port Type");

    PE_parse_boot_argn("ahci_dbg", &gIOAHCIDebugFlags, sizeof(UInt32));
}

IOAHCIFamilyGlobals::~IOAHCIFamilyGlobals()
{
    OSSafeReleaseNULL(gIOAHCIPortTypeKey);
    OSSafeReleaseNULL(gIOAHCIHostCapabilitiesKey);
}
