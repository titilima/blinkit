// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_event_listener.cpp
// Description: DukEventListener Class
//      Author: Ziming Li
//     Created: 2020-03-19
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_event_listener.h"

#include "base/memory/ptr_util.h"
#include "base/strings/stringprintf.h"
#include "blinkit/js/browser_context.h"
#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_event.h"
#include "third_party/blink/renderer/bindings/core/duk/script_controller.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"

using namespace blink;

namespace BlinKit {

static const char ListenerPrefix[] = DUK_HIDDEN_SYMBOL("listener_");
static const char Listener[] = "listener";
static const char NativeLister[] = "nativeListener";

DukEventListener::DukEventListener(duk_context *ctx, void *heapPtr, const std::string &key)
    : m_ctx(ctx), m_heapPtr(heapPtr), m_key(key)
{
    duk_push_global_object(m_ctx);

    duk_push_bare_object(m_ctx);
    duk_push_heapptr(m_ctx, m_heapPtr);
    duk_put_prop_string(m_ctx, -2, Listener);
    duk_push_pointer(m_ctx, this);
    duk_put_prop_string(m_ctx, -2, NativeLister);

    duk_put_prop_lstring(m_ctx, -2, m_key.data(), m_key.length());
    duk_pop(m_ctx);
}

DukEventListener::~DukEventListener(void)
{
    duk_push_global_object(m_ctx);
    duk_del_prop_lstring(m_ctx, -1, m_key.data(), m_key.length());
    duk_pop(m_ctx);
}

bool DukEventListener::BelongsToTheCurrentWorld(ExecutionContext *executionContext) const
{
    BrowserContext *ctxImpl = BrowserContext::From(executionContext);
    return ctxImpl->GetRawContext() == m_ctx;
}

std::shared_ptr<EventListener> DukEventListener::CreateAttributeEventListener(Node *node, const QualifiedName &name, const AtomicString &value)
{
    ASSERT(nullptr != node);
    if (value.IsNull())
        return nullptr;

    if (LocalFrame *frame = node->GetDocument().GetFrame())
    {
        ScriptController &scriptController = frame->GetScriptController();
        if (!node->GetDocument().CanExecuteScripts(kAboutToExecuteScript))
            return nullptr;

        ASSERT(false); // BKTODO:
    }
    ASSERT(false); // BKTODO:
    return nullptr;
}

std::string DukEventListener::GenerateKey(EventTarget *target, const AtomicString &type, void *heapPtr)
{
    std::string ret(ListenerPrefix);
    ret += base::StringPrintf("%p_%s_%p", target, type.StdUtf8().c_str(), heapPtr);
    return ret;
}

std::shared_ptr<EventListener> DukEventListener::Get(duk_context *ctx, duk_idx_t idx, EventTarget *target, const AtomicString &type, bool createIfNotExists)
{
    std::shared_ptr<EventListener> ret;

    void *heapPtr = duk_get_heapptr(ctx, idx);
    std::string key = GenerateKey(target, type, heapPtr);

    do {
        Duk::StackGuard sg(ctx);
        duk_push_global_object(ctx);
        if (!duk_get_prop_lstring(ctx, -1, key.data(), key.length()))
        {
            if (createIfNotExists)
                ret = base::WrapShared(new DukEventListener(ctx, heapPtr, key));
            break;
        }

        if (!duk_get_prop_string(ctx, -1, NativeLister))
        {
            NOTREACHED();
            break;
        }

        ret = reinterpret_cast<DukEventListener *>(duk_to_pointer(ctx, -1))->shared_from_this();
    } while (false);
    return ret;
}

void DukEventListener::handleEvent(ExecutionContext *executionContext, Event *event)
{
    BrowserContext *ctxImpl = BrowserContext::From(executionContext);

    ASSERT(ctxImpl->GetRawContext() == m_ctx);

    Duk::StackGuard sg(m_ctx);

    duk_push_heapptr(m_ctx, m_heapPtr);
    DukEvent::Push(m_ctx, event);
    int r = duk_pcall(m_ctx, 1);
    if (DUK_EXEC_SUCCESS == r)
    {
        if (event->IsBeforeUnloadEvent() && !duk_is_null(m_ctx, -1) && !duk_is_undefined(m_ctx, -1))
        {
            ASSERT(false); // BKTODO:
#if 0
            size_t len = 0;
            const char *s = duk_to_lstring(ctx, -1, &len);
            toBeforeUnloadEvent(event)->setReturnValue(String::fromUTF8(s, len));
#endif
        }

        if (duk_is_boolean(m_ctx, -1))
        {
            if (!duk_to_boolean(m_ctx, -1))
                event->preventDefault();
        }
    }
    else
    {
#ifndef NDEBUG
        duk_get_prop_string(m_ctx, -1, "stack");
#endif
        std::string str = Duk::To<std::string>(m_ctx, -1);
        ctxImpl->ConsoleOutput(BK_CONSOLE_ERROR, str.c_str());
    }
}

} // namespace BlinKit
