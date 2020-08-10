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

#include "blinkit/js/browser_context.h"

namespace BlinKit {

class CrawlerContext final : public BrowserContext
{
public:
    CrawlerContext(const blink::LocalFrame &frame);
    ~CrawlerContext(void) override;

    JSObjectImpl* UserObject(void) { return m_userObject.get(); }
private:
    static void RegisterPrototypes(duk_context *ctx);
    void CreateUserObject(const CrawlerImpl &crawler);
    void InitializeNewGlobalObject(const CrawlerImpl &crawler);

    JSObjectImpl* GetContextObject(int callContext) override;
    void UpdateDocument(void) override;

    std::optional<std::string> m_objectScript;
    std::unique_ptr<JSObjectImpl> m_userObject;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CRAWLER_CONTEXT_H
