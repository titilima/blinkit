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
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/bindings/core/duk/script_controller.h"

using namespace blink;

namespace BlinKit {

int LocalFrameClientImpl::CallJS(LocalFrame *frame, BkJSCallback callback, void *userData)
{
    if (IsMainThread())
        CallJSImpl(frame, callback, userData);
    else
        m_appCaller.Call(FROM_HERE, std::bind(CallJSImpl, frame, callback, userData));
    return BK_ERR_SUCCESS;
}

void LocalFrameClientImpl::CallJSImpl(LocalFrame *frame, BkJSCallback callback, void *userData)
{
    ScriptController &ctx = frame->GetScriptController();
    callback(&ctx, userData);
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
