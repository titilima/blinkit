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
#ifndef BLINKIT_CRAWLER_ONLY
#   include "bindings/duk_css_style_declaration.h"
#endif
#include "bindings/duk_document.h"
#include "bindings/duk_element.h"
#include "bindings/duk_exception_state.h"
#include "bindings/duk_html_collection.h"
#include "bindings/duk_location.h"
#include "bindings/duk_node_list.h"
#include "bindings/duk_text.h"
#include "bindings/duk_window.h"
#include "bindings/duk_xhr.h"
#include "context/caller_context_impl.h"
#include "context/function_manager.h"
#include "context/prototype_manager.h"
#include "context/value_impl.h"
#include "wrappers/duk.h"

using namespace blink;

#if defined(_WIN32)
#   define DebugBreakImpl() ::DebugBreak()
#else
#   error OS not supported!
#endif

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


namespace Impl {

#ifdef _DEBUG
static duk_ret_t DebugBreak(duk_context *ctx)
{
    if (duk_get_top(ctx) > 0 && !duk_is_undefined(ctx, 0))
        BKLOG("DEBUG BREAK: %s", duk_to_string(ctx, 0));
    DebugBreakImpl();
    return 0;
}
#endif // _DEBUG

} // namespace Impl

namespace Crawler {

static duk_ret_t Notify(duk_context *ctx)
{
    CrawlerImpl *crawler = DukContext::From(ctx)->GetCrawler();
    crawler->Client().DataNotify(crawler, ValueImpl(ctx, 0));
    return 0;
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
    GC();
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
    m_globalsPtr = duk_get_heapptr(m_context, -1);
    duk_put_prop_string(m_context, -2, StashFields::Globals);

    m_prototypeManager = std::make_unique<PrototypeManager>(m_context);
#ifdef BLINKIT_CRAWLER_ONLY
    RegisterPrototypesForCrawler();
#else
    if (isCrawler)
    {
        RegisterPrototypesForCrawler();
    }
    else
    {
        RegisterPrototypesForUI();

        duk_push_object(m_context);
#   ifdef _DEBUG
        duk_push_c_function(m_context, Impl::DebugBreak, 1);
        duk_put_prop_string(m_context, -2, "debugBreak");
#   endif
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

bool DukContext::CheckIncantationAndPushCrawler(duk_context *ctx, const std::string &s)
{
    int top = duk_get_top(ctx);
    do {
        assert(nullptr != m_crawlerObjectPtr);
        duk_push_heapptr(ctx, m_crawlerObjectPtr);
        if (!duk_get_prop_string(ctx, -1, "incantation"))
            break;

        const std::string incantation = Duk::ToString(ctx);
        if (incantation.empty() || incantation != s)
            break;

        duk_pop(ctx); // Pop incantation string, leave the crawler.
        return true;
    } while (false);
    duk_set_top(ctx, top);
    return false;
}

std::tuple<int, std::string> DukContext::CreateCrawlerObject(const char *script, size_t length)
{
    Duk::StackKeeper sk(m_context);
    duk_push_heap_stash(m_context);

    int r = duk_peval_lstring(m_context, script, length);
    if (DUK_EXEC_SUCCESS != r)
    {
        int errorCode = Duk::ToErrorCode(m_context);

        duk_get_prop_string(m_context, -1, "message");
        std::string message = Duk::ToString(m_context);

        return std::make_tuple(errorCode, message);
    }

    if (DUK_TYPE_OBJECT != duk_get_type(m_context, -1))
    {
        std::string message("Crawler MUST BE the type of Object!");
        return std::make_tuple(BkError::TypeError, message);
    }

    m_crawlerObjectPtr = duk_get_heapptr(m_context, -1);

    duk_push_c_function(m_context, Crawler::Notify, 1);
    duk_put_prop_string(m_context, -2, "notify");
#ifdef _DEBUG
    duk_push_c_function(m_context, Impl::DebugBreak, 1);
    duk_put_prop_string(m_context, -2, "debugBreak");
#endif

    m_functionManager = std::make_unique<FunctionManager>(m_context);
    duk_put_prop_string(m_context, -2, StashFields::CrawlerObject);
    return std::make_tuple(BkError::Success, std::string());
}

void DukContext::CreateObject(
    duk_context *ctx, const char *protoName, ScriptWrappable *nativeThis,
    void(*createCallback)(duk_context *, ScriptWrappable *),
    void(*gcCallback)(ScriptWrappable *))
{
    if (m_prototypeManager->CreateObject(ctx, protoName))
    {
        if (nullptr != nativeThis)
        {
            ObjectEntry entry;
            entry.HeapPtr = duk_get_heapptr(ctx, -1);
            entry.GC = gcCallback;
            m_objectPool[nativeThis] = entry;

            Duk::BindNativeThis(ctx, nativeThis);

            createCallback(ctx, nativeThis);
        }
    }
    else
    {
        duk_push_undefined(ctx);
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

void DukContext::GC(void)
{
    for (const auto &it : m_objectPool)
        it.second.GC(it.first);
    m_objectPool.clear();
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
    assert(m_frame.client()->IsCrawler());
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
    DOMWindow *window = m_frame.domWindow();

    duk_push_global_object(m_context);
    DOMWindow *previousWindow = DukEventTarget::GetNativeThis<DOMWindow>(m_context);
    duk_pop(m_context);
    if (window == previousWindow)
        return; // Window didn't changed, so nothing to do.

    CreateObject<DukWindow>(m_context, window);
    BKLOG("Window object (re-)created: %x (%x)", duk_get_heapptr(m_context, -1), window);
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

PrototypeManager* DukContext::PrototypeManagerFrom(duk_context *ctx)
{
    DukContext *context = From(ctx);
    return nullptr != context ? context->m_prototypeManager.get() : nullptr;
}

void DukContext::PushNode(duk_context *ctx, Node *node)
{
    if (nullptr == node)
    {
        duk_push_undefined(ctx);
        return;
    }
    if (node->isDocumentNode())
    {
        PushObject<DukDocument>(ctx, node);
        return;
    }
    if (node->isTextNode())
    {
        PushObject<DukText>(ctx, node);
        return;
    }

    assert(node->isElementNode());
    PushObject<DukElement>(ctx, node);
}

int DukContext::RegisterFunction(const char *name, BkCallback &functionImpl)
{
    if (!m_functionManager)
        return BkError::Forbidden;
    return m_functionManager->Register(m_context, name, functionImpl);
}

#ifndef BLINKIT_CRAWLER_ONLY
void DukContext::RegisterPrototypesForUI(void)
{
    m_prototypeManager->BeginRegisterTransaction(m_context);
    DukConsole::RegisterPrototype(m_context, *m_prototypeManager);
    DukCSSStyleDeclaration::RegisterPrototype(m_context, *m_prototypeManager);
    DukDocument::RegisterPrototypeForUI(m_context, *m_prototypeManager);
    DukElement::RegisterPrototypeForUI(m_context, *m_prototypeManager);
    DukHTMLCollection::RegisterPrototype(m_context, *m_prototypeManager);
    DukLocation::RegisterPrototypeForUI(m_context, *m_prototypeManager);
    DukText::RegisterPrototype(m_context, *m_prototypeManager);
    DukWindow::RegisterPrototypeForUI(m_context, *m_prototypeManager);
    m_prototypeManager->EndRegisterTransaction(m_context);
}
#endif // BLINKIT_CRAWLER_ONLY

void DukContext::RegisterPrototypesForCrawler(void)
{
    m_prototypeManager->BeginRegisterTransaction(m_context);
    DukConsole::RegisterPrototype(m_context, *m_prototypeManager);
    DukDocument::RegisterPrototypeForCrawler(m_context, *m_prototypeManager);
    DukElement::RegisterPrototypeForCrawler(m_context, *m_prototypeManager);
    DukHTMLCollection::RegisterPrototype(m_context, *m_prototypeManager);
    DukLocation::RegisterPrototypeForCrawler(m_context, *m_prototypeManager);
    DukNodeList::RegisterPrototype(m_context, *m_prototypeManager);
    DukText::RegisterPrototype(m_context, *m_prototypeManager);
    DukWindow::RegisterPrototypeForCrawler(m_context, *m_prototypeManager);
    DukXHR::RegisterPrototype(m_context, *m_prototypeManager);
    m_prototypeManager->EndRegisterTransaction(m_context);
}

void DukContext::RemoveObjectFromPool(ScriptWrappable *nativeThis)
{
    auto it = m_objectPool.find(nativeThis);
    if (std::end(m_objectPool) != it)
    {
        it->second.GC(nativeThis);
        m_objectPool.erase(it);
    }
}

void DukContext::Reset(void)
{
    GC();
    Initialize();

#ifndef BLINKIT_CRAWLER_ONLY
    if (!m_frame.client()->IsCrawler())
        return;
#endif

    CrawlerImpl *crawler = GetCrawler();
    crawler->Client().ContextReady(crawler);
}

} // namespace BlinKit
