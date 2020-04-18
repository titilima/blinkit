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

#include "blinkit/app/app_constants.h"
#include "blinkit/apple/ns.h"
#include "blinkit/common/bk_url.h"
#include "blinkit/http/response_impl.h"

namespace BlinKit {

AppleRequest::AppleRequest(const char *URL, const BkRequestClient &client) : RequestImpl(URL, client)
{
    SetHeader("User-Agent", AppConstants::DefaultUserAgent);
}

AppleRequest::~AppleRequest(void) = default;

void AppleRequest::Cancel(void)
{
    ASSERT(false); // BKTODO:
}

ControllerImpl* AppleRequest::GetController(void)
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

int AppleRequest::Perform(void)
{
    int err = BK_ERR_UNKNOWN;

    NSURL *u = NS::StringToURL(m_URL);
    do {
        // 1. Check URL.
        NSString *scheme = u.scheme;
        if (![scheme isEqualToString: @"http"] && ![scheme isEqualToString: @"https"])
        {
            err = BK_ERR_URI;
            break;
        }

        NSMutableURLRequest *req = [NSMutableURLRequest requestWithURL: u];
        double timeoutInMs = static_cast<double>(TimeoutInMs());
        req.timeoutInterval = timeoutInMs / 1000;
        req.HTTPMethod = NS::StringFromStd(m_method);
        for (const auto &it : m_headers.GetRawMap())
        {
            NSString *k = NS::StringFromStd(it.first);
            NSString *v = NS::StringFromStd(it.second);
            [req addValue: v forHTTPHeaderField: k];
        }
        if (!m_body.empty())
        {
            NSData *body = [NSData dataWithBytes: m_body.data()
                                          length: m_body.size()];
            [req setHTTPBody: body];
        }

        NSURLSession *session = [NSURLSession sharedSession];
        id handler = ^(NSData *data, NSURLResponse *response, NSError *error)
        {
            RequestComplete(data, response, error);
            RequestImpl::Release();
        };
        NSURLSessionDataTask *task = [session dataTaskWithRequest: req completionHandler: handler];
        [task resume];
        return BK_ERR_SUCCESS;
    } while (false);

    ASSERT(BK_ERR_SUCCESS == err);
    delete this;
    return err;
}

void AppleRequest::RequestComplete(NSData *data, NSURLResponse *response, NSError *error)
{
    if (nil != error)
    {
        m_client.RequestFailed(BK_ERR_NETWORK, m_client.UserData);
        return;
    }

    m_response = std::make_unique<ResponseImpl>(m_URL);

    NSHTTPURLResponse *repo = static_cast<NSHTTPURLResponse *>(response);
    m_response->SetCurrentURL(NS::StringToStd(repo.URL.absoluteString));
    m_response->SetStatusCode(repo.statusCode);
    for (NSString *key in repo.allHeaderFields)
    {
        NSString *val = repo.allHeaderFields[key];
        m_response->AppendHeader(key.UTF8String, val.UTF8String);
    }
    m_response->AppendData(data.bytes, data.length);

    m_client.RequestComplete(m_response.get(), m_client.UserData);
}

} // namespace BlinKit

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" BkRequest BKAPI BkCreateRequest(const char *URL, BkRequestClient *client)
{
    return new BlinKit::AppleRequest(URL, *client);
}
