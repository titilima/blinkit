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

#include "platform/network/HTTPParsers.h"
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
    // Nothing
}

static AtomicString AdjustMIMEType(const KURL &URI)
{
    do {
        String fileName = URI.lastPathComponent();
        if (fileName.isEmpty())
            break;

        size_t p = fileName.reverseFind('.');
        if (kNotFound == p)
            break;

        WebString ext = fileName.substring(p + 1);
        return Platform::current()->mimeRegistry()->mimeTypeForExtension(ext);
    } while (false);
    return "application/octet-stream";
}

void ResponseTask::run(void)
{
    WebURLResponse response;
    response.initialize();
    response.setURL(m_responseData->URI);
    response.setHTTPStatusCode(m_responseData->StatusCode);
    if (!m_responseData->ContentType.empty())
    {
        AtomicString contentType = AtomicString::fromUTF8(m_responseData->ContentType.data(),
            m_responseData->ContentType.length()).lower();
        response.setHTTPHeaderField("Content-Type", contentType);

        AtomicString MIMEType = extractMIMETypeFromMediaType(contentType);
        response.setMIMEType(MIMEType);

        String textEncoding = extractCharsetFromMediaType(contentType);
        if (!textEncoding.isEmpty())
            response.setTextEncodingName(textEncoding);
    }
    else
    {
        AtomicString mimeType = AdjustMIMEType(m_responseData->URI);
        response.setMIMEType(mimeType);
        response.setHTTPHeaderField("Content-Type", mimeType);
    }
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
