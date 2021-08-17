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

#include "chromium/build/build_config.h"

#if OS_WIN
#   pragma warning(disable: 4068 4244 4291 4305 4819 5033)
#   define _CRT_SECURE_NO_WARNINGS
#   define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#   define NOMINMAX
#   define WIN32_LEAN_AND_MEAN
#   define UCHAR_TYPE   wchar_t
#endif

#if (OS_MACOSX || OS_IOS)
#   pragma clang diagnostic ignored "-Wcomma"
#   pragma clang diagnostic ignored "-Wdocumentation"
#   pragma clang diagnostic ignored "-Winconsistent-missing-override"
#   pragma clang diagnostic ignored "-Wshorten-64-to-32"
#   pragma clang diagnostic ignored "-Wswitch"
#   define UCHAR_TYPE   unsigned short
#endif

#define BLINK_IMPLEMENTATION    1
#define ENABLE_OILPAN           1
#define INSIDE_BLINK            1
#define LOG_DISABLED            1

#ifdef BLINKIT_CRAWLER_ENABLED
#   ifndef BLINKIT_UI_ENABLED
#       define BLINKIT_CRAWLER_ONLY
#   endif
#endif
#ifdef BLINKIT_UI_ENABLED
#   ifndef BLINKIT_CRAWLER_ENABLED
#       define BLINKIT_UI_ONLY
#   endif
#endif
#if defined(BLINKIT_CRAWLER_ENABLED) && defined(BLINKIT_UI_ENABLED)
#   define BLINKIT_FULL_BUILD
#endif

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
#ifdef BLINKIT_UI_ENABLED
#   include "skia/_skia.h"
#endif

#ifdef NDEBUG
// Used for speeding up compilation
#   include "blinkit/blink/renderer/core/dom/Document.h" 
#   ifdef BLINKIT_UI_ENABLED
#       include "blinkit/blink/renderer/core/layout/LayoutObject.h"
#   endif
#endif

#endif // BLINKIT_BLINKIT__PC_H
