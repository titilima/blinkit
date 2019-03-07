// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: _pc.h
// Description: Pre-compiled Header File
//      Author: Ziming Li
//     Created: 2019-03-07
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS__PC_H
#define BLINKIT_JS__PC_H

#pragma once

#include "build/build_config.h"

#if OS_WIN

#   pragma warning(disable: 4291)

#   define NOMINMAX
#   define WIN32_LEAN_AND_MEAN

#endif

#define BLINK_IMPLEMENTATION    1
#define INSIDE_BLINK            1

#include <cassert>

#endif // BLINKIT_JS__PC_H
