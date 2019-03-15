// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: resource_util.cpp
// Description: Resource Utilities
//      Author: Ziming Li
//     Created: 2019-03-15
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "resource_util.h"

namespace base {

bool GetResourceFromModule(HMODULE module, PCSTR resourceId, PCSTR resourceType, void **data, size_t* length)
{
    HRSRC hResInfo = FindResourceA(module, resourceId, resourceType);
    if (nullptr == hResInfo)
        return false;

    DWORD dataSize = SizeofResource(module, hResInfo);
    HGLOBAL hRes = LoadResource(module, hResInfo);
    if (nullptr == hRes)
        return false;

    void *resource = LockResource(hRes);
    if (nullptr == resource)
        return false;

    *data = resource;
    *length = static_cast<size_t>(dataSize);
    return true;
}

} // namespace base
