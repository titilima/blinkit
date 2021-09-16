// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_context.h
// Description: CrawlerContext Class
//      Author: Ziming Li
//     Created: 2020-08-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CRAWLER_CONTEXT_H
#define BLINKIT_BLINKIT_CRAWLER_CONTEXT_H

#pragma once

#include "blinkit/blink/renderer/bindings/core/duk/script_controller.h"

class CrawlerImpl;

namespace BlinKit {

class CrawlerContext final : public blink::ScriptController
{
public:
    CrawlerContext(blink::LocalFrame &frame);
    ~CrawlerContext(void) override;
private:
    void CreateUserObject(duk_idx_t globalStashIndex);
    static void RegisterPrototypes(duk_context *ctx, duk_idx_t globalStashIndex);

    bool IsDukSessionDirty(void) const override;
    void Attach(duk_context *ctx, duk_idx_t globalStashIndex) override;
    void Detach(duk_context *ctx) override;
    void* GetUserObject(void) override { return m_userObject; }
    // BKTODO: bool ScriptEnabled(const std::string &URL) const override;
    void ConsoleOutput(int type, const char *msg) override;

    // BKTODO: CrawlerImpl &m_crawler;
    void *m_userObject = nullptr;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_CONTEXT_H
