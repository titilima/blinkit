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
#include "blinkit/js/browser_context.h"
#include "third_party/blink/renderer/core/exported/web_document_loader_impl.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/bindings/core/duk/script_controller.h"

using namespace blink;

namespace BlinKit {

int LocalFrameClientImpl::CallJS(LocalFrame *frame, BkJSCallback callback, void *userData)
{
    auto task = [frame, callback, userData]
    {
        BrowserContext &ctx = frame->GetScriptController().EnsureContext();
        callback(&ctx, userData);
    };
    m_appCaller.Call(FROM_HERE, std::move(task));
    return BK_ERR_SUCCESS;
}

DocumentLoader* LocalFrameClientImpl::CreateDocumentLoader(
    LocalFrame *frame,
    const ResourceRequest &request, const SubstituteData &data,
    std::unique_ptr<WebDocumentLoader::ExtraData> extraData)
{
    WebDocumentLoaderImpl *ret = WebDocumentLoaderImpl::Create(frame, request, data);
#ifndef BLINKIT_CRAWLER_ONLY
    ret->GetTiming().SetNavigationStart(CurrentTimeTicks());
#endif
    ret->SetExtraData(std::move(extraData));
    return ret;
}

void LocalFrameClientImpl::DispatchDidFinishLoad(void)
{
    AutoGarbageCollector gc;
    m_clientCaller.Post(FROM_HERE,
        [this] {
            DidFinishLoad();
        }
    );
}

String LocalFrameClientImpl::UserAgent(void)
{
    return String::FromUTF8(Strings::DefaultUserAgent);
}

} // namespace BlinKit
