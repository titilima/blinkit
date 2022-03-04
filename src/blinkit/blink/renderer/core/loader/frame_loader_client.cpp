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

#include "./frame_loader_client.h"

#include "bkcommon/bk_strings.h"
#include "blinkit/blink/renderer/web/WebDataSourceImpl.h"
#include "blinkit/js/web_view_context.h"

using namespace BlinKit;

namespace blink {

std::unique_ptr<ScriptController> FrameLoaderClient::CreateContext(LocalFrame &frame)
{
    const Type type = GetType();

#ifdef BLINKIT_CRAWLER_ENABLED
    if (Type::Crawler == type)
        ASSERT(false); // BKTODO:
#endif

#ifdef BLINKIT_UI_ENABLED
    if (Type::WebView == type)
        return std::make_unique<WebViewContext>(frame);
#endif

    NOTREACHED();
    return nullptr;
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

String FrameLoaderClient::userAgent(void)
{
    return String::fromUTF8(Strings::DefaultUserAgent);
}

} // namespace blink
