// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: context_impl.cpp
// Description: ContextImpl Class
//      Author: Ziming Li
//     Created: 2020-01-21
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "context_impl.h"

#include "base/strings/string_util.h"
#include "blinkit/crawler/crawler_impl.h"
#include "blinkit/js/js_value_impl.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_attr.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_console.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_document.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_event.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_location.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_navigator.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_script_element.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_window.h"
#include "third_party/blink/renderer/bindings/core/duk/script_controller.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"

using namespace blink;
using namespace BlinKit;

static const char CrawlerObject[] = "crawlerObject";
static const char Globals[] = "globals";
static const char NativeContext[] = "nativeContext";

static void DefaultConsoleOutput(int type, const char *msg)
{
    BkLog("%s", msg);
}

ContextImpl::ContextImpl(const LocalFrame &frame)
    : m_frame(frame)
    , m_ctx(duk_create_heap_default())
    , m_consoleMessager(std::bind(DefaultConsoleOutput, std::placeholders::_1, std::placeholders::_2))
#ifdef BLINKIT_CRAWLER_ONLY
    , m_prototypeMap(DukElement::PrototypeMapForCrawler())
#else
#endif
{
    InitializeHeapStash();
}

ContextImpl::~ContextImpl(void)
{
    duk_destroy_heap(m_ctx);
}

bool ContextImpl::AccessCrawler(const Callback &worker)
{
    const duk_idx_t top = duk_get_top(m_ctx);

    bool ret = true;
    duk_push_heap_stash(m_ctx);
    if (duk_get_prop_string(m_ctx, -1, CrawlerObject))
        worker(m_ctx);
    else
        ret = false;

    duk_set_top(m_ctx, top);
    return ret;
}

void ContextImpl::CreateCrawlerObject(const CrawlerImpl &crawler)
{
    do {
        std::string objectScript;
        if (crawler.GetConfig(BK_CFG_OBJECT_SCRIPT, objectScript))
            base::TrimWhitespaceASCII(objectScript, base::TRIM_ALL, &objectScript);
        if (objectScript.empty())
        {
            BKLOG("Default crawler object created.");
            break;
        }

        std::string errorLog;
        const auto callback = [&errorLog](duk_context *ctx)
        {
            if (!duk_is_error(ctx, -1))
            {
                if (duk_is_object(ctx, -1))
                {
                    duk_put_prop_string(ctx, -2, CrawlerObject);
                    return;
                }

                duk_type_error(ctx, "Object type expected.");
            }

            size_t len = 0;
            const char *s = duk_safe_to_lstring(ctx, -1, &len);
            errorLog.assign(s, len);
        };
        Eval(objectScript, callback, nullptr);

        if (errorLog.empty())
            return;

        m_consoleMessager(BK_CONSOLE_ERROR, errorLog.c_str());
    } while (false);

    duk_push_object(m_ctx);
    duk_put_prop_string(m_ctx, -2, CrawlerObject);
}

void ContextImpl::Eval(const std::string_view code, const Callback &callback, const char *fileName)
{
    const duk_idx_t top = duk_get_top(m_ctx);

    int r;
    if (nullptr == fileName || '\0' == *fileName)
    {
        r = duk_pcompile_lstring(m_ctx, 0, code.data(), code.length());
    }
    else
    {
        duk_push_string(m_ctx, fileName);
        r = duk_pcompile_lstring_filename(m_ctx, 0, code.data(), code.length());
    }

    if (DUK_EXEC_SUCCESS == r)
        r = duk_pcall(m_ctx, 0);
    callback(m_ctx);

    duk_set_top(m_ctx, top);
}

void ContextImpl::ExposeGlobals(duk_context *ctx, duk_idx_t dst)
{
    const duk_idx_t top = duk_get_top(ctx);

    duk_push_heap_stash(ctx);
    duk_get_prop_string(ctx, -1, Globals);

    static const duk_uint_t flags =
        DUK_ENUM_OWN_PROPERTIES_ONLY
        | DUK_ENUM_INCLUDE_NONENUMERABLE
        | DUK_ENUM_INCLUDE_SYMBOLS;
    duk_enum(ctx, -1, flags);
    while (duk_next(ctx, -1, true))
        duk_put_prop(ctx, dst);

    duk_set_top(ctx, top);
}

ContextImpl* ContextImpl::From(duk_context *ctx)
{
    ContextImpl *ret = nullptr;

    duk_push_heap_stash(ctx);
    duk_get_prop_string(ctx, -1, NativeContext);
    ret = reinterpret_cast<ContextImpl *>(duk_get_pointer(ctx, -1));
    duk_pop_2(ctx);

    return ret;
}

ContextImpl* ContextImpl::From(ExecutionContext *executionContext)
{
    if (executionContext->IsDocument())
    {
        Document *document = static_cast<Document *>(executionContext);
        return document->GetFrame()->GetScriptController().GetContext();
    }

    NOTREACHED();
    return nullptr;
}

GCPool& ContextImpl::GetGCPool(void)
{
    return m_frame.GetGCPool();
}

void ContextImpl::InitializeHeapStash(void)
{
    duk_push_heap_stash(m_ctx);

    duk_push_pointer(m_ctx, this);
    duk_put_prop_string(m_ctx, -2, NativeContext);

    duk_push_global_object(m_ctx);
    duk_put_prop_string(m_ctx, -2, Globals);

#ifdef BLINKIT_CRAWLER_ONLY
    ASSERT(m_frame.Client()->IsCrawler());

    RegisterPrototypesForCrawler(m_ctx);

    CrawlerImpl *crawler = ToCrawlerImpl(m_frame.Client());
    crawler->ApplyConsoleMessager(m_consoleMessager);
    CreateCrawlerObject(*crawler);
#else
    if (frame.Client()->IsCrawler())
    {
        RegisterPrototypesForCrawler(m_ctx);

        CrawlerImpl *crawler = ToCrawlerImpl(m_frame.Client());
        crawler->ApplyLogger(m_logger);
        CreateCrawlerObject(*crawler);
    }
    else
    {
        ASSERT(false); // BKTODO:
    }
#endif

    duk_pop(m_ctx);
}

const char* ContextImpl::LookupPrototypeName(const std::string &tagName) const
{
    auto it = m_prototypeMap.find(tagName);
    if (std::end(m_prototypeMap) == it)
        return DukElement::ProtoName;
    return it->second;
}

void ContextImpl::RegisterPrototypesForCrawler(duk_context *ctx)
{
    PrototypeHelper helper(ctx);
    DukAttr::RegisterPrototype(helper);
    DukNode::RegisterPrototype(helper, ProtoNames::Comment);
    DukConsole::RegisterPrototype(helper);
    DukDocument::RegisterPrototypeForCrawler(helper);
    DukNode::RegisterPrototype(helper, ProtoNames::DocumentFragment);
    DukElement::RegisterPrototypeForCrawler(helper);
    DukEvent::RegisterPrototype(helper);
    DukLocation::RegisterPrototype(helper);
    DukNavigator::RegisterPrototype(helper);
    DukNode::RegisterPrototype(helper, ProtoNames::DocumentType);
    DukNode::RegisterPrototype(helper, ProtoNames::Text);
    DukScriptElement::RegisterPrototypeForCrawler(helper);
    DukWindow::RegisterPrototypeForCrawler(helper);
}

void ContextImpl::Reset(void)
{
    const duk_idx_t idx = DukScriptObject::Create<DukWindow>(m_ctx, *(m_frame.DomWindow()));
    ExposeGlobals(m_ctx, idx);
    duk_set_global_object(m_ctx);

#ifdef BLINKIT_CRAWLER_ONLY
    ASSERT(m_frame.Client()->IsCrawler());
#else
    if (!m_frame.Client()->IsCrawler())
        return;
#endif

    ToCrawlerImpl(m_frame.Client())->ProcessDocumentReset();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT BkJSValue BKAPI BkJSEvaluate(BkJSContext context, const char *code, unsigned flags)
{
    JSValueImpl *ret = nullptr;

    const auto callback = [flags, &ret](duk_context *ctx)
    {
        if (0 == (BK_EVAL_IGNORE_RETURN_VALUE & flags))
            ret = JSValueImpl::Create(ctx, -1);
    };
    context->Eval(code, callback);

    return ret;
}

} // extern "C"
