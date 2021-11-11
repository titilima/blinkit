// -------------------------------------------------
// BlinKit SDK
// -------------------------------------------------
//   File Name: bk.hpp
//      Author: Ziming Li
//     Created: 2020-07-16
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SDK_BK_HPP
#define BLINKIT_SDK_BK_HPP

#pragma once

#include <cassert>
#include <functional>
#include <list>
#include <string>
#include <string_view>
#include <vector>
#include "bk_crawler.h"
#include "bk_http.h"
#include "bk_js.h"
#include "bk_ui.h"

namespace bk {

/**
 * BkBuffer Wrappers
 */

template <typename C>
class buffer_adaptor {
public:
    buffer_adaptor(C &data);

    operator BkBuffer* (void) const;
private:
    static void* BKAPI Alloc(size_t size, void *user_data);

    BkBuffer m_buffer;
};

template <typename CharType>
buffer_adaptor<std::basic_string<CharType>> make_buffer(std::basic_string<CharType> &s);

buffer_adaptor<std::vector<unsigned char>> make_buffer(std::vector<unsigned char> &v);

/**
 * Client Wrappers
 */

template <class T, typename Client>
class client_root_impl {
protected:
    Client get_client(void) const;
    static T* get_client_root(void *p);
private:
    virtual void adjust_raw_client(Client &) const {}
};

template <class T, class ClientRoot>
class client_impl : public ClientRoot {
protected:
    static T* get_this(void *p) { return static_cast<T *>(ClientRoot::get_client_root(p)); }
};

class crawler_client_root : public client_root_impl<crawler_client_root, BkCrawlerClient> {
public:
    BkCrawler create_crawler(void) const;
protected:
    void adjust_raw_client(BkCrawlerClient &client) const override;
    virtual bool get_config(int cfg, std::string &dst) { return false; }
private:
    virtual void document_ready(BkJSContext ctx) = 0;
    virtual void on_crawler_error(int code, const char *url) { assert(false); /* TODO: Process error yourself! */ }

    static void BKAPI document_ready_callback(BkJSContext ctx, void *p);
    static bool_t BKAPI get_config_callback(int cfg, BkBuffer *dst, void *p);
    static void BKAPI error_callback(int code, const char *url, void *p);
};

class request_client_root : public client_root_impl<request_client_root, BkRequestClient> {
public:
    BkRequest create_request(const char *url) const;
protected:
    void adjust_raw_client(BkRequestClient &client) const override;
private:
    virtual void request_complete(BkResponse response) = 0;
    virtual void on_request_failed(int code) { assert(false); /* TODO: Process error yourself! */ }

    static void BKAPI request_complete_callback(BkResponse response, void *p);
    static void BKAPI request_failed_callback(int code, void *p);
};

class web_view_client_root : public client_root_impl<web_view_client_root, BkWebViewClient> {
public:
    BkRequest create_request(const char *url) const;
protected:
    void adjust_raw_client(BkWebViewClient &client) const override;
private:
    virtual void document_ready(void) = 0;

    static void BKAPI document_ready_callback(void *p);
};

template <class T>
class crawler_client_impl : public client_impl<T, crawler_client_root> {};

template <class T>
class request_client_impl : public client_impl<T, request_client_root> {};

#ifdef __ATLWIN_H__
template <class T>
class ATL_NO_VTABLE web_view_client_impl : public client_impl<T, web_view_client_root> {
public:
    bool OnCreate(BkWebView v, LPCREATESTRUCT cs) { return true; }
protected:
    BOOL ProcessWindowMessage(HWND h, UINT m, WPARAM w, LPARAM l, LRESULT &r);
};
#endif

class js_call
{
public:
    static js_call* prepare(std::function<void(BkJSContext)> &&callback);
    int commit_to(BkCrawler crawler) { return BkCrawlerCallJS(crawler, callback_impl, this); }
private:
    js_call(std::function<void(BkJSContext)> &&callback) : m_callback(std::move(callback)) {}

    static void BKAPI callback_impl(BkJSContext ctx, void *userData);

    std::function<void(BkJSContext)> m_callback;
};

class js_value
{
public:
    js_value(BkJSValue val = nullptr) : m_val(val) {}
    ~js_value(void)
    {
        if (nullptr != m_val)
            BkReleaseValue(m_val);
    }

    BkJSValue* operator&() { return &m_val; }
    operator bool() const { return nullptr != m_val; }
    operator BkJSValue() const { return m_val; }

    bool to_boolean(bool def_val) const
    {
        bool_t ret = def_val;
        if (nullptr != m_val)
            BkGetBooleanValue(m_val, &ret);
        return ret;
    }
    std::string to_string(void) const
    {
        std::string ret;
        if (nullptr != m_val)
            BkGetValueAsString(m_val, make_buffer(ret));
        return ret;
    }
private:
    BkJSValue m_val;
};

class js_array
{
public:
    js_array(BkJSArray arr) : m_arr(arr) {}
    operator BkJSArray() const { return m_arr; }

    bool get_boolean(unsigned i, bool def_val) const
    {
        bool ret = def_val;
        if (nullptr != m_arr)
        {
            js_value v(BkArrayGetMember(m_arr, i));
            ret = v.to_boolean(def_val);
        }
        return ret;
    }
    std::string get_string(unsigned i, const char *def_val = "") const
    {
        std::string ret(def_val);
        if (nullptr != m_arr)
        {
            js_value v(BkArrayGetMember(m_arr, i));
            if (v)
                ret = v.to_string();
        }
        return ret;
    }

    std::string to_json(void) const
    {
        std::string ret("null");
        if (nullptr != m_arr)
            BkArrayToJSON(m_arr, make_buffer(ret));
        return ret;
    }
private:
    BkJSArray m_arr;
};

class js_object
{
public:
    js_object(BkJSObject obj) : m_obj(obj) {}
    operator BkJSObject() const { return m_obj; }

    bool get_boolean(const char *name, bool def_val) const
    {
        bool ret = def_val;
        if (nullptr != m_obj)
        {
            js_value v(BkObjectGetMember(m_obj, name));
            ret = v.to_boolean(def_val);
        }
        return ret;
    }
    std::string get_string(const char *name, const char *def_val = "") const
    {
        std::string ret(def_val);
        if (nullptr != m_obj)
        {
            js_value v(BkObjectGetMember(m_obj, name));
            if (v)
                ret = v.to_string();
        }
        return ret;
    }

    std::string to_json(void) const
    {
        std::string ret("null");
        if (nullptr != m_obj)
            BkObjectToJSON(m_obj, make_buffer(ret));
        return ret;
    }
private:
    BkJSObject m_obj;
};

class js_function
{
public:
    static js_function* prepare(BkJSContext js_ctx, int ctx, const char *name)
    {
        js_function *ret = nullptr;
        BkJSCallerContext fn_ctx = BkPrepareFunctionCall(js_ctx, ctx, name);
        if (nullptr != fn_ctx)
            ret = new js_function(fn_ctx);
        return ret;
    }
    static js_function* prepare(BkJSContext js_ctx, const char *code)
    {
        js_function *ret = nullptr;
        BkJSCallerContext fn_ctx = BkPrepareScriptFunction(js_ctx, code);
        if (nullptr != fn_ctx)
            ret = new js_function(fn_ctx);
        return ret;
    }

    int call(BkJSValue *ret = nullptr)
    {
        int r = BkCallFunction(m_ctx, ret);
        delete this;
        return r;
    }

    class arg_list
    {
    public:
        void push(int n)
        {
            BkPushInteger(m_ctx, n);
        }
        void push(const std::string_view &s)
        {
            BkPushString(m_ctx, s.data(), s.length());
        }
        void push_json_string(const std::string_view &j)
        {
            BkPushJSONString(m_ctx, j.data(), j.length());
        }
    private:
        friend class js_function;
        arg_list(BkJSCallerContext ctx) : m_ctx(ctx) {}

        BkJSCallerContext m_ctx;
    };
    typedef std::function<void(arg_list &)> args_callback;

    int call(const args_callback &cb, BkJSValue *ret = nullptr)
    {
        arg_list args(m_ctx);
        cb(args);
        return call(ret);
    }
private:
    js_function(BkJSCallerContext ctx) : m_ctx(ctx) {}

    BkJSCallerContext m_ctx;
};

class function_context
{
public:
    operator BkJSCalleeContext() const { return m_ctx; }
private:
    friend class function_manager;
    function_context(BkJSCalleeContext ctx) : m_ctx(ctx) {}
    BkJSCalleeContext m_ctx;
};

typedef std::function<void(function_context &)> user_function;

class function_manager
{
public:
    function_manager(void) = default;

    void register_function(BkJSContext ctx, int memberContext, const char *name, const user_function &fn)
    {
        m_functions.push_back(fn);
        BkRegisterFunction(ctx, memberContext, name, impl, &(m_functions.back()));
    }
private:
    static void BKAPI impl(BkJSCalleeContext ctx, void *p)
    {
        function_context fnctx(ctx);
        user_function *fn = reinterpret_cast<user_function *>(p);
        (*fn)(fnctx);
    }

    std::list<user_function> m_functions;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Implementations

template <typename C>
buffer_adaptor<C>::buffer_adaptor(C &data)
{
    m_buffer.Allocator = Alloc;
    m_buffer.UserData = &data;
}

template <typename C>
buffer_adaptor<C>::operator BkBuffer* (void) const
{
    BkBuffer *buf = const_cast<BkBuffer *>(&m_buffer);
    return buf;
}

template <typename C>
void* BKAPI buffer_adaptor<C>::Alloc(size_t size, void *user_data)
{
    C *c = reinterpret_cast<C *>(user_data);
    c->resize(size / sizeof(typename C::value_type));
    return const_cast<typename C::value_type *>(c->data());
}

template <typename CharType>
buffer_adaptor<std::basic_string<CharType>> make_buffer(std::basic_string<CharType> &s)
{
    return buffer_adaptor<std::basic_string<CharType>>(s);
}

inline buffer_adaptor<std::vector<unsigned char>> make_buffer(std::vector<unsigned char> &v)
{
    return buffer_adaptor<std::vector<unsigned char>>(v);
}

template <class T, typename Client>
Client client_root_impl<T, Client>::get_client(void) const
{
    Client client = { 0 };
    client.SizeOfStruct = sizeof(Client);
    client.UserData = const_cast<T *>(static_cast<const T *>(this));
    adjust_raw_client(client);
    return client;
}

template <class T, typename Client>
T* client_root_impl<T, Client>::get_client_root(void *p)
{
    return reinterpret_cast<T *>(p);
}

inline BkCrawler crawler_client_root::create_crawler(void) const
{
    BkCrawlerClient client = get_client();
    return BkCreateCrawler(&client);
}

inline void crawler_client_root::adjust_raw_client(BkCrawlerClient &client) const
{
    client.DocumentReady = document_ready_callback;
    client.GetConfig = get_config_callback;
    client.Error = error_callback;
}

inline void BKAPI crawler_client_root::document_ready_callback(BkJSContext ctx, void *p)
{
    get_client_root(p)->document_ready(ctx);
}

inline bool_t BKAPI crawler_client_root::get_config_callback(int cfg, BkBuffer *dst, void *p)
{
    std::string s;
    if (!get_client_root(p)->get_config(cfg, s))
        return false;
    BkSetBufferData(dst, s.data(), s.length());
    return true;
}

inline void BKAPI crawler_client_root::error_callback(int code, const char *url, void *p)
{
    get_client_root(p)->on_crawler_error(code, url);
}

inline BkRequest request_client_root::create_request(const char *url) const
{
    BkRequestClient client = get_client();
    return BkCreateRequest(url, &client);
}

inline void request_client_root::adjust_raw_client(BkRequestClient &client) const
{
    client.RequestComplete = request_complete_callback;
    client.RequestFailed = request_failed_callback;
}

inline void BKAPI request_client_root::request_complete_callback(BkResponse response, void *p)
{
    get_client_root(p)->request_complete(response);
}

inline void BKAPI request_client_root::request_failed_callback(int code, void *p)
{
    get_client_root(p)->on_request_failed(code);
}

inline void web_view_client_root::adjust_raw_client(BkWebViewClient &client) const
{
    client.DocumentReady = document_ready_callback;
}

inline void BKAPI web_view_client_root::document_ready_callback(void *p)
{
    get_client_root(p)->document_ready();
}

#ifdef __ATLWIN_H__
template <class T>
BOOL web_view_client_impl<T>::ProcessWindowMessage(HWND h, UINT m, WPARAM w, LPARAM l, LRESULT &r)
{
    BOOL ret = BkProcessWindowMessage(h, m, w, l, &r);
    if (WM_CREATE == m)
    {
        bool ok = false;
        if (BkWebView v = BkGetWebView(h))
        {
            BkWebViewClient client = client_impl<T, web_view_client_root>::get_client();
            BkWebViewSetClient(v, &client);

            ok = static_cast<T *>(this)->OnCreate(v, reinterpret_cast<LPCREATESTRUCT>(l));
        }

        if (!ok)
            r = -1;
        ret = TRUE;
    }
    return ret;
}
#endif

inline js_call* js_call::prepare(std::function<void(BkJSContext)> &&callback)
{
    return new js_call(std::move(callback));
}

inline void BKAPI js_call::callback_impl(BkJSContext ctx, void *userData)
{
    js_call *This = reinterpret_cast<js_call *>(userData);
    (This->m_callback)(ctx);
    delete This;
}

} // namespace bk

#endif // BLINKIT_SDK_BK_HPP
