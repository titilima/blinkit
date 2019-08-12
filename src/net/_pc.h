// -------------------------------------------------
// BlinKit - net Library
// -------------------------------------------------
//   File Name: _pc.h
// Description: Pre-compiled Header File
//      Author: Ziming Li
//     Created: 2019-04-26
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_NET__PC_H
#define BLINKIT_NET__PC_H

#pragma once

#include "build/build_config.h"

#if OS_WIN
#   pragma warning(disable: 4244)
#   define NOMINMAX
#endif

#if (OS_MACOSX || OS_IOS)
#   pragma clang diagnostic ignored "-Wshorten-64-to-32"
#   pragma clang diagnostic ignored "-Wunused-const-variable"
#   pragma clang diagnostic ignored "-Wunused-variable"
#endif

#include <cassert>

#endif // BLINKIT_NET__PC_H
