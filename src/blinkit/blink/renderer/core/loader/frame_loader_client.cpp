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
#include "blinkit/blink/renderer/web/WebDataSourceImpl.h"

using namespace BlinKit;

namespace blink {

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
