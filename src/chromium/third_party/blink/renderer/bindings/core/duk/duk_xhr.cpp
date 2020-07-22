// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_xhr.cpp
// Description: DukXHR Class
//      Author: Ziming Li
//     Created: 2020-07-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_xhr.h"

#include <condition_variable>
#include <mutex>
#include "base/single_thread_task_runner.h"
#include "bkcommon/buffer_impl.hpp"
#include "bkcommon/response_impl.h"
#include "bkcommon/bk_strings.h"
#include "blinkit/js/heap_retained.h"
#include "third_party/blink/public/platform/platform.h"
#include "third_party/blink/renderer/bindings/core/duk/duk.h"
#include "third_party/blink/renderer/core/dom/document.h"
#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/local_frame.h"
#include "third_party/blink/renderer/platform/wtf/wtf.h"

using namespace blink;

namespace BlinKit {

const char DukXHR::ProtoName[] = "XMLHttpRequest";
static const char NativeXHR[] = DUK_HIDDEN_SYMBOL("nativeXHR");

template <>
static inline DukXHR* DukScriptObject::To<DukXHR>(duk_context *ctx, duk_idx_t idx)
{
    DukXHR *ret = nullptr;
    if (duk_get_prop_string(ctx, idx, NativeXHR))
        ret = reinterpret_cast<DukXHR *>(duk_to_pointer(ctx, -1));
    duk_pop(ctx);
    return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DukXHR::Session
{
public:
    virtual ~Session(void)
    {
        if (nullptr != m_response)
            m_response->Release();
    }

    int StatusCode(void) const
    {
        return nullptr != m_response ? m_response->StatusCode() : 0;
    }
    std::string ResponseText(void) const
    {
        std::string ret;
        if (nullptr != m_response)
            m_response->GetData(BK_RESPONSE_BODY, BufferImpl::Wrap(ret));
        return ret;
    }

    virtual void OnRequestSent(duk_context *ctx) {}
    virtual void OnRequestComplete(BkResponse response)
    {
        ASSERT(nullptr == m_response);
        m_response = response->Retain();
    }
    void OnRequestFailed(int errorCode)
    {
        ASSERT(false); // BKTODO:
    }
protected:
    Session(duk_context *ctx, duk_idx_t idx) : m_heapPtr(duk_get_heapptr(ctx, idx))
    {
        ASSERT(duk_normalize_index(ctx, idx) == idx);
    }

    void ProcessResponse(duk_context *ctx)
    {
        ASSERT(IsMainThread());

        duk_push_heapptr(ctx, m_heapPtr);
        duk_idx_t idx = duk_normalize_index(ctx, -1);

        DukXHR *xhr = DukScriptObject::To<DukXHR>(ctx, idx);
        xhr->SetReadyState(ctx, idx, ReadyState::Receiving);
        xhr->SetReadyState(ctx, idx, ReadyState::Loaded);
    }

    BkResponse m_response = nullptr;
    void *m_heapPtr;
};

class DukXHR::AsyncSession final : public DukXHR::Session, public HeapRetained
{
public:
    AsyncSession(duk_context *ctx, duk_idx_t idx)
        : Session(ctx, idx), HeapRetained(DUK_HIDDEN_SYMBOL("xhr"))
        , m_ctx(ctx)
        , m_taskRunner(Platform::Current()->CurrentThread()->GetTaskRunner())
    {
        ASSERT(IsMainThread());
        HeapRetained::Retain(m_ctx, idx);
    }
private:
    void ProcessResponse(void)
    {
        Session::ProcessResponse(m_ctx);
        HeapRetained::Release(m_ctx);
    }

    void OnRequestComplete(BkResponse response) override
    {
        Session::OnRequestComplete(response);

        const auto task = std::bind(&AsyncSession::ProcessResponse, this);
        m_taskRunner->PostTask(FROM_HERE, task);
    }

    duk_context *m_ctx;
    std::shared_ptr<base::SingleThreadTaskRunner> m_taskRunner;
};

class DukXHR::SyncSession final : public DukXHR::Session
{
public:
    SyncSession(duk_context *ctx, duk_idx_t idx) : Session(ctx, idx) {}
private:
    void OnRequestSent(duk_context *ctx) override
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            while (!m_signal)
                m_cond.wait(lock);
        }
        ProcessResponse(ctx);
    }
    void OnRequestComplete(BkResponse response) override
    {
        Session::OnRequestComplete(response);
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_signal = true;
        }
        m_cond.notify_one();
    }

    bool m_signal = false;
    std::mutex m_mutex;
    std::condition_variable m_cond;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DukXHR::DukXHR(duk_context *ctx, duk_idx_t idx) : m_heapPtr(duk_get_heapptr(ctx, idx))
{
    idx = duk_normalize_index(ctx, idx);
    duk_push_pointer(ctx, this);
    duk_put_prop_string(ctx, idx, NativeXHR);
}

DukXHR::~DukXHR(void) = default;

duk_ret_t DukXHR::Construct(duk_context *ctx)
{
    if (duk_is_constructor_call(ctx))
    {
        duk_push_this(ctx);
        new DukXHR(ctx, -1);
        PrototypeHelper::AttachToScriptObject(ctx, -1, ProtoName);
        return 0;
    }
    else
    {
        ASSERT(false); // BKTODO:
        return 1;
    }
}

namespace Impl {

static duk_ret_t Abort(duk_context *ctx)
{
    ASSERT(false); // BKTODO:
    return 0;
}

static duk_ret_t GetAllResponseHeaders(duk_context *ctx)
{
    ASSERT(false); // BKTODO:
    return 0;
}

static duk_ret_t GetResponseHeader(duk_context *ctx)
{
    ASSERT(false); // BKTODO:
    return 0;
}

static duk_ret_t Open(duk_context *ctx)
{
    const std::string method = Duk::To<std::string>(ctx, 0);
    const String URL = Duk::To<String>(ctx, 1);
    bool async = duk_opt_boolean(ctx, 2, true);

    std::string username;
    if (duk_is_string(ctx, 3))
        username = Duk::To<std::string>(ctx, 3);

    std::string password;
    if (duk_is_string(ctx, 4))
        password = Duk::To<std::string>(ctx, 4);

    duk_push_this(ctx);
    return DukScriptObject::To<DukXHR>(ctx, -1)->Open(ctx, -1, method, URL, async, username, password);
}

static duk_ret_t ReadyStateGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    return DukScriptObject::To<DukXHR>(ctx, -1)->GetReadyState(ctx);
}

static duk_ret_t ResponseTextGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    return DukScriptObject::To<DukXHR>(ctx, -1)->GetResponseText(ctx);
}

static duk_ret_t ResponseXMLGetter(duk_context *ctx)
{
    ASSERT(false); // BKTODO:
    return 0;
}

static duk_ret_t Send(duk_context *ctx)
{
    const std::string body = Duk::To<std::string>(ctx, 0);

    duk_push_this(ctx);
    return DukScriptObject::To<DukXHR>(ctx, -1)->Send(ctx, -1, body);
}

static duk_ret_t SetRequestHeader(duk_context *ctx)
{
    const std::string name = Duk::To<std::string>(ctx, 0);
    const std::string value = Duk::To<std::string>(ctx, 1);

    duk_push_this(ctx);
    return DukScriptObject::To<DukXHR>(ctx, -1)->SetRequestHeader(name, value);
}

static duk_ret_t StatusGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    return DukScriptObject::To<DukXHR>(ctx, -1)->GetStatus(ctx);
}

static duk_ret_t StatusTextGetter(duk_context *ctx)
{
    ASSERT(false); // BKTODO:
    return 0;
}

} // namespace Impl

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<DukXHR::Session> DukXHR::CreateSession(duk_context *ctx, duk_idx_t idx, bool async)
{
    if (async)
        return std::make_unique<DukXHR::AsyncSession>(ctx, idx);
    else
        return std::make_unique<DukXHR::SyncSession>(ctx, idx);
}

void DukXHR::FillPrototypeEntry(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "abort",                 Impl::Abort,                 0 },
        { "getAllResponseHeaders", Impl::GetAllResponseHeaders, 0 },
        { "getResponseHeader",     Impl::GetResponseHeader,     1 },
        { "open",                  Impl::Open,                  5 },
        { "send",                  Impl::Send,                  1 },
        { "setRequestHeader",      Impl::SetRequestHeader,      2 },
    };
    static const PrototypeEntry::Property Properties[] = {
        { "readyState",   Impl::ReadyStateGetter,   nullptr },
        { "responseText", Impl::ResponseTextGetter, nullptr },
        { "responseXML",  Impl::ResponseXMLGetter,  nullptr },
        { "status",       Impl::StatusGetter,       nullptr },
        { "statusText",   Impl::StatusTextGetter,   nullptr },
    };

    DukScriptObject::FillPrototypeEntry(entry);
    entry.SetFinalizer(Finalizer);
    entry.Add(Methods, std::size(Methods));
    entry.Add(Properties, std::size(Properties));
}

duk_ret_t DukXHR::Finalizer(duk_context *ctx)
{
    DukXHR *xhr = DukScriptObject::To<DukXHR>(ctx, 0);
    if (nullptr != xhr)
        delete xhr;
    return 0;
}

duk_ret_t DukXHR::GetReadyState(duk_context *ctx) const
{
    duk_push_int(ctx, static_cast<duk_int_t>(m_readyState));
    return 1;
}

duk_ret_t DukXHR::GetResponseText(duk_context *ctx) const
{
    std::string ret;
    if (m_currentSession)
        ret = m_currentSession->ResponseText();
    duk_push_lstring(ctx, ret.data(), ret.length());
    return 1;
}

duk_ret_t DukXHR::GetStatus(duk_context *ctx) const
{
    duk_int_t ret = 0;
    if (m_currentSession)
        ret = m_currentSession->StatusCode();
    duk_push_int(ctx, ret);
    return 1;
}

duk_ret_t DukXHR::Open(
    duk_context *ctx, duk_idx_t idx,
    const std::string &method, const String &URL, bool async,
    const std::string username, const std::string &password)
{
    idx = duk_normalize_index(ctx, idx);

    duk_push_global_object(ctx);
    LocalDOMWindow *window = DukScriptObject::To<LocalDOMWindow>(ctx, -1);
    Document *document = window->document();

    Reset();
    m_method = method;
    m_currentSession = CreateSession(ctx, idx, async);
    m_URL = document->CompleteURL(URL).spec();
    m_requestHeaders[Strings::HttpHeader::Referer] = document->Url().spec();
    m_requestHeaders[Strings::HttpHeader::UserAgent] = window->GetFrame()->Loader().UserAgent().StdUtf8();

    std::string cookie = document->cookie(ASSERT_NO_EXCEPTION).StdUtf8();
    if (!cookie.empty())
        m_requestHeaders[Strings::HttpHeader::Cookie] = cookie;

    ASSERT(username.empty() && password.empty()); // BKTODO:

    SetReadyState(ctx, idx, ReadyState::Open);
    return 0;
}

void DukXHR::RegisterPrototype(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntry);
}

void BKAPI DukXHR::RequestCompleteImpl(BkResponse response, void *userData)
{
    reinterpret_cast<Session *>(userData)->OnRequestComplete(response);
}

void BKAPI DukXHR::RequestFailedImpl(int errorCode, void *userData)
{
    reinterpret_cast<Session *>(userData)->OnRequestFailed(errorCode);
}

void DukXHR::Reset(void)
{
    m_method.clear();
    m_URL.clear();
    m_requestHeaders.clear();
    m_currentSession.reset();
}

duk_ret_t DukXHR::Send(duk_context *ctx, duk_idx_t idx, const std::string &body)
{
    if (ReadyState::Open != m_readyState)
        return duk_generic_error(ctx, "The object is in an invalid state.");

    idx = duk_normalize_index(ctx, idx);

    BkRequestClient client = { 0 };
    client.SizeOfStruct = sizeof(BkRequestClient);
    client.UserData = m_currentSession.get();
    client.RequestComplete = RequestCompleteImpl;
    client.RequestFailed = RequestFailedImpl;
    BkRequest req = BkCreateRequest(m_URL.c_str(), &client);
    BkSetRequestMethod(req, m_method.c_str());
    for (const auto &it : m_requestHeaders)
        BkSetRequestHeader(req, it.first.c_str(), it.second.c_str());
    if (!body.empty())
        BkSetRequestBody(req, body.data(), body.length());
    if (BK_ERR_SUCCESS != BkPerformRequest(req, nullptr))
    {
        ASSERT(false); // BKTODO:
    }

    m_currentSession->OnRequestSent(ctx);
    return 0;
}

void DukXHR::SetReadyState(duk_context *ctx, duk_idx_t idx, ReadyState newState)
{
    ASSERT(duk_normalize_index(ctx, idx) == idx);
    if (m_readyState == newState)
        return;

    m_readyState = newState;
    if (duk_get_prop_string(ctx, idx, "onreadystatechange"))
    {
        if (duk_is_callable(ctx, -1))
            duk_pcall(ctx, 0);
    }
    duk_pop(ctx);
}

duk_ret_t DukXHR::SetRequestHeader(const std::string &name, const std::string &value)
{
    m_requestHeaders[name] = value;
    return 0;
}

} // namespace BlinKit
