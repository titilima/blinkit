// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_context.cpp
// Description: CrawlerContext Class
//      Author: Ziming Li
//     Created: 2022-03-05
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#include "./crawler_context.h"

#include "blinkit/blink/renderer/bindings/core/qjs/qjs_bindings.h"
#include "blinkit/blink/renderer/core/frame/LocalFrame.h"
#include "blinkit/blink/renderer/core/loader/DocumentLoader.h"
#include "blinkit/crawler/crawler_impl.h"

namespace BlinKit {

CrawlerContext::CrawlerContext(LocalFrame &frame) : ScriptController(frame)
{
}

bool CrawlerContext::canExecuteScripts(ReasonForCallingCanExecuteScripts reason)
{
    FrameLoader &loader = m_frame.loader();

    const KURL &url = loader.documentLoader()->url();
    return static_cast<CrawlerImpl *>(loader.client())->ScriptEnabled(url.spec());
}

void CrawlerContext::OnContextCreated(JSContext *ctx, JSValue global, Prototypes &prototypes)
{
    using namespace qjs;

    prototypes.prototypes[PROTO_Window] = CreateWindowPrototypeForCrawler(ctx);

    JSValue protoEventTarget = CreateEventTargetPrototype(ctx);
    RegisterEventTargetPrototypes(ctx, prototypes, protoEventTarget);
    JS_FreeValue(ctx, protoEventTarget);

    prototypes.prototypes[PROTO_Location] = CreateLocationPrototypeForCrawler(ctx);
}

void CrawlerContext::RegisterContainerNodePrototypes(JSContext *ctx, Prototypes &dst, JSValue protoContainerNode)
{
    using namespace qjs;

    dst.prototypes[PROTO_Document] = CreateDocumentPrototypeForCrawler(ctx, protoContainerNode);
    dst.prototypes[PROTO_Element] = CreateElementPrototypeForCrawler(ctx, protoContainerNode);
    RegisterElementPrototypes(dst.elements, ctx, dst.prototypes[PROTO_Element]);
}

void CrawlerContext::RegisterElementPrototypes(ElementPrototypes &dst, JSContext *ctx, JSValue elementPrototype)
{
    // BKTODO:
}

void CrawlerContext::RegisterEventTargetPrototypes(JSContext *ctx, Prototypes &dst, JSValue protoEventTarget)
{
    JSValue protoNode = qjs::CreateNodePrototypeForCrawler(ctx, protoEventTarget);
    RegisterNodePrototypes(ctx, dst, protoNode);
    JS_FreeValue(ctx, protoNode);
}

void CrawlerContext::RegisterNodePrototypes(JSContext *ctx, Prototypes &dst, JSValue protoNode)
{
    using namespace qjs;

    JSValue protoContainerNode = CreateContainerNodePrototypeForCrawler(ctx, protoNode);
    RegisterContainerNodePrototypes(ctx, dst, protoContainerNode);
    JS_FreeValue(ctx, protoContainerNode);
}

} // namespace BlinKit
