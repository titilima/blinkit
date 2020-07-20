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
#include "blinkit/js/js_caller_context_impl.h"
#include "blinkit/js/js_value_impl.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_anchor_element.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_attr.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_console.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_document.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_event.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_location.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_navigator.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_script_element.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_window.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_xhr.h"
#include "third_party/blink/renderer/bindings/core/duk/script_controller.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"

using namespace blink;
using namespace BlinKit;

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
    if (nullptr != m_userObject)
        delete m_userObject;
    duk_destroy_heap(m_ctx);
}

void ContextImpl::CreateUserObject(const CrawlerImpl &crawler)
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

    if (nullptr != m_userObject)
    {
        delete m_userObject;
        m_userObject = nullptr;
    }

    std::string errorLog;
    const auto callback = [this, &errorLog](duk_context *ctx)
    {
        if (!duk_is_error(ctx, -1))
        {
            if (duk_is_object(ctx, -1))
            {
                ASSERT(nullptr == m_userObject);
                m_userObject = new JSObjectImpl(ctx, -1);
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
#else
    if (frame.Client()->IsCrawler())
    {
        RegisterPrototypesForCrawler(m_ctx);

        CrawlerImpl *crawler = ToCrawlerImpl(m_frame.Client());
        crawler->ApplyLogger(m_logger);
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

BkJSCallerContext ContextImpl::PrepareFunctionCall(int callContext, const char *functionName)
{
    JSCallerContextImpl *ret = nullptr;
    const int top = duk_get_top(m_ctx);

    do {
        void *thisPtr = nullptr;
        switch (callContext)
        {
            case BK_CTX_GLOBAL:
                duk_push_global_object(m_ctx);
                break;
            case BK_CTX_USER_OBJECT:
                m_userObject->PushTo(m_ctx);
                thisPtr = duk_get_heapptr(m_ctx, -1);
                break;
            default:
                NOTREACHED();
        }

        if (duk_get_top(m_ctx) == top || !duk_is_object(m_ctx, -1))
            break;
        if (!duk_get_prop_string(m_ctx, -1, functionName))
            break;
        if (!duk_is_callable(m_ctx, -1))
            break;

        ret = new JSCallerContextImpl(m_ctx, -1);
        if (nullptr != thisPtr)
            ret->SetThis(thisPtr);
    } while (false);

    duk_set_top(m_ctx, top);
    return ret;
}

void ContextImpl::RegisterPrototypesForCrawler(duk_context *ctx)
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
    DukLocation::RegisterPrototype(helper);
    DukNavigator::RegisterPrototype(helper);
    DukNode::RegisterPrototype(helper, ProtoNames::DocumentType);
    DukNode::RegisterPrototype(helper, ProtoNames::Text);
    DukScriptElement::RegisterPrototypeForCrawler(helper);
    DukWindow::RegisterPrototypeForCrawler(helper);
    DukXHR::RegisterPrototype(helper);
}

void ContextImpl::Reset(void)
{
    const duk_idx_t idx = DukScriptObject::Create<DukWindow>(m_ctx, *(m_frame.DomWindow()));
    BKLOG("New DukWindow object: %p", duk_get_heapptr(m_ctx, idx));
    ExposeGlobals(m_ctx, idx);
    duk_set_global_object(m_ctx);

#ifdef BLINKIT_CRAWLER_ONLY
    ASSERT(m_frame.Client()->IsCrawler());
#else
    if (!m_frame.Client()->IsCrawler())
        return;
#endif

    CrawlerImpl *crawler = ToCrawlerImpl(m_frame.Client());

    /**
     * User object SHOULD BE created after new global object set.
     * Otherwise, all the contexts of user object member functions won't be in correct status.
     */
    CreateUserObject(*crawler);

    crawler->ProcessDocumentReset();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT int BKAPI BkEvaluate(BkJSContext context, const char *code, BkJSValue *retVal)
{
    JSValueImpl *ret = nullptr;
    const auto callback = [&ret](duk_context *ctx)
    {
        ret = JSValueImpl::Create(ctx, -1);
    };
    context->Eval(code, callback);

    int r = ret->GetType() == BK_VT_ERROR
        ? static_cast<JSErrorImpl *>(ret)->GetCode()
        : BK_ERR_SUCCESS;
    if (nullptr != retVal)
        *retVal = ret;
    else
        ret->Release();
    return r;
}

BKEXPORT BkJSObject BKAPI BkGetUserObject(BkJSContext context)
{
    return context->UserObject();
}

BKEXPORT BkJSCallerContext BKAPI BkPrepareFunctionCall(BkJSContext context, int callContext, const char *functionName)
{
    return context->PrepareFunctionCall(callContext, functionName);
}

} // extern "C"
