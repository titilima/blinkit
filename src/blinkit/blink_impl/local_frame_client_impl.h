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

#include "third_party/blink/renderer/core/frame/local_frame_client.h"

namespace BlinKit {

class LocalFrameClientImpl : public blink::LocalFrameClient
{
protected:
    String UserAgent(void) override;
private:
    bool HasWebView(void) const override { return false; }
    void Detached(blink::FrameDetachType) override {}
    std::unique_ptr<blink::DocumentLoader> CreateDocumentLoader(blink::LocalFrame *frame,
        const blink::ResourceRequest &request, const blink::SubstituteData &data,
        std::unique_ptr<blink::WebDocumentLoader::ExtraData> extraData) override;
    void DispatchWillSendRequest(blink::ResourceRequest &request) override {}
    void DidCreateNewDocument(void) override {}
    void DispatchDidFailProvisionalLoad(const blink::ResourceError &error) override {}
    void DispatchDidFailLoad(const blink::ResourceError &error) override {}
    void DispatchDidFinishDocumentLoad(void) override {}
    void DispatchDidHandleOnloadEvents(void) override {}
    void WillBeDetached(void) override {}
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_LOCAL_FRAME_CLIENT_IMPL_H
