// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: frame_loader_client.cpp
// Description: FrameLoaderClient Class
//      Author: Ziming Li
//     Created: 2021-08-08
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./FrameLoaderClient.h"

#include "bkcommon/bk_strings.h"
#include "blinkit/app/caller.h"
#include "blinkit/blink/renderer/web/WebDataSourceImpl.h"

using namespace BlinKit;

namespace blink {

FrameLoaderClient::FrameLoaderClient(AppCaller &appCaller, ClientCaller &clientCaller)
    : m_appCaller(appCaller), m_clientCaller(clientCaller)
{
}

PassRefPtrWillBeRawPtr<DocumentLoader> FrameLoaderClient::createDocumentLoader(
    LocalFrame *frame,
    const ResourceRequest &request,
    const SubstituteData &data)
{
    // BKTODO: Is WebDataSourceImpl necessary?
    //         DocumentLoader may be enough.
    return WebDataSourceImpl::create(frame, request, data);
}

#ifndef NDEBUG
bool FrameLoaderClient::IsClientThread(void) const
{
    return m_clientCaller.IsClientThread();
}
#endif

String FrameLoaderClient::userAgent(void)
{
    return String::fromUTF8(Strings::DefaultUserAgent);
}

} // namespace blink
