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

#include "./crawler_context.h"

#include "base/strings/string_util.h"
#include "blinkit/crawler/crawler_impl.h"
#include "blinkit/js/js_value_impl.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_anchor_element.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_attr.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_console.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_document.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_event.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_image_element.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_location.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_navigator.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_script_element.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_window.h"
#include "blinkit/blink/renderer/bindings/core/duk/duk_xhr.h"
#include "blinkit/blink/renderer/core/frame/LocalFrame.h"

using namespace blink;

namespace BlinKit {

static const char UserObject[] = "userObject";

CrawlerContext::CrawlerContext(LocalFrame &frame)
    : ScriptController(frame, DukElement::PrototypeMapForCrawler())
    // BKTODO: , m_crawler(*ToCrawlerImpl(frame.Client()))
{
    ASSERT(false); // BKTODO:
}

CrawlerContext::~CrawlerContext(void) = default;

void CrawlerContext::Attach(duk_context *ctx, duk_idx_t globalStashIndex)
{
    ScriptController::Attach(ctx, globalStashIndex);
    CreateUserObject(globalStashIndex);
    RegisterPrototypes(ctx, globalStashIndex);

    ASSERT(false); // BKTODO: m_crawler.CleanupDirtyFlag();
}

void CrawlerContext::ConsoleOutput(int type, const char *msg)
{
    ASSERT(false); // BKTODO:
#if 0
    if (!m_crawler.ProcessConsoleMessage(type, msg))
        ScriptController::ConsoleOutput(type, msg);
#endif
}

void CrawlerContext::CreateUserObject(duk_idx_t globalStashIndex)
{
    ASSERT(nullptr == m_userObject);

    ASSERT(false); // BKTODO:
#if 0
    Document *document = GetFrame().GetDocument();
    if (nullptr == document)
        return;

    std::string objectScript;
    if (m_crawler.GetConfig(BK_CFG_OBJECT_SCRIPT, objectScript))
        base::TrimWhitespaceASCII(objectScript, base::TRIM_ALL, &objectScript);
    if (objectScript.empty())
        return;

    std::string errorLog;
    const auto callback = [this, globalStashIndex, &errorLog](duk_context *ctx) {
        if (!duk_is_error(ctx, -1))
        {
            if (duk_is_object(ctx, -1))
            {
                m_userObject = duk_get_heapptr(ctx, -1);
                duk_put_prop_string(ctx, globalStashIndex, UserObject);
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
#endif
}

void CrawlerContext::Detach(duk_context *ctx)
{
    m_userObject = nullptr;
    ScriptController::Detach(ctx);
}

bool CrawlerContext::IsDukSessionDirty(void) const
{
    ASSERT(false); // BKTODO: return m_crawler.DirtyFlag();
    return false;
}

void CrawlerContext::RegisterPrototypes(duk_context *ctx, duk_idx_t globalStashIndex)
{
    PrototypeHelper helper(ctx, globalStashIndex);
    DukAnchorElement::RegisterPrototypeForCrawler(helper);
    DukAttr::RegisterPrototype(helper);
    DukNode::RegisterPrototype(helper, ProtoNames::Comment);
    DukConsole::RegisterPrototype(helper);
    DukCrawlerDocument::RegisterPrototype(helper);
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

#if 0 // BKTODO:
bool CrawlerContext::ScriptEnabled(const std::string &URL) const
{
    ASSERT(false); // BKTODO: return m_crawler.ScriptEnabled(URL);
    return false;
}
#endif

} // namespace BlinKit
