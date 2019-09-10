// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_xhr.cpp
// Description: Bindings for XHR
//      Author: Ziming Li
//     Created: 2019-06-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_xhr.h"

#include "public/platform/Platform.h"
#include "public/platform/WebTaskRunner.h"
#include "public/platform/WebTraceLocation.h"
#include "blinkit/http/request_impl.h"
#include "context/duk_context.h"
#include "context/prototype_manager.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {

class ReadyState {
public:
    enum {
        Uninitialized = 0,
        Open          = 1,
        Sent          = 2,
        Receiving     = 3,
        Loaded        = 4
    };
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DukXHR::Session {
public:
    Session(duk_context *ctx, const std::string &method, const std::string &URL, bool async,
        const std::string username, const std::string &password)
        : m_method(method), m_URL(URL), m_username(username), m_password(password)
    {
        if (async)
            m_ctx = ctx;

        Duk::StackKeeper sk(ctx);
        duk_peval_string(ctx, "location.href");
        if (duk_is_string(ctx, -1))
            m_headers["Referer"] = Duk::ToString(ctx);
    }
    ~Session(void)
    {
        if (nullptr != m_response)
            m_response->Release();
    }

    void Attach(const BkResponse &response)
    {
        assert(nullptr == m_response);
        m_response = response.Retain();
    }

    BkRequest* CreateRequest(DukXHR &xhr)
    {
        BkRequest *ret = RequestImpl::CreateInstance(m_URL.c_str(), xhr);
        ret->SetMethod(m_method.c_str());
        for (const auto &it : m_headers)
            ret->SetHeader(it.first.c_str(), it.second.c_str());
        assert(m_username.empty() && m_password.empty()); // BKTODO:
        return ret;
    }

    bool Async(void) const { return nullptr != m_ctx; }
    duk_context* Context(void) const { return m_ctx; }
    BkResponse* Response(void) const { return m_response; }
private:
    duk_context *m_ctx = nullptr;
    const std::string m_method, m_URL, m_username, m_password;
    std::unordered_map<std::string, std::string> m_headers;
    BkRetainedResponse *m_response = nullptr;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NDEBUG

#include <unordered_set>

class XHRPool {
public:
    ~XHRPool(void) { assert(m_pool.empty()); }

    void Add(DukXHR *xhr) { m_pool.insert(xhr); }
    void Remove(DukXHR *xhr) { m_pool.erase(xhr); }
private:
    std::unordered_set<DukXHR *> m_pool;
};

static XHRPool g_xhrPool;

#endif

const char DukXHR::ProtoName[] = "XMLHttpRequest";

DukXHR::DukXHR(duk_context *ctx) : m_heapPtr(duk_get_heapptr(ctx, -1))
{
#ifndef NDEBUG
    g_xhrPool.Add(this);
#endif
}

DukXHR::~DukXHR(void)
{
#ifndef NDEBUG
    g_xhrPool.Remove(this);
#endif
}

duk_ret_t DukXHR::Constructor(duk_context *ctx)
{
    if (duk_is_constructor_call(ctx))
    {
        duk_push_this(ctx);
        DukContext::PrototypeManagerFrom(ctx)->Bind(ctx, ProtoName);
        Duk::BindNativeThis(ctx, new DukXHR(ctx));
        return 0;
    }
    else
    {
        assert(false); // BKTODO:
        return 1;
    }
}

duk_ret_t DukXHR::Finalizer(duk_context *ctx)
{
    DukXHR *xhr = Get(ctx, 0);
    if (nullptr != xhr)
        delete xhr;
    return 0;
}

DukXHR* DukXHR::Get(duk_context *ctx, duk_idx_t idx)
{
    return static_cast<DukXHR *>(Duk::GetNativeThis(ctx, idx));
}

duk_ret_t DukXHR::GetResponseText(duk_context *ctx)
{
    std::string ret;
    if (ReadyState::Loaded == m_readyState)
        m_session->Response()->GetBody(BkMakeBuffer(ret).Wrap());
    duk_push_lstring(ctx, ret.data(), ret.length());
    return 1;
}

duk_ret_t DukXHR::GetStatus(duk_context *ctx)
{
    if (ReadyState::Loaded == m_readyState)
    {
        duk_push_int(ctx, m_session->Response()->StatusCode());
    }
    else
    {
        assert(m_session);
        duk_push_undefined(ctx);
    }
    return 1;
}

duk_ret_t DukXHR::Open(
    duk_context *ctx,
    const std::string &method, const std::string &URL, bool async,
    const std::string username, const std::string &password)
{
    m_session = std::make_unique<Session>(ctx, method, URL, async, username, password);
    SetReadyState(ctx, ReadyState::Open);
    return 0;
}

void BKAPI DukXHR::RequestComplete(const BkResponse &response)
{
    m_session->Attach(response);

    duk_context *ctx = m_session->Context();
    if (nullptr != ctx)
    {
        const auto worker = [this, ctx] {
            SetReadyState(ctx, ReadyState::Loaded);
        };
        Platform::current()->mainThread()->taskRunner()->postTask(BLINK_FROM_HERE, worker);
    }
    else
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_signal = true;
        }
        m_cv.notify_one();
    }
}

void BKAPI DukXHR::RequestFailed(int errorCode)
{
    assert(false); // BKTODO:
}

duk_ret_t DukXHR::Send(duk_context *ctx)
{
    if (!m_session)
        return 0;

    BkRequest *req = m_session->CreateRequest(*this);
    if (duk_is_string(ctx, 0))
    {
        const std::string body = Duk::ToString(ctx, 0);
        req->SetBody(body.data(), body.length());
    }

    m_signal = false;
    req->Perform();
    if (m_session->Async())
        return 0;

    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (!m_signal)
            m_cv.wait(lock, [this] { return m_signal; });
    }
    SetReadyState(ctx, ReadyState::Loaded);
    return 0;
}

void DukXHR::SetReadyState(duk_context *ctx, int newState)
{
    if (m_readyState == newState)
        return;

    Duk::StackKeeper sk(ctx);
    m_readyState = newState;
    duk_push_heapptr(ctx, m_heapPtr);
    if (!duk_get_prop_string(ctx, -1, "onreadystatechange"))
        return;

    if (duk_is_callable(ctx, -1))
        duk_pcall(ctx, 0);
    else
        assert(duk_is_callable(ctx, -1));
}

namespace Impl {

static duk_ret_t Abort(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t GetAllResponseHeaders(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t GetResponseHeader(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t Open(duk_context *ctx)
{
    const std::string method = Duk::ToString(ctx, 0);
    const std::string URL = Duk::ToString(ctx, 1);
    bool async = duk_opt_boolean(ctx, 2, true);

    std::string username;
    if (duk_is_string(ctx, 3))
        username = Duk::ToString(ctx, 3);

    std::string password;
    if (duk_is_string(ctx, 4))
        password = Duk::ToString(ctx, 4);

    duk_push_this(ctx);
    return DukXHR::Get(ctx)->Open(ctx, method, URL, async, username, password);
}

static duk_ret_t ReadyStateGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    duk_push_int(ctx, DukXHR::Get(ctx)->GetReadyState());
    return 1;
}

static duk_ret_t ResponseTextGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    return DukXHR::Get(ctx)->GetResponseText(ctx);
}

static duk_ret_t ResponseXMLGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

static duk_ret_t Send(duk_context *ctx)
{
    duk_push_this(ctx);
    return DukXHR::Get(ctx)->Send(ctx);
}

static duk_ret_t SetRequestHeader(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 0;
}

static duk_ret_t StatusGetter(duk_context *ctx)
{
    duk_push_this(ctx);
    return DukXHR::Get(ctx)->GetStatus(ctx);
}

static duk_ret_t StatusTextGetter(duk_context *ctx)
{
    assert(false); // BKTODO:
    return 1;
}

} // namespace Impl

void DukXHR::RegisterPrototype(duk_context *ctx, PrototypeManager &protos)
{
    static const PrototypeEntry::Property Properties[] = {
        { "readyState",   Impl::ReadyStateGetter,   nullptr },
        { "responseText", Impl::ResponseTextGetter, nullptr },
        { "responseXML",  Impl::ResponseXMLGetter,  nullptr },
        { "status",       Impl::StatusGetter,       nullptr },
        { "statusText",   Impl::StatusTextGetter,   nullptr },
    };
    static const PrototypeEntry::Method Methods[] = {
        { "abort",                 Impl::Abort,                 0 },
        { "getAllResponseHeaders", Impl::GetAllResponseHeaders, 0 },
        { "getResponseHeader",     Impl::GetResponseHeader,     1 },
        { "open",                  Impl::Open,                  5 },
        { "send",                  Impl::Send,                  1 },
        { "setRequestHeader",      Impl::SetRequestHeader,      2 },
    };
    const auto worker = [](PrototypeEntry &entry)
    {
        entry.SetFinalizer(Finalizer);
        entry.Add(Properties, WTF_ARRAY_LENGTH(Properties));
        entry.Add(Methods, WTF_ARRAY_LENGTH(Methods));
    };
    protos.Register(ctx, ProtoName, worker);
}

} // namespace BlinKit
