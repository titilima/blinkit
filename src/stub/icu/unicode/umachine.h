// -------------------------------------------------
// BlinKit - stub Part
// -------------------------------------------------
//   File Name: umachine.h
// Description: ICU Stub
//      Author: Ziming Li
//     Created: 2019-02-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_STUB_ICU_UMACHINE_H
#define BLINKIT_STUB_ICU_UMACHINE_H

#pragma once

#include <cstddef>
#include <cstdint>
#if (OS_MACOSX || OS_IOS)
#   include <MacTypes.h>
#endif

typedef int8_t UBool;

#ifndef TRUE
#   define TRUE  1
#endif
#ifndef FALSE
#   define FALSE 0
#endif

#if defined(OS_WIN)
typedef wchar_t UChar;
#elif defined(OS_MACOSX) || defined(OS_IOS)
typedef UniChar UChar;
#elif defined(OS_POSIX)
typedef uint16_t UChar;
#endif

typedef int32_t UChar32;

#endif // BLINKIT_STUB_ICU_UMACHINE_H
