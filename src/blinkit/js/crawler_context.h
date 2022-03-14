#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_context.h
// Description: CrawlerContext Class
//      Author: Ziming Li
//     Created: 2022-03-05
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_CRAWLER_CONTEXT_H
#define BLINKIT_CRAWLER_CONTEXT_H

#include "blinkit/blink/renderer/bindings/core/script_controller.h"

namespace BlinKit {

class CrawlerContext final : public ScriptController
{
public:
    CrawlerContext(LocalFrame &frame);
private:
    static void RegisterEventTargetPrototypes(JSContext *ctx, Prototypes &dst, JSValue protoEventTarget);
    static void RegisterNodePrototypes(JSContext *ctx, Prototypes &dst, JSValue protoNode);
    static void RegisterContainerNodePrototypes(JSContext *ctx, Prototypes &dst, JSValue protoContainerNode);
    static void RegisterElementPrototypes(ElementPrototypes &dst, JSContext *ctx, JSValue elementPrototype);

    bool canExecuteScripts(ReasonForCallingCanExecuteScripts reason) override;
    void OnContextCreated(JSContext *ctx, JSValue global, Prototypes &prototypes) override;
};

} // namespace BlinKit

#endif // BLINKIT_CRAWLER_CONTEXT_H
