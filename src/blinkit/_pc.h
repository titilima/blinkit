// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: _pc.h
// Description: Pre-compiled Header File
//      Author: Ziming Li
//     Created: 2019-03-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT__PC_H
#define BLINKIT_BLINKIT__PC_H

#pragma once

#include "build/build_config.h"

#if OS_WIN
#   pragma warning(disable: 4244 4291 4305 4819 5033)
#   define _CRT_SECURE_NO_WARNINGS
#   define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#   define NOMINMAX
#   define WIN32_LEAN_AND_MEAN
#   define U_STATIC_IMPLEMENTATION
#   define UCHAR_TYPE   wchar_t
#endif

#if (OS_MACOSX || OS_IOS)
#   pragma clang diagnostic ignored "-Wcomma"
#   pragma clang diagnostic ignored "-Wdocumentation"
#   pragma clang diagnostic ignored "-Winconsistent-missing-override"
#   pragma clang diagnostic ignored "-Wshorten-64-to-32"
#   pragma clang diagnostic ignored "-Wswitch"
#endif

#define BLINK_IMPLEMENTATION    1
#define INSIDE_BLINK            1

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef OS_WIN
#   include <Windows.h>
extern HMODULE theModule;
#endif

#if defined(OS_MACOSX) || defined(OS_IOS)
#   include <CoreFoundation/CoreFoundation.h>
#endif

#include "base/logging.h"

#endif // BLINKIT_BLINKIT__PC_H
