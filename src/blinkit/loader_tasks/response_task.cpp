// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: response_task.cpp
// Description: ResponseTask Class
//      Author: Ziming Li
//     Created: 2019-03-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "response_task.h"

#include "public/platform/Platform.h"
#include "public/platform/WebMimeRegistry.h"
#include "public/platform/WebString.h"
#include "public/platform/WebURLLoaderClient.h"
#include "public/platform/WebURLResponse.h"

using namespace blink;

namespace BlinKit {

ResponseTask::ResponseTask(WebURLLoader *loader, WebURLLoaderClient *client, ResponseData &responseData)
    : m_loader(loader), m_client(client), m_responseData(responseData.shared_from_this())
{
    if (!m_responseData->MIMEType.isEmpty())
        return;

    do {
        String fileName = m_responseData->URI.lastPathComponent();
        if (fileName.isEmpty())
            break;

        size_t p = fileName.reverseFind('.');
        if (kNotFound == p)
            break;

        WebString ext = fileName.substring(p + 1);
        m_responseData->MIMEType = Platform::current()->mimeRegistry()->mimeTypeForExtension(ext);
        return;
    } while (false);

    m_responseData->MIMEType = "application/octet-stream";
}

void ResponseTask::run(void)
{
    WebURLResponse response;
    response.initialize();
    response.setURL(m_responseData->URI);
    response.setHTTPStatusCode(m_responseData->StatusCode);

    if (!m_responseData->ContentType.empty())
    {
        WebString contentType = WebString::fromUTF8(m_responseData->ContentType);
        response.setHTTPHeaderField("Content-Type", contentType);
    }

    assert(!m_responseData->MIMEType.isEmpty());
    response.setMIMEType(m_responseData->MIMEType);

    if (!m_responseData->TextEncoding.isEmpty())
        response.setTextEncodingName(m_responseData->TextEncoding);

    m_client->didReceiveResponse(m_loader, response);

    size_t bodySize = m_responseData->Body.size();
    if (0 == bodySize)
    {
        // Use a white space to avoid assertions.
        m_responseData->Body.push_back(' ');
        bodySize = 1;
    }
    const char *body = reinterpret_cast<const char *>(m_responseData->Body.data());
    m_client->didReceiveData(m_loader, body, bodySize, 0);
    m_client->didFinishLoading(m_loader, 0.0, bodySize);
}

} // namespace BlinKit
