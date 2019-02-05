// -------------------------------------------------
// BlinKit - stub Part
// -------------------------------------------------
//   File Name: uchar.h
// Description: ICU Stub
//      Author: Ziming Li
//     Created: 2019-02-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_STUB_UCHAR_H
#define BLINKIT_STUB_UCHAR_H

#pragma once

#include <cstdint>

#if defined(OS_WIN)
typedef wchar_t UChar;
#endif

typedef int32_t UChar32;

#define U_IS_BMP(c) ((uint32_t)(c)<=0xffff)

#define U16_SURROGATE_OFFSET                ((0xd800<<10UL)+0xdc00-0x10000)
#define U16_GET_SUPPLEMENTARY(lead, trail)  (((UChar32)(lead)<<10UL)+(UChar32)(trail)-U16_SURROGATE_OFFSET)

#define U16_IS_LEAD(c)  (((c)&0xfffffc00)==0xd800)
#define U16_IS_TRAIL(c) (((c)&0xfffffc00)==0xdc00)

#define U16_LEAD(supplementary)     (UChar)(((supplementary)>>10)+0xd7c0)
#define U16_TRAIL(supplementary)    (UChar)(((supplementary)&0x3ff)|0xdc00)

#endif // BLINKIT_STUB_UCHAR_H
