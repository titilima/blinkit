// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: local_frame_client_impl.cpp
// Description: LocalFrameClientImpl Class
//      Author: Ziming Li
//     Created: 2019-09-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "local_frame_client_impl.h"

#include "bkcommon/bk_strings.h"
#include "third_party/blink/renderer/core/exported/web_document_loader_impl.h"

using namespace blink;

namespace BlinKit {

std::unique_ptr<DocumentLoader> LocalFrameClientImpl::CreateDocumentLoader(
    LocalFrame *frame,
    const ResourceRequest &request,
    const SubstituteData &data, std::unique_ptr<WebDocumentLoader::ExtraData> extraData)
{
    std::unique_ptr<WebDocumentLoaderImpl> ret = WebDocumentLoaderImpl::Create(frame, request, data);
    ret->SetExtraData(std::move(extraData));
    return ret;
}

String LocalFrameClientImpl::UserAgent(void)
{
    return String::FromUTF8(Strings::DefaultUserAgent);
}

} // namespace BlinKit
