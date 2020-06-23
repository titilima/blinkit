// -------------------------------------------------
// BlinKit - BkLogin Library
// -------------------------------------------------
//   File Name: _pc.h
// Description: Pre-compiled Header File
//      Author: Ziming Li
//     Created: 2020-06-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BKLOGIN__PC_H
#define BLINKIT_BKLOGIN__PC_H

#pragma once

#include <memory>
#include <string>
#include "build/build_config.h"

#if OS_WIN
#   define WIN32_LEAN_AND_MEAN
#endif

#include "BlinKit.hpp"
#include "base/logging.h"

#ifndef BN_RAND_TOP_ANY
#   define BN_RAND_TOP_ANY  -1
#endif

#ifndef BN_RAND_BOTTOM_ANY
#   define BN_RAND_BOTTOM_ANY   -1
#endif

#endif // BLINKIT_BKLOGIN__PC_H
