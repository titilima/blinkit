// -------------------------------------------------
// BlinKit - harfbuzz Library
// -------------------------------------------------
//   File Name: _pc.h
// Description: Pre-compiled Header File
//      Author: Ziming Li
//     Created: 2020-12-27
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_HARFBUZZ__PC_H
#define BLINKIT_HARFBUZZ__PC_H

#pragma once

#include "chromium/build/build_config.h"

#if OS_WIN
#   pragma warning(disable: 4244 4267 4819)
#   define _CRT_SECURE_NO_WARNINGS
#endif

#define HAVE_OT
#define HB_NO_UNICODE_FUNCS

#endif // BLINKIT_HARFBUZZ__PC_H
