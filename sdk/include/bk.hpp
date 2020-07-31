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

namespace bk {

template <typename C>
class buffer_adaptor
{
public:
    buffer_adaptor(C &data)
    {
        m_buffer.Allocator = Alloc;
        m_buffer.UserData = &data;
    }
    operator BkBuffer* (void) const
    {
        BkBuffer *buf = const_cast<BkBuffer *>(&m_buffer);
        return buf;
    }
private:
    static void* BKAPI Alloc(size_t size, void *user_data)
    {
        C *c = reinterpret_cast<C *>(user_data);
        c->resize(size / sizeof(typename C::value_type));
        return const_cast<typename C::value_type *>(c->data());
    }
    BkBuffer m_buffer;
};

template <typename CharType>
inline buffer_adaptor<std::basic_string<CharType>> make_buffer(std::basic_string<CharType> &s)
{
    return buffer_adaptor<std::basic_string<CharType>>(s);
}

inline buffer_adaptor<std::vector<unsigned char>> make_buffer(std::vector<unsigned char> &v)
{
    return buffer_adaptor<std::vector<unsigned char>>(v);
}

template <class T, typename Client>
class client_root_impl
{
protected:
    Client get_client(void) const
    {
        Client client = { 0 };
        client.SizeOfStruct = sizeof(Client);
        client.UserData = const_cast<T *>(static_cast<const T *>(this));
        adjust_raw_client(client);
        return client;
    }
    static T* get_client_root(void *p)
    {
        return reinterpret_cast<T *>(p);
    }
private:
    virtual void adjust_raw_client(Client &) const {}
};

template <class T, class ClientRoot>
class client_impl : public ClientRoot
{
protected:
    static T* get_this(void *p)
    {
        return static_cast<T *>(ClientRoot::get_client_root(p));
    }
};

class crawler_client_root : public client_root_impl<crawler_client_root, BkCrawlerClient>
{
public:
    BkCrawler create_crawler(void) const
    {
        BkCrawlerClient client = get_client();
        return BkCreateCrawler(&client);
    }
protected:
    void adjust_raw_client(BkCrawlerClient &client) const override
    {
        client.DocumentReady = document_ready_callback;
        client.GetConfig = get_config_callback;
        client.Error = error_callback;
    }
    virtual bool get_crawler_config(int cfg, std::string &dst) const
    {
        switch (cfg)
        {
            case BK_CFG_REQUEST_PROXY:
                // Leave it empty to apply system proxy.
                break;
            default:
                return false;
        }
        return true;
    }
private:
    virtual void document_ready(void) = 0;
    virtual void on_crawler_error(int code, const char *url)
    {
        assert(false); // TODO: Process error!
    }

    static void BKAPI document_ready_callback(void *p)
    {
        get_client_root(p)->document_ready();
    }
    static bool_t BKAPI get_config_callback(int cfg, BkBuffer *dst, void *p)
    {
        std::string s;
        if (!get_client_root(p)->get_crawler_config(cfg, s))
            return false;
        BkSetBufferData(dst, s.data(), s.length());
        return true;
    }
    static void BKAPI error_callback(int code, const char *url, void *p)
    {
        get_client_root(p)->on_crawler_error(code, url);
    }
};

class request_client_root : public client_root_impl<request_client_root, BkRequestClient>
{
public:
    BkRequest create_request(const char *url) const
    {
        BkRequestClient client = get_client();
        return BkCreateRequest(url, &client);
    }
protected:
    void adjust_raw_client(BkRequestClient &client) const override
    {
        client.RequestComplete = request_complete_callback;
        client.RequestFailed = request_failed_callback;
    }
private:
    virtual void request_complete(BkResponse response) = 0;
    virtual void on_request_failed(int code)
    {
        assert(false); // TODO: Process error!
    }

    static void BKAPI request_complete_callback(BkResponse response, void *p)
    {
        get_client_root(p)->request_complete(response);
    }
    static void BKAPI request_failed_callback(int code, void *p)
    {
        get_client_root(p)->on_request_failed(code);
    }
};

template <class T>
class crawler_client_impl : public client_impl<T, crawler_client_root>
{
};

template <class T>
class request_client_impl : public client_impl<T, request_client_root>
{
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
    static js_object user_object_from(BkCrawler crawler)
    {
        BkJSContext ctx = BkGetScriptContextFromCrawler(crawler);
        return js_object(BkGetUserObject(ctx));
    }

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
    static js_function* prepare(BkCrawler crawler, int ctx, const char *name)
    {
        js_function *ret = nullptr;
        BkJSContext js_ctx = BkGetScriptContextFromCrawler(crawler);
        BkJSCallerContext fn_ctx = BkPrepareFunctionCall(js_ctx, ctx, name);
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
            BkPushStringPiece(m_ctx, s.data(), s.length());
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
    function_manager(BkJSContext ctx) : m_ctx(ctx) {}

    void register_function(int memberContext, const char *name, const user_function &fn)
    {
        m_functions.push_back(fn);
        BkRegisterFunction(m_ctx, memberContext, name, impl, &(m_functions.back()));
    }
private:
    static void BKAPI impl(BkJSCalleeContext ctx, void *p)
    {
        function_context fnctx(ctx);
        user_function *fn = reinterpret_cast<user_function *>(p);
        (*fn)(fnctx);
    }

    BkJSContext m_ctx;
    std::list<user_function> m_functions;
};

} // namespace bk

#endif // BLINKIT_SDK_BK_HPP
