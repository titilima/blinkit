// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: local_frame_client_impl.h
// Description: LocalFrameClientImpl Class
//      Author: Ziming Li
//     Created: 2019-09-13
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_LOCAL_FRAME_CLIENT_IMPL_H
#define BLINKIT_BLINKIT_LOCAL_FRAME_CLIENT_IMPL_H

#pragma once

#include "bk_def.h"
#include "blinkit/app/caller.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"

namespace BlinKit {

class ClientCaller;

class LocalFrameClientImpl : public blink::LocalFrameClient
{
protected:
    LocalFrameClientImpl(AppCaller &appCaller, ClientCaller &clientCaller)
        : m_appCaller(appCaller), m_clientCaller(clientCaller)
    {
    }

#ifndef NDEBUG
    bool IsClientThread(void) const { return m_clientCaller.IsClientThread(); }
#endif

    int CallJS(blink::LocalFrame *frame, BkJSCallback callback, void *userData);

    virtual void DidFinishLoad(void) = 0;
    String UserAgent(void) override;

    AppCaller &m_appCaller;
    ClientCaller &m_clientCaller;
private:
    bool HasWebView(void) const override { return false; }
    void Detached(blink::FrameDetachType) override {}
    blink::DocumentLoader* CreateDocumentLoader(blink::LocalFrame *frame, const blink::ResourceRequest &request,
        const blink::SubstituteData &data, std::unique_ptr<blink::WebDocumentLoader::ExtraData> extraData) override;
    void DispatchWillSendRequest(blink::ResourceRequest &request) override {}
    void DidCreateNewDocument(void) override {}
    void DispatchDidFailProvisionalLoad(const blink::ResourceError &error) override {}
    void DispatchDidFailLoad(const blink::ResourceError &error) override {}
    void DispatchDidFinishDocumentLoad(void) override {}
    void DispatchDidHandleOnloadEvents(void) override {}
    void DispatchDidFinishLoad(void) override final;
    void WillBeDetached(void) override {}
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_LOCAL_FRAME_CLIENT_IMPL_H
