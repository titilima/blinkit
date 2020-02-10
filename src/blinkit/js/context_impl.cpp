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
#include "third_party/blink/renderer/bindings/core/duk/duk_document.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_window.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"

using namespace blink;
using namespace BlinKit;

static const char CrawlerObject[] = "crawlerObject";
static const char Globals[] = "globals";

static void DefaultLog(const char *log)
{
    BkLog("%s", log);
}

ContextImpl::ContextImpl(const LocalFrame &frame)
    : m_frame(frame)
    , m_ctx(duk_create_heap_default())
    , m_logger(std::bind(DefaultLog, std::placeholders::_1))
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

bool ContextImpl::CreateCrawlerObject(const BkCrawlerClient &crawlerClient)
{
    if (nullptr != crawlerClient.ConsoleLog)
        m_logger = std::bind(crawlerClient.ConsoleLog, std::placeholders::_1, crawlerClient.UserData);

    if (nullptr == crawlerClient.GetUserScript)
        return false;

    std::string userScript;
    crawlerClient.GetUserScript(BkMakeBuffer(userScript), crawlerClient.UserData);
    base::TrimWhitespaceASCII(userScript, base::TRIM_ALL, &userScript);
    if (userScript.empty())
        return false;

    bool ret = true;
    std::string log;
    const auto callback = [&crawlerClient, &ret, &log](duk_context *ctx)
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
        log.assign(s, len);
    };
    Eval(userScript, callback, nullptr);

    if (!ret)
        m_logger(log.c_str());
    ASSERT(ret);
    return ret;
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

void ContextImpl::InitializeHeapStash(void)
{
    duk_push_heap_stash(m_ctx);

    duk_push_global_object(m_ctx);
    duk_put_prop_string(m_ctx, -2, Globals);

#ifdef BLINKIT_CRAWLER_ONLY
    ASSERT(m_frame.Client()->IsCrawler());
    RegisterPrototypesForCrawler(m_ctx);
    CreateCrawlerObject(ToCrawlerImpl(m_frame.Client())->Client());
#else
    if (frame.Client()->IsCrawler())
    {
        RegisterPrototypesForCrawler(m_ctx);
        CreateCrawlerObject(ToCrawlerImpl(frame.Client())->Client());
    }
    else
    {
        ASSERT(false); // BKTODO:
    }
#endif

    duk_pop(m_ctx);
}

void ContextImpl::RegisterPrototypesForCrawler(duk_context *ctx)
{
    PrototypeHelper helper(ctx);
    DukDocument::RegisterPrototypeForCrawler(helper);
    DukWindow::RegisterPrototypeForCrawler(helper);
}

void ContextImpl::Reset(void)
{
    const duk_idx_t idx = DukScriptObject::Create<DukWindow>(m_ctx, *(m_frame.DomWindow()));
    ExposeGlobals(m_ctx, idx);

    duk_set_global_object(m_ctx);
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
