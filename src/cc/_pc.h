// -------------------------------------------------
// BlinKit - cc Library
// -------------------------------------------------
//   File Name: _pc.h
// Description: Pre-compiled Header File
//      Author: Ziming Li
//     Created: 2020-08-04
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_CC__PC_H
#define BLINKIT_CC__PC_H

#pragma once

#include "build/build_config.h"

#if OS_WIN
#   pragma warning(disable: 4244 4819)
#endif

#include <memory>

#include "base/logging.h"

#endif // BLINKIT_CC__PC_H
