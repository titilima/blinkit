// -------------------------------------------------
// BlinKit - stub Part
// -------------------------------------------------
//   File Name: utf.h
// Description: ICU Stub
//      Author: Ziming Li
//     Created: 2019-02-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_STUB_ICU_UTF_H
#define BLINKIT_STUB_ICU_UTF_H

#pragma once

#include "umachine.h"

#define U_IS_BMP(c) ((uint32_t)(c)<=0xffff)

#define U_IS_SUPPLEMENTARY(c) ((uint32_t)((c)-0x10000)<=0xfffff)

#define U_IS_SURROGATE(c) (((c)&0xfffff800)==0xd800)

#include "utf8.h"
#include "utf16.h"

#endif // BLINKIT_STUB_ICU_UTF_H
