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
    virtual void document_ready(BkCrawler crawler) = 0;
    virtual void on_crawler_error(int code, const char *url) { assert(false); /* TODO: Process error yourself! */ }

    static void BKAPI document_ready_callback(BkCrawler crawler, void *p);
    static bool_t BKAPI get_config_callback(int cfg, BkBuffer *dst, void *p);
    static void BKAPI error_callback(int code, const char *url, void *p);
};

class request_client_root : public client_root_impl<request_client_root, BkRequestClient> {
public:
    BkRequest create_request(const char *url) const;
    BkRequest create_request_for_app_thread(const char *url) const;
protected:
    void adjust_raw_client(BkRequestClient &client) const override;
private:
    virtual void request_complete(BkResponse response) = 0;
    virtual void on_request_failed(int code) { assert(false); /* TODO: Process error yourself! */ }

    static void BKAPI request_complete_callback(BkResponse response, void *p);
    static void BKAPI request_failed_callback(int code, void *p);
};

class web_view_client_root : public client_root_impl<web_view_client_root, BkWebViewClient> {
protected:
    void adjust_raw_client(BkWebViewClient &client) const override;
private:
    virtual void document_ready(BkWebView v) = 0;
    virtual void size_changed(BkWebView v, int width, int height) {}

    static void BKAPI document_ready_callback(BkWebView v, void *p);
    static void BKAPI size_changed_callback(BkWebView v, int w, int h, void *p);
};

template <class T>
class crawler_client_impl : public client_impl<T, crawler_client_root> {};

template <class T>
class request_client_impl : public client_impl<T, request_client_root> {};

template <class T>
class web_view_client_impl : public client_impl<T, web_view_client_root> {};

#ifdef __ATLWIN_H__

template <class T, class TBase = ATL::CWindow, class TWinTraits = ATL::CControlWinTraits>
class ATL_NO_VTABLE web_view_window_impl : public ATL::CWindowImplBaseT<TBase, TWinTraits>
                                         , public web_view_client_impl<T>
{
public:
    static PCTSTR GetWndCaption(void) { return nullptr; }

    HWND Create(HWND hWndParent, ATL::_U_RECT rect = nullptr, PCTSTR szWindowName = nullptr,
        DWORD dwStyle = 0, DWORD dwExStyle = 0, ATL::_U_MENUorID MenuOrID = 0U);

    virtual void OnFinalMessage(HWND) {}
protected:
    void adjust_raw_client(BkWebViewClient &client) const override;
private:
    static bool_t BKAPI process_message_callback(HWND h, UINT m, WPARAM w, LPARAM l, LRESULT *r, void *p);
};

#endif

template <class T>
class click_handler_impl1
{
public:
    using callback = void (T::*)(void);
    void add_click_observer(BkWebView v, const char *id, callback cb);
private:
    struct context;
    static void BKAPI callback_impl(void *p);
    std::list<context> m_contexts;
};

template <class T>
class click_handler_impl2
{
public:
    using callback = std::function<void()>;
    void add_click_observer(BkWebView v, const char *id, callback &&cb);
private:
    static void BKAPI callback_impl(void *p);
    std::list<callback> m_callbacks;
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

inline void BKAPI crawler_client_root::document_ready_callback(BkCrawler crawler, void *p)
{
    get_client_root(p)->document_ready(crawler);
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

inline BkRequest request_client_root::create_request_for_app_thread(const char *url) const
{
    BkRequestClient client = get_client();
    return BkCreateRequestForAppThread(url, &client);
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
    client.SizeChanged = size_changed_callback;
}

inline void BKAPI web_view_client_root::document_ready_callback(BkWebView v, void *p)
{
    get_client_root(p)->document_ready(v);
}

inline void BKAPI web_view_client_root::size_changed_callback(BkWebView v, int w, int h, void *p)
{
    get_client_root(p)->size_changed(v, w, h);
}

#ifdef __ATLWIN_H__

template <class T, class TBase, class TWinTraits>
void web_view_window_impl<T, TBase, TWinTraits>::adjust_raw_client(BkWebViewClient &client) const
{
    web_view_client_impl<T>::adjust_raw_client(client);
    client.ProcessMessage = process_message_callback;
}

template <class T, class TBase, class TWinTraits>
HWND web_view_window_impl<T, TBase, TWinTraits>::Create(
    HWND hWndParent,
    ATL::_U_RECT rect,
    PCTSTR szWindowName,
    DWORD dwStyle, DWORD dwExStyle,
    ATL::_U_MENUorID MenuOrID)
{
    ATL::CWndClassInfo &wc = T::GetWndClassInfo();
    if (nullptr == wc.m_lpszOrigName)
        wc.m_lpszOrigName = this->GetWndClassName();
    wc.m_wc.lpfnWndProc = ::BkDefWindowProc;
    WNDPROC p = nullptr;
    ATOM atom = wc.Register(&p);

    dwStyle = T::GetWndStyle(dwStyle);
    dwExStyle = T::GetWndExStyle(dwExStyle);

    if (szWindowName == NULL)
        szWindowName = T::GetWndCaption();

    BkWebViewClient client = web_view_client_impl<T>::get_client();
    return CWindowImplBaseT<TBase, TWinTraits>::Create(hWndParent, rect, szWindowName,
        dwStyle, dwExStyle, MenuOrID, atom, &client);
}

template <class T, class TBase, class TWinTraits>
bool_t BKAPI web_view_window_impl<T, TBase, TWinTraits>::process_message_callback(HWND h, UINT m, WPARAM w, LPARAM l, LRESULT *r, void *p)
{
    T *pThis = web_view_client_impl<T>::get_this(p);

    if (WM_NCCREATE == m)
        pThis->m_hWnd = h;

    auto b = pThis->ProcessWindowMessage(h, m, w, l, *r);

    if (WM_NCDESTROY == m)
    {
        pThis->m_hWnd = nullptr;
        static_cast<web_view_window_impl<T, TBase, TWinTraits> *>(pThis)->OnFinalMessage(h);
    }
    return b;
}

#endif

template <class T>
struct click_handler_impl1<T>::context
{
    callback cb;
    T *p;
    void execute(void) { (p->*cb)(); }

    context(callback cb, T *p) : cb(cb), p(p) {}
};

template <class T>
void click_handler_impl1<T>::add_click_observer(BkWebView v, const char *id, callback cb)
{
    context &ctx = m_contexts.emplace_back(cb, static_cast<T *>(this));
    BkAddClickObserver(v, id, click_handler_impl1::callback_impl, &ctx);
}

template <class T>
void BKAPI click_handler_impl1<T>::callback_impl(void *p)
{
    reinterpret_cast<context *>(p)->execute();
}

template <class T>
void click_handler_impl2<T>::add_click_observer(BkWebView v, const char *id, callback &&cb)
{
    callback &cb2 = m_callbacks.emplace_back(std::move(cb));
    BkAddClickObserver(v, id, click_handler_impl2::callback_impl, &cb2);
}

template <class T>
void BKAPI click_handler_impl2<T>::callback_impl(void *p)
{
    callback *cb = reinterpret_cast<callback *>(p);
    (*cb)();
}

} // namespace bk

#endif // BLINKIT_SDK_BK_HPP
