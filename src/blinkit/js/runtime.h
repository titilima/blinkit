#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: runtime.h
// Description: Declaration for Global JSRuntime Object
//      Author: Ziming Li
//     Created: 2022-03-03
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_RUNTIME_H
#define BLINKIT_RUNTIME_H

#include "third_party/quickjs/quickjs.h"

namespace BlinKit {
extern JSRuntime *g_runtime;
}

#endif // BLINKIT_RUNTIME_H
