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

#include <cassert>
#include "build/build_config.h"

#ifdef OS_WIN

#   pragma warning(disable: 4244 4291 5033)

#   define _CRT_NONSTDC_NO_WARNINGS
#   define _CRT_SECURE_NO_WARNINGS
#   define NOMINMAX

#endif

#define INSIDE_BLINK    1

#endif // BLINKIT_BLINK__PC_H
