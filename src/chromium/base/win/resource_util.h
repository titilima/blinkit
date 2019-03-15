// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: resource_util.h
// Description: Resource Utilities
//      Author: Ziming Li
//     Created: 2019-03-15
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BASE_RESOURCE_UTIL_H
#define BLINKIT_BASE_RESOURCE_UTIL_H

#pragma once

#include <vector>
#include <Windows.h>

namespace base {

bool GetResourceFromModule(HMODULE module, PCSTR resourceId, PCSTR resourceType, void **data, size_t* length);

} // namespace base

#endif // BLINKIT_BASE_RESOURCE_UTIL_H
