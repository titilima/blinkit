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

namespace BlinKit {

CrawlerContext::CrawlerContext(LocalFrame &frame, JSRuntime *runtime) : ScriptController(frame, runtime)
{
}

void CrawlerContext::FillElementPrototypes(ElementPrototypes &dst, JSContext *ctx, JSValue elementPrototype)
{
    // BKTODO:
}

void CrawlerContext::OnContextCreated(JSContext *ctx, JSValue global, Prototypes &prototypes)
{
    using namespace qjs;

    prototypes.window = CreateWindowPrototypeForCrawler(ctx);
    prototypes.eventTarget = CreateEventTargetPrototype(ctx);
    prototypes.node = CreateNodePrototypeForCrawler(ctx, prototypes.eventTarget);
    prototypes.containerNode = CreateContainerNodePrototypeForCrawler(ctx, prototypes.node);
    prototypes.document = CreateDocumentPrototypeForCrawler(ctx, prototypes.containerNode);

    prototypes.genericElement = CreateElementPrototypeForCrawler(ctx, prototypes.containerNode);
    FillElementPrototypes(prototypes.elements, ctx, prototypes.genericElement);
}

} // namespace BlinKit
