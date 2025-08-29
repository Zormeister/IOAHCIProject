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

#ifndef _IOKIT_SATA_IOSERIALATATYPES_H_
#define _IOKIT_SATA_IOSERIALATATYPES_H_

#include <IOKit/IOTypes.h>

enum {
    /*!
     * @const kIOSerialATAFISTypeHost2Device
     * The FIS for transferring the register block from the host to the SATA device.
     */
    kIOSerialATAFISTypeHost2Device = 0x27,
    
    /*!
     * @const kIOSerialATAFISTypeDevice2Host
     * The FIS for transferring the register block from the device to the host.
     */
    kIOSerialATAFISTypeDevice2Host = 0x34,

    /*!
     * @const kIOSerialATAFISTypeDMAActivate
     * The FIS that signals the host to begin a DMA transfer.
     */
    kIOSerialATAFISTypeDMAActivate = 0x39,
    
    /*!
     * @const kIOSerialATAFISTypeDMAActivate
     * The FIS that establishes a DMA transfer operation, this FIS can come from the host or device.
     */
    kIOSerialATAFISTypeDMASetup = 0x41,
    
    /*!
     * @const kIOSerialATAFISTypeData
     * The FIS that carries a payload of data, the maximum size is 2048 32-bit integers of data.
     */
    kIOSerialATAFISTypeData = 0x46,
    
    kIOSerialATAFISTypeBISTActivate = 0x58,
    
    kIOSerialATAFISTypePIOSetup = 0x5F,
    
    kIOSerialATAFISTypeSetDeviceBits = 0xA1,
};


#endif /* _IOKIT_SATA_IOSERIALATATYPES_H_ */
