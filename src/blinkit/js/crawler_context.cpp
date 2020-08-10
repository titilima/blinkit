// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: crawler_context.cpp
// Description: CrawlerContext Class
//      Author: Ziming Li
//     Created: 2020-08-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "crawler_context.h"

#include "base/strings/string_util.h"
#include "blinkit/crawler/crawler_impl.h"
#include "blinkit/js/js_value_impl.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_anchor_element.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_attr.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_console.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_document.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_event.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_image_element.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_location.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_navigator.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_script_element.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_window.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_xhr.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"

using namespace blink;

namespace BlinKit {

CrawlerContext::CrawlerContext(const LocalFrame &frame) : BrowserContext(frame, DukElement::PrototypeMapForCrawler())
{
    BrowserContext::InitializeHeapStash(RegisterPrototypes);

    CrawlerImpl *crawler = ToCrawlerImpl(GetFrame().Client());
    crawler->ApplyConsoleMessager(m_consoleMessager);

    InitializeNewGlobalObject(*crawler);
}

CrawlerContext::~CrawlerContext(void) = default;

void CrawlerContext::CreateUserObject(const CrawlerImpl &crawler)
{
    if (!m_objectScript.has_value())
    {
        std::string objectScript;
        if (crawler.GetConfig(BK_CFG_OBJECT_SCRIPT, objectScript))
            base::TrimWhitespaceASCII(objectScript, base::TRIM_ALL, &objectScript);
        m_objectScript = objectScript;
    }

    if (m_objectScript->empty())
        return;

    m_userObject.reset();

    std::string errorLog;
    const auto callback = [this, &errorLog](duk_context *ctx)
    {
        if (!duk_is_error(ctx, -1))
        {
            if (duk_is_object(ctx, -1))
            {
                ASSERT(!m_userObject);
                m_userObject = std::make_unique<JSObjectImpl>(ctx, -1);
                return;
            }

            duk_type_error(ctx, "Object type expected.");
        }

        size_t len = 0;
        const char *s = duk_safe_to_lstring(ctx, -1, &len);
        errorLog.assign(s, len);
    };
    Eval(*m_objectScript, callback, "UserScript");

    if (!errorLog.empty())
        m_consoleMessager(BK_CONSOLE_ERROR, errorLog.c_str());
}

JSObjectImpl* CrawlerContext::GetContextObject(int callContext)
{
    if (BK_CTX_USER_OBJECT == callContext)
        return m_userObject.get();
    return BrowserContext::GetContextObject(callContext);
}

void CrawlerContext::InitializeNewGlobalObject(const CrawlerImpl &crawler)
{
    CreateUserObject(crawler);
    RegisterFunctions();
}

void CrawlerContext::RegisterPrototypes(duk_context *ctx)
{
    PrototypeHelper helper(ctx);
    DukAnchorElement::RegisterPrototypeForCrawler(helper);
    DukAttr::RegisterPrototype(helper);
    DukNode::RegisterPrototype(helper, ProtoNames::Comment);
    DukConsole::RegisterPrototype(helper);
    DukDocument::RegisterPrototypeForCrawler(helper);
    DukNode::RegisterPrototype(helper, ProtoNames::DocumentFragment);
    DukElement::RegisterPrototypeForCrawler(helper);
    DukEvent::RegisterPrototype(helper);
    DukImageElement::RegisterPrototypeForCrawler(helper);
    DukLocation::RegisterPrototype(helper);
    DukNavigator::RegisterPrototype(helper);
    DukNode::RegisterPrototype(helper, ProtoNames::DocumentType);
    DukNode::RegisterPrototype(helper, ProtoNames::Text);
    DukScriptElement::RegisterPrototypeForCrawler(helper);
    DukWindow::RegisterPrototypeForCrawler(helper);
    DukXHR::RegisterPrototype(helper);
}

void CrawlerContext::UpdateDocument(void)
{
    BrowserContext::UpdateDocument();

    CrawlerImpl *crawler = ToCrawlerImpl(GetFrame().Client());
    InitializeNewGlobalObject(*crawler);

    crawler->ProcessDocumentReset();
}

} // namespace BlinKit
