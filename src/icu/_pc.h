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
#   pragma warning(disable: 4819)
#   define UCHAR_TYPE   wchar_t
#endif

#endif // BLINKIT_ICU__PC_H
