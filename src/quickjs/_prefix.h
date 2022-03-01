#pragma once
// -------------------------------------------------
// BlinKit - quickjs Library
// -------------------------------------------------
//   File Name: _prefix.h
// Description: Prefix Header
//      Author: Ziming Li
//     Created: 2022-03-01
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_QUICKJS__PREFIX_H
#define BLINKIT_QUICKJS__PREFIX_H

#include "third_party/zed/include/zed/build_macros.h"

#ifdef _Z_OS_WINDOWS

#   define _CRT_SECURE_NO_WARNINGS
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>

#   pragma warning(disable: 4018 4244)

#   define EMSCRIPTEN

#   define __attribute(x)
#   define __attribute__(x)
#   define __builtin_frame_address(x)   (x)
#   define likely(x)                    (x)
#   define unlikely(x)                  (x)

#   ifdef _M_X64
typedef int64_t     ssize_t;
#   else
typedef int         ssize_t;
#   endif

#endif // _Z_OS_WINDOWS

#define CONFIG_VERSION  "20210327"

#endif // BLINKIT_QUICKJS__PREFIX_H
