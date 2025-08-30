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

#ifndef _IOAHCIDEBUG_H_
#define _IOAHCIDEBUG_H_

#include <AssertMacros.h>
#include <sys/kdebug.h>
#include <IOKit/IOTypes.h>

extern UInt32 gIOAHCIDebugFlags;

/*
 * TODO:
 * ktrace/kdebug Integration?
 *
 * DBG_IOKIT - DBG_IOSATA
 */

enum {
    kIOAHCIDebugTrace = (1 << 0),
    kIOAHCIDebugPrint = (1 << 1),
    
    kIOAHCIDebugController = (1 << 4),
    kIOAHCIDebugPort = (1 << 5),
    kIOAHCIDebugDevice = (1 << 6),
};

enum {
    kIOAHCITraceController = 2,
    kIOAHCITracePort       = 3,
};

enum {
    kIOAHCIControllerTraceFunctionStart = 1,
};

enum {
    kIOAHCIPortTraceFunctionInit     = 1,
    kIOAHCIPortTraceFunctionInitCaps = 2,
};

#define IOAHCITraceCode(class, func) \
    IOKDBG_CODE(DBG_IOSATA, ( ( kIOAHCITrace##class & 0x3F ) << 10 ) | ( ( kIOAHCI##class##TraceFunction##func & 0xFF ) << 2 ))

#define IOAHCITraceBegin(class, func, a, b, c, d) \
    if (gIOAHCIDebugFlags & kIOAHCIDebug##class) { \
        if (gIOAHCIDebugFlags & kIOAHCIDebugPrint) { \
            kprintf("[AHCI][" #class "]: >> %s\n", __FUNCTION__); \
        } \
        if (gIOAHCIDebugFlags & kIOAHCIDebugTrace) { \
            KDBG((IOAHCITraceCode(class, func) | DBG_FUNC_START), a, b, c, d); \
        } \
    } \

#define IOAHCITraceBeginMsg(class, func, a, b, c, d, fmt, args...) \
    if (gIOAHCIDebugFlags & kIOAHCIDebug##class) { \
        if (gIOAHCIDebugFlags & kIOAHCIDebugPrint) { \
            kprintf("[AHCI][" #class "]: >> %s - " fmt "\n", __FUNCTION__, ##args); \
        } \
        if (gIOAHCIDebugFlags & kIOAHCIDebugTrace) { \
            KDBG((IOAHCITraceCode(class, func) | DBG_FUNC_START), a, b, c, d); \
        } \
    } \

#define IOAHCITraceEnd(class, func, a, b, c, d) \
    if (gIOAHCIDebugFlags & kIOAHCIDebug##class) { \
        if (gIOAHCIDebugFlags & kIOAHCIDebugPrint) { \
            kprintf("[AHCI][" #class "]: << %s\n", __FUNCTION__); \
        } \
        if (gIOAHCIDebugFlags & kIOAHCIDebugTrace) { \
            KDBG((IOAHCITraceCode(class, func) | DBG_FUNC_END), a, b, c, d); \
        } \
    } \

#define IOAHCITraceEndMsg(class, func, a, b, c, d, fmt, args...) \
    if (gIOAHCIDebugFlags & kIOAHCIDebug##class) { \
        if (gIOAHCIDebugFlags & kIOAHCIDebugPrint) { \
            kprintf("[AHCI][" #class "]: << %s - " fmt "\n", __FUNCTION__, ##args); \
        } \
        if (gIOAHCIDebugFlags & kIOAHCIDebugTrace) { \
            KDBG((IOAHCITraceCode(class, func) | DBG_FUNC_END), a, b, c, d); \
        } \
    } \

#define IOAHCITrace(class, func, a, b, c, d) \
    if (gIOAHCIDebugFlags & kIOAHCIDebug##class) { \
        if (gIOAHCIDebugFlags & kIOAHCIDebugPrint) { \
            kprintf("[AHCI][" #class "]: -- %s\n", __FUNCTION__); \
        } \
        if (gIOAHCIDebugFlags & kIOAHCIDebugTrace) { \
            KDBG((IOAHCITraceCode(class, func) | DBG_FUNC_NONE), a, b, c, d); \
        } \
    } \

#define IOAHCITraceMsg(class, code, a, b, c, d, fmt, args...) \
    if (gIOAHCIDebugFlags & kIOAHCIDebug##class) { \
        if (gIOAHCIDebugFlags & kIOAHCIDebugPrint) { \
            kprintf("[AHCI][" #class "]: -- %s - " fmt "\n", __FUNCTION__, ##args); \
        } \
        if (gIOAHCIDebugFlags & kIOAHCIDebugTrace) { \
            KDBG((IOAHCITraceCode(class, func) | DBG_FUNC_NONE), a, b, c, d); \
        } \
    } \

#define IOAHCIRequire(cond, label) __Require(cond, label)
#define IOAHCIRequireString(cond, label, string) __Require_String(cond, label, string)
#define IOAHCINRequire(cond, label) __nRequire(cond, label)
#define IOAHCINRequireString(cond, label, string) __nRequire_String(cond, label, string)

#define IOAHCIDebugLog(class, fmt, args...) \
    if ((gIOAHCIDebugFlags & kIOAHCIDebug##class) && (gIOAHCIDebugFlags & kIOAHCIDebugPrint)) { \
        kprintf("[AHCI][" #class "]: %s: " fmt "\n", __FUNCTION__, ##args); \
    } \

#endif /* _IOAHCIDEBUG_H_ */
