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

#   pragma warning(disable: 4244 4291)

#   define NOMINMAX
#   define WIN32_LEAN_AND_MEAN

#endif

#define BLINK_IMPLEMENTATION    1
#define INSIDE_BLINK            1

#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#if OS_WIN
#   include <Windows.h>
#endif

#endif // BLINKIT_BLINKIT__PC_H
