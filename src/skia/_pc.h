// -------------------------------------------------
// BlinKit - skia Library
// -------------------------------------------------
//   File Name: _pc.h
// Description: Pre-compiled Header File
//      Author: Ziming Li
//     Created: 2020-10-12
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SKIA__PC_H
#define BLINKIT_SKIA__PC_H

#pragma once

#include "build/build_config.h"

#if OS_WIN
#   pragma warning(disable: 4244 4267 4291 4334 4819)
#   define _CRT_SECURE_NO_WARNINGS
#   define SK_GDI_ALWAYS_USE_TEXTMETRICS_FOR_FONT_METRICS
#endif

#if (OS_MACOSX || OS_IOS)
// #   pragma clang diagnostic ignored "-Wconditional-uninitialized"
// #   pragma clang diagnostic ignored "-Wdeprecated-declarations"
// #   pragma clang diagnostic ignored "-Wdocumentation"
// #   pragma clang diagnostic ignored "-Wshorten-64-to-32"
// #   pragma clang diagnostic ignored "-Wunreachable-code"
#endif

#include "_skia.h"

#define SK_CPU_SSE_LEVEL    SK_CPU_SSE_LEVEL_SSE41

#endif // BLINKIT_SKIA__PC_H
