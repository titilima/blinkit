// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: apple_request.mm
// Description: AppleRequest Class
//      Author: Ziming Li
//     Created: 2019-08-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "apple_request.h"

#include "app/app_constants.h"
#include "http/response_impl.h"

namespace BlinKit {
#if 0
AppleRequest::AppleRequest(const char *URL, const BkRequestClient &client)
    : RequestImpl(URL, client), m_sessionTask(nil)
{
    SetHeader("User-Agent", AppConstants::DefaultUserAgent);
}

AppleRequest::~AppleRequest(void)
{
    if (m_sessionTask)
    {
        [m_sessionTask release];
        m_sessionTask = nil;
    }
}

void AppleRequest::Cancel(void)
{
    if (m_sessionTask)
        [m_sessionTask cancel];
    RequestImpl::Release();
}

int AppleRequest::Perform(void)
{
    NSURL *u = [NSURL URLWithString: [NSString stringWithUTF8String: m_URL.c_str()]];

    NSString *scheme = u.scheme;
    if (![scheme isEqualToString: @"http"] && ![scheme isEqualToString: @"https"])
        return BK_ERR_URI;

    NSMutableURLRequest *req = [NSMutableURLRequest requestWithURL: u];

    double timeoutInMs = static_cast<double>(TimeoutInMs());
    req.timeoutInterval = timeoutInMs / 1000;

    req.HTTPMethod = [NSString stringWithUTF8String: m_method.c_str()];

    for (const auto &it : m_headers.GetRawMap())
    {
        NSString *name = [NSString stringWithUTF8String: it.first.c_str()];
        NSString *val = [NSString stringWithUTF8String: it.second.c_str()];
        [req addValue: val forHTTPHeaderField: name];
    }

    if (!m_body.empty())
    {
        req.HTTPBody = [NSData dataWithBytes: m_body.data()
                                      length: m_body.size()];
    }

    NSURLSession *session = [NSURLSession sharedSession];
    id handler = ^(NSData *data, NSURLResponse *response, NSError *error) {
        RequestComplete(data, response, error);
        RequestImpl::Release();
    };
    m_sessionTask = [session dataTaskWithRequest: req completionHandler: handler];
    [m_sessionTask retain];
    [m_sessionTask resume];
    return BK_ERR_SUCCESS;
}

void AppleRequest::RequestComplete(NSData *data, NSURLResponse *response, NSError *error)
{
    if (nil != error)
    {
        m_client.RequestFailed(BK_ERR_NETWORK, m_client.UserData);
        return;
    }

    NSHTTPURLResponse *repo = static_cast<NSHTTPURLResponse *>(response);

    m_response = std::make_shared<ResponseImpl>(m_URL);
    m_response->SetCurrentURL(repo.URL.absoluteString.UTF8String);
    m_response->SetStatusCode(repo.statusCode);
    for (NSString *key in repo.allHeaderFields)
    {
        NSString *val = repo.allHeaderFields[key];
        m_response->AppendHeader(key.UTF8String, val.UTF8String);
    }
    m_response->AppendData(data.bytes, data.length);

    m_client.RequestComplete(m_response.get(), m_client.UserData);
}
#endif

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" BkRequest BKAPI BkCreateRequest(const char *URL, BkRequestClient *client)
{
    ASSERT(false); // BKTODO:
    return nullptr;
    // BKTODO: return new BlinKit::AppleRequest(URL, *client);
}
