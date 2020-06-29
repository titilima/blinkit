// -------------------------------------------------
// BlinKit - BkHelper Library
// -------------------------------------------------
//   File Name: proxy_request_impl.cpp
// Description: Exports for BkProxyRequest
//      Author: Ziming Li
//     Created: 2020-06-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "proxy_request_impl.h"

#include "bk_helper.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT int BKAPI BkGetProxyRequestData(BkProxyRequest request, int data, struct BkBuffer *dst)
{
    return request->GetData(data, dst);
}

BKEXPORT int BKAPI BkGetProxyRequestHeader(BkProxyRequest request, const char *name, struct BkBuffer *dst)
{
    return request->GetHeader(name, dst);
}

BKEXPORT void BKAPI BkSetProxyRequestHeader(BkProxyRequest request, const char *name, const char *value)
{
    request->SetHeader(name, value);
}

} // extern "C"
