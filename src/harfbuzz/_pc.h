// -------------------------------------------------
// BlinKit - harfbuzz Library
// -------------------------------------------------
//   File Name: _pc.h
// Description: Pre-compiled Header File
//      Author: Ziming Li
//     Created: 2019-03-07
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_HARFBUZZ__PC_H
#define BLINKIT_HARFBUZZ__PC_H

#pragma once

#include "build/build_config.h"

#if OS_WIN

#   pragma warning(disable: 4244 4819)

#   define _CRT_SECURE_NO_WARNINGS

#endif

#define HAVE_OT

#endif // BLINKIT_HARFBUZZ__PC_H
