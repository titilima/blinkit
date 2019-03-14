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
    if (!m_responseData->MimeType.empty())
        return;

    do {
        String fileName = m_responseData->URI.lastPathComponent();
        if (fileName.isEmpty())
            break;

        size_t p = fileName.reverseFind('.');
        if (kNotFound == p)
            break;

        WebString ext = fileName.substring(p + 1);
        WebString mimeType = Platform::current()->mimeRegistry()->mimeTypeForExtension(ext);
        m_responseData->MimeType = mimeType.utf8();
        return;
    } while (false);

    m_responseData->MimeType = "application/octet-stream";
}

void ResponseTask::run(void)
{
    WebURLResponse response;
    response.initialize();
    response.setURL(m_responseData->URI);
    response.setHTTPStatusCode(m_responseData->StatusCode);

    WebString mimeType = WebString::fromUTF8(m_responseData->MimeType);
    response.setMIMEType(mimeType);
    response.setHTTPHeaderField("Content-Type", mimeType);

    m_client->didReceiveResponse(m_loader, response);

    const char *body = reinterpret_cast<const char *>(m_responseData->Body.data());
    size_t bodySize = m_responseData->Body.size();
    m_client->didReceiveData(m_loader, body, bodySize, 0);
    m_client->didFinishLoading(m_loader, 0.0, bodySize);

    delete this;
}

} // namespace BlinKit
