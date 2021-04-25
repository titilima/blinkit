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

CrawlerContext::CrawlerContext(LocalFrame &frame)
    : ScriptController(frame, DukElement::PrototypeMapForCrawler())
    , m_crawler(*ToCrawlerImpl(frame.Client()))
{
}

CrawlerContext::~CrawlerContext(void) = default;

void CrawlerContext::Attach(duk_context *ctx, duk_idx_t globalStashIndex)
{
    ASSERT(nullptr == m_userObject);
    ScriptController::Attach(ctx, globalStashIndex);

    Document *document = GetFrame().GetDocument();
    if (nullptr == document)
        return;

    std::string objectScript;
    if (m_crawler.GetConfig(BK_CFG_OBJECT_SCRIPT, objectScript))
        base::TrimWhitespaceASCII(objectScript, base::TRIM_ALL, &objectScript);
    if (objectScript.empty())
        return;

    std::string errorLog;
    const auto callback = [this, &errorLog](duk_context *ctx)
    {
        if (!duk_is_error(ctx, -1))
        {
            if (duk_is_object(ctx, -1))
            {
                m_userObject = duk_get_heapptr(ctx, -1);
                return;
            }

            duk_type_error(ctx, "Object type expected.");
        }

        size_t len = 0;
        const char *s = duk_safe_to_lstring(ctx, -1, &len);
        errorLog.assign(s, len);
    };
    Eval(objectScript, callback, "UserScript");

    if (!errorLog.empty())
        ConsoleOutput(BK_CONSOLE_ERROR, errorLog.c_str());
}

void CrawlerContext::ConsoleOutput(int type, const char *msg)
{
    if (!m_crawler.ProcessConsoleMessage(type, msg))
        ScriptController::ConsoleOutput(type, msg);
}

void CrawlerContext::Detach(duk_context *ctx)
{
    m_userObject = nullptr;
    ScriptController::Detach(ctx);
}

void CrawlerContext::RegisterPrototypes(duk_context *ctx, duk_idx_t globalStashIndex)
{
    PrototypeHelper helper(ctx, globalStashIndex);
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

} // namespace BlinKit
