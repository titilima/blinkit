// -------------------------------------------------
// BlinKit - icu Library
// -------------------------------------------------
//   File Name: _pc.h
// Description: Pre-compiled Header File
//      Author: Ziming Li
//     Created: 2020-12-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_ICU__PC_H
#define BLINKIT_ICU__PC_H

#pragma once

#include "chromium/build/build_config.h"

#if OS_WIN
#   pragma warning(disable: 4244 4819)
#   define _CRT_SECURE_NO_WARNINGS
#   define U_COMMON_IMPLEMENTATION
#   define U_STATIC_IMPLEMENTATION
#   define UCHAR_TYPE               char16_t
#endif

#endif // BLINKIT_ICU__PC_H
