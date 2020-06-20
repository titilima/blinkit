// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: http_exports.cpp
// Description: Exported Functions for HTTP Part
//      Author: Ziming Li
//     Created: 2020-06-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "bk_http.h"

#include "blinkit/http/request_impl.h"
#include "blinkit/http/response_impl.h"

extern "C" {

BKEXPORT BkRequest BKAPI BkCreateRequest(const char *URL, BkRequestClient *client)
{
    return RequestImpl::CreateInstance(URL, *client);
}

BKEXPORT int BKAPI BkPerformRequest(BkRequest request, BkWorkController *controller)
{
    if (nullptr != controller)
        *controller = request->GetController();
    return request->Perform();
}

BKEXPORT void BKAPI BkSetRequestBody(BkRequest request, const void *data, size_t dataLength)
{
    request->SetBody(data, dataLength);
}

BKEXPORT void BKAPI BkSetRequestHeader(BkRequest request, const char *name, const char *value)
{
    request->SetHeader(name, value);
}

BKEXPORT void BKAPI BkSetRequestMethod(BkRequest request, const char *method)
{
    request->SetMethod(method);
}

BKEXPORT void BKAPI BkSetRequestProxy(BkRequest request, int type, const char *proxy)
{
    request->SetProxy(type, proxy);
}

BKEXPORT void BKAPI BkSetRequestTimeout(BkRequest request, unsigned timeout)
{
    request->SetTimeout(timeout);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BKEXPORT int BKAPI BkGetResponseCookie(BkResponse response, size_t index, BkBuffer *dst)
{
    return response->GetCookie(index, dst);
}

BKEXPORT size_t BKAPI BkGetResponseCookiesCount(BkResponse response)
{
    return response->CookiesCount();
}

BKEXPORT int BKAPI BkGetResponseData(BkResponse response, int data, BkBuffer *dst)
{
    return response->GetData(data, dst);
}

BKEXPORT int BKAPI BkGetResponseHeader(BkResponse response, const char *name, BkBuffer *dst)
{
    return response->GetHeader(name, dst);
}

BKEXPORT int BKAPI BkGetResponseStatusCode(BkResponse response)
{
    return response->StatusCode();
}

} // extern "C"
