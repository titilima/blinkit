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

#include "bindings/duk_console.h"
#include "bindings/duk_window.h"
#include "context/caller_context_impl.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

namespace StashFields {
static const char Context[] = "context";
static const char CrawlerObject[] = "crawlerObject";
static const char Globals[] = "globals";
}

namespace Crawler {
static duk_idx_t Eval(duk_context *ctx)
{
    assert(false); // BKTODO: Check incantation.
    duk_peval(ctx);
    return 1;
}
}

DukContext::DukContext(LocalFrame &frame)
    : m_context(duk_create_heap_default())
{
    Attach(frame);
    Initialize();
}

DukContext::~DukContext(void)
{
    duk_destroy_heap(m_context);
}

void DukContext::AdjustGlobalsForCrawler(duk_context *ctx)
{
    static const char eval[] = "eval";

    duk_del_prop_string(ctx, -1, eval);
    duk_push_c_function(ctx, Crawler::Eval, 1);
    duk_put_prop_string(ctx, -2, eval);
}

void DukContext::Attach(LocalFrame &frame)
{
#ifdef BLINKIT_CRAWLER_ONLY
    assert(frame.client()->IsCrawler());
    bool isCrawler = true;
#else
    bool isCrawler = frame.client()->IsCrawler();
#endif

    Duk::StackKeeper sk(m_context);
    duk_push_heap_stash(m_context);

    duk_push_pointer(m_context, this);
    duk_put_prop_string(m_context, -2, StashFields::Context);

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
#endif
}

int DukContext::CallCrawler(const char *method, BkCallerContext::Callback callback, void *userData)
{
    if (nullptr == m_crawlerObjectPtr)
        return BkError::NotFound;

    duk_push_heapptr(m_context, m_crawlerObjectPtr);

    CallerContextImpl context(m_context);
    context.SetAsThisCall();
    return context.Call(method, callback, userData);
}

int DukContext::CallFunction(const char *name, BkCallerContext::Callback callback, void *userData)
{
    duk_push_global_object(m_context);

    CallerContextImpl context(m_context);
    return context.Call(name, callback, userData);
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
    duk_put_prop_string(m_context, -2, StashFields::CrawlerObject);
    return BkError::Success;
}

void DukContext::CreateObject(const char *protoName)
{
    if (!m_prototypeManager->CreateObject(m_context, protoName))
        duk_push_undefined(m_context);
}

DukContext* DukContext::From(duk_context *ctx)
{
    Duk::StackKeeper sk(ctx);
    duk_push_heap_stash(ctx);
    if (!duk_get_prop_string(ctx, -1, StashFields::Context))
        return nullptr;
    return reinterpret_cast<DukContext *>(duk_get_pointer(ctx, -1));
}

void DukContext::Initialize(void)
{
    CreateObject(DukWindow::ProtoName);
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
    DukWindow::RegisterPrototypeForCrawler(m_context, *m_prototypeManager);
    m_prototypeManager->EndRegisterTransaction(m_context);
}

void DukContext::Reset(void)
{
    assert(false); // BKTODO:
}

} // namespace BlinKit
