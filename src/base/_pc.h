// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: _pc.h
// Description: Pre-compiled Header File
//      Author: Ziming Li
//     Created: 2019-03-08
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BASE__PC_H
#define BLINKIT_BASE__PC_H

#pragma once

#include "build/build_config.h"

#if OS_WIN
#   pragma warning(disable: 4244)
#   define NOMINMAX
#endif

#if (OS_MACOSX || OS_IOS)
#   pragma clang diagnostic ignored "-Wdeprecated-declarations"
#   pragma clang diagnostic ignored "-Wshorten-64-to-32"
#endif

#include <cassert>
#include <cctype>
#include <cstring>

#endif // BLINKIT_BASE__PC_H
