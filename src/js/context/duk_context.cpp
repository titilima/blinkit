// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_context.cpp
// Description: DukContext Class
//      Author: Ziming Li
//     Created: 2019-03-07
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_context.h"

#include "core/frame/FrameClient.h"
#include "core/frame/LocalFrame.h"

#include "blinkit/crawler/crawler_impl.h"

#include "bindings/duk_console.h"
#include "bindings/duk_document.h"
#include "bindings/duk_element.h"
#include "bindings/duk_exception_state.h"
#include "bindings/duk_window.h"
#include "context/caller_context_impl.h"
#include "context/function_manager.h"
#include "context/prototype_manager.h"
#include "context/value_impl.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

namespace StashFields {
static const char Context[] = "context";
static const char CrawlerObject[] = "crawlerObject";
#ifndef BLINKIT_CRAWLER_ONLY
static const char External[] = "external";
#endif
static const char Globals[] = "globals";
static const char NativeCrawler[] = "nativeCrawler";
}

namespace Crawler {

static duk_ret_t Eval(duk_context *ctx)
{
    int top = duk_get_top(ctx);

    // Check incantation first.
    duk_push_heap_stash(ctx);
    duk_get_prop_string(ctx, -1, StashFields::CrawlerObject);
    if (duk_get_prop_string(ctx, -1, "incantation") && duk_equals(ctx, 0, -1))
    {
        duk_pop(ctx);
        return 1;
    }

    duk_set_top(ctx, top);
    duk_peval(ctx);
    return 1;
}

static duk_ret_t Gather(duk_context *ctx)
{
    CrawlerImpl *crawler = DukContext::From(ctx)->GetCrawler();
    crawler->Client().DataGathered(crawler, ValueImpl(ctx, 0));
    return 0;
}

static duk_ret_t TextContentBySelector(duk_context *ctx)
{
    duk_push_global_object(ctx);
    DOMWindow *window = DukEventTarget::GetNativeThis<DOMWindow>(ctx);

    DukExceptionState es(ctx, "querySelector", "Document");
    PassRefPtr<Element> e = window->document()->querySelector(Duk::ToAtomicString(ctx, 0), es);
    if (es.hadException())
    {
        es.throwIfNeeded();
        return 0;
    }

    String ret;
    if (e)
        ret = e->textContent();
    Duk::PushString(ctx, ret);
    duk_trim(ctx, -1);
    return 1;
}

} // namespace Crawler

DukContext::DukContext(LocalFrame &frame)
    : m_frame(frame), m_context(duk_create_heap_default())
{
    Attach();
    Initialize();
}

DukContext::~DukContext(void)
{
    duk_destroy_heap(m_context);
}

int DukContext::AccessCrawlerMember(const char *name, BkCallback &callback)
{
    Duk::StackKeeper sk(m_context);
    duk_push_heapptr(m_context, m_crawlerObjectPtr);
    if (!duk_get_prop_string(m_context, -1, name))
        return BkError::NotFound;

    callback.OnReturn(ValueImpl(m_context));
    return BkError::Success;
}

void DukContext::AdjustGlobalsForCrawler(duk_context *ctx)
{
    static const char eval[] = "eval";

    duk_del_prop_string(ctx, -1, eval);
    duk_push_c_function(ctx, Crawler::Eval, 1);
    duk_put_prop_string(ctx, -2, eval);
}

void DukContext::Attach(void)
{
#ifdef BLINKIT_CRAWLER_ONLY
    assert(m_frame.client()->IsCrawler());
    bool isCrawler = true;
#else
    bool isCrawler = m_frame.client()->IsCrawler();
#endif

    Duk::StackKeeper sk(m_context);
    duk_push_heap_stash(m_context);

    duk_push_pointer(m_context, this);
    duk_put_prop_string(m_context, -2, StashFields::Context);

    if (isCrawler)
    {
        CrawlerImpl *nativeCrawler = toCrawlerImpl(m_frame.client());
        duk_push_pointer(m_context, nativeCrawler);
        duk_put_prop_string(m_context, -2, StashFields::NativeCrawler);
    }

    duk_push_global_object(m_context);
    if (isCrawler)
        AdjustGlobalsForCrawler(m_context);
    m_globalsPtr = duk_get_heapptr(m_context, -1);
    duk_put_prop_string(m_context, -2, StashFields::Globals);

    m_prototypeManager = std::make_unique<PrototypeManager>(m_context);
    if (isCrawler)
        RegisterPrototypesForCrawler();
#ifndef BLINKIT_CRAWLER_ONLY
    else
        RegisterPrototypes();

    if (!isCrawler)
    {
        duk_push_object(m_context);
        m_functionManager = std::make_unique<FunctionManager>(m_context);
        duk_put_prop_string(m_context, -2, StashFields::External);
    }
#endif
}

int DukContext::CallCrawler(const char *method, BkCallback *callback)
{
    if (nullptr == m_crawlerObjectPtr)
        return BkError::NotFound;

    duk_push_heapptr(m_context, m_crawlerObjectPtr);

    CallerContextImpl context(m_context);
    context.SetAsThisCall();
    return context.Call(method, callback);
}

int DukContext::CallFunction(const char *name, BkCallback *callback)
{
    duk_push_global_object(m_context);

    CallerContextImpl context(m_context);
    return context.Call(name, callback);
}

int DukContext::CreateCrawlerObject(const char *script, size_t length)
{
    std::string code(1, '(');
    code.append(script, length);
    code.push_back(')');

    Duk::StackKeeper sk(m_context);
    duk_push_heap_stash(m_context);

    int r = duk_peval_lstring(m_context, code.data(), code.length());
    if (DUK_EXEC_SUCCESS != r)
    {
#ifdef _DEBUG
        duk_get_prop_string(m_context, -1, "message");
        BKLOG("ERROR: %s", duk_to_string(m_context, -1));
#endif
        return Duk::ToErrorCode(m_context);
    }

    if (DUK_TYPE_OBJECT != duk_get_type(m_context, -1))
        return BkError::TypeError;

    m_crawlerObjectPtr = duk_get_heapptr(m_context, -1);

    duk_push_c_function(m_context, Crawler::Gather, 1);
    duk_put_prop_string(m_context, -2, "gather");
    duk_push_c_function(m_context, Crawler::TextContentBySelector, 1);
    duk_put_prop_string(m_context, -2, "textContentBySelector");

    m_functionManager = std::make_unique<FunctionManager>(m_context);
    duk_put_prop_string(m_context, -2, StashFields::CrawlerObject);
    return BkError::Success;
}

void DukContext::CreateObject(const char *protoName, ScriptWrappable *nativeThis, void(*createCallback)(ScriptWrappable *))
{
    if (m_prototypeManager->CreateObject(m_context, protoName))
    {
        if (nullptr != nativeThis)
        {
            m_objectPool[nativeThis] = duk_get_heapptr(m_context, -1);
            Duk::BindNativeThis(m_context, nativeThis);
            createCallback(nativeThis);
        }
    }
    else
    {
        duk_push_undefined(m_context);
    }
}

int DukContext::Eval(const char *code, size_t length, BkCallback *callback, const char *fileName)
{
    if (0 == length)
        length = strlen(code);

    Duk::StackKeeper sk(m_context);

    if (nullptr == fileName)
        fileName = "eval";
    duk_push_string(m_context, fileName);

    int r = duk_pcompile_lstring_filename(m_context, 0, code, length);
    if (DUK_EXEC_SUCCESS == r)
        r = duk_pcall(m_context, 0);

    ValueImpl retVal(m_context);
    if (DUK_EXEC_SUCCESS != r)
        retVal.SetAsErrorType();
    if (nullptr != callback)
        callback->OnReturn(retVal);

    return retVal.ErrorCode();
}

DukContext* DukContext::From(duk_context *ctx)
{
    Duk::StackKeeper sk(ctx);
    duk_push_heap_stash(ctx);
    if (!duk_get_prop_string(ctx, -1, StashFields::Context))
        return nullptr;
    return reinterpret_cast<DukContext *>(duk_get_pointer(ctx, -1));
}

CrawlerImpl* DukContext::GetCrawler(void)
{
    assert(m_frame.client()->IsCrawler());
    Duk::StackKeeper sk(m_context);
    duk_push_heap_stash(m_context);
    duk_get_prop_string(m_context, -1, StashFields::NativeCrawler);
    return reinterpret_cast<CrawlerImpl *>(duk_to_pointer(m_context, -1));
}

void DukContext::GetCrawlerProperty(const char *name, const std::function<void(const BkValue &)> &callback)
{
    if (nullptr == m_crawlerObjectPtr)
        return;

    Duk::StackKeeper sk(m_context);
    duk_push_heapptr(m_context, m_crawlerObjectPtr);
    if (duk_get_prop_string(m_context, -1, name))
    {
        ValueImpl prop(m_context);
        callback(prop);
    }
}

void DukContext::Initialize(void)
{
    CreateObject<DukWindow>(m_frame.domWindow());
    BKLOG("Window object created: %x", duk_get_heapptr(m_context, -1));
    PrepareGlobalsToTop();
    duk_set_global_object(m_context);
}

void DukContext::PrepareGlobalsToTop(void)
{
    Duk::StackKeeper sk(m_context);
    int idxNewGlobal = duk_get_top_index(m_context);

    duk_push_heapptr(m_context, m_globalsPtr);
    // ... newGlobal globals
    duk_enum(m_context, -1, DUK_ENUM_OWN_PROPERTIES_ONLY | DUK_ENUM_INCLUDE_NONENUMERABLE | DUK_ENUM_INCLUDE_SYMBOLS);
    // ... newGlobal globals enum
    while (duk_next(m_context, -1, true))
    {
        // ... newGlobal globals enum key value
#if _BK_VERBOSE_LOG
        const char *key = duk_get_string(m_context, -2);
        int type = duk_get_type(m_context, -1);
        BKLOG("%s (%d) exposed.", key, type);
#endif
        duk_put_prop(m_context, idxNewGlobal);
        // ... newGlobal globals enum
    }
}

int DukContext::RegisterFunction(const char *name, BkCallback &functionImpl)
{
    if (!m_functionManager)
        return BkError::Forbidden;
    return m_functionManager->Register(m_context, name, functionImpl);
}

#ifndef BLINKIT_CRAWLER_ONLY
void DukContext::RegisterPrototypes(void)
{
    m_prototypeManager->BeginRegisterTransaction(m_context);
    DukConsole::RegisterPrototype(m_context, *m_prototypeManager);
    DukWindow::RegisterPrototype(m_context, *m_prototypeManager);
    m_prototypeManager->EndRegisterTransaction(m_context);
}
#endif // BLINKIT_CRAWLER_ONLY

void DukContext::RegisterPrototypesForCrawler(void)
{
    m_prototypeManager->BeginRegisterTransaction(m_context);
    DukConsole::RegisterPrototype(m_context, *m_prototypeManager);
    DukDocument::RegisterPrototypeForCrawler(m_context, *m_prototypeManager);
    DukElement::RegisterPrototypeForCrawler(m_context, *m_prototypeManager);
    DukWindow::RegisterPrototypeForCrawler(m_context, *m_prototypeManager);
    m_prototypeManager->EndRegisterTransaction(m_context);
}

void DukContext::Reset(void)
{
    m_objectPool.clear();
    Initialize();

#ifndef BLINKIT_CRAWLER_ONLY
    if (!m_frame.client()->IsCrawler())
        return;
#endif

    CrawlerImpl *crawler = GetCrawler();
    crawler->Client().ContextReady(crawler);
}

} // namespace BlinKit
