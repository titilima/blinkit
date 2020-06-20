// -------------------------------------------------
// BlinKit - BkBase Library
// -------------------------------------------------
//   File Name: _pc.h
// Description: Pre-compiled Header File
//      Author: Ziming Li
//     Created: 2020-06-20
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKBASE__PC_H
#define BLINKIT_BKBASE__PC_H

#pragma once

#include <memory>
#include <string>
#include "build/build_config.h"

#if 0
#if OS_WIN
#   pragma warning(disable: 4244 4291 5033)
#   define _CRT_SECURE_NO_WARNINGS
#   define NOMINMAX
#   define WIN32_LEAN_AND_MEAN
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

#include <unordered_map>
#include <vector>

#ifdef OS_WIN
#   include <Windows.h>
extern HMODULE theModule;
#endif

#if defined(OS_MACOSX) || defined(OS_IOS)
#   include <CoreFoundation/CoreFoundation.h>
#endif
#endif

#include "BlinKit.hpp"
#include "base/logging.h"

#endif // BLINKIT_BKBASE__PC_H
