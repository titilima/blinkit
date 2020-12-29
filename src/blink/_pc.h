// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: _pc.h
// Description: Pre-compiled Header File
//      Author: Ziming Li
//     Created: 2018-12-09
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK__PC_H
#define BLINKIT_BLINK__PC_H

#pragma once

#include "build/build_config.h"

#if OS_WIN
#   pragma warning(disable: 4068 4244 4291 4305 4819 5033)
#   define _CRT_NONSTDC_NO_WARNINGS
#   define _CRT_SECURE_NO_WARNINGS
#   define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#   define NOMINMAX
#   define WIN32_LEAN_AND_MEAN
#   define UCHAR_TYPE   wchar_t
#endif

#if (OS_MACOSX || OS_IOS)
#   pragma clang diagnostic ignored "-Wcomma"
#   pragma clang diagnostic ignored "-Wconditional-uninitialized"
#   pragma clang diagnostic ignored "-Wdeprecated-declarations"
#   pragma clang diagnostic ignored "-Wdocumentation"
#   pragma clang diagnostic ignored "-Winconsistent-missing-override"
#   pragma clang diagnostic ignored "-Wrange-loop-analysis"
#   pragma clang diagnostic ignored "-Wshorten-64-to-32"
#   pragma clang diagnostic ignored "-Wswitch"
#   pragma clang diagnostic ignored "-Wunused-function"
#   pragma clang diagnostic ignored "-Wunused-variable"
#endif

#define BLINK_IMPLEMENTATION            1
#define INSIDE_BLINK                    1
#define WTF_USE_DEFAULT_RENDER_THEME    1

#include <functional>
#include <memory>

#include "base/logging.h"
#include "third_party/blink/renderer/core/dom/document.h" // Used for speeding up compilation
#ifndef BLINKIT_CRAWLER_ONLY
#   include "_skia.h"
#   include "third_party/blink/renderer/core/layout/layout_object.h" // Used for speeding up compilation
#endif

#endif // BLINKIT_BLINK__PC_H
