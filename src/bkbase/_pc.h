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

#if OS_WIN
#   define WIN32_LEAN_AND_MEAN
#   define NOMINMAX
#endif

#define CURL_STATICLIB
#define CURL_STRICTER

#include "base/logging.h"

#endif // BLINKIT_BKBASE__PC_H
