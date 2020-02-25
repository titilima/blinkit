// -------------------------------------------------
// BlinKit SDK
// -------------------------------------------------
//   File Name: BlinKit.hpp
//      Author: Ziming Li
//     Created: 2020-02-25
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SDK_BLINKIT_HPP
#define BLINKIT_SDK_BLINKIT_HPP

#pragma once

#include <cassert>
#include <cstddef>
#include <cstring>
#include <string>
#include <vector>
#include "bk_crawler.h"
#include "bk_http.h"

namespace BlinKit {

/*
 * BkBufferImpl
 */

template <typename Container>
class BkBufferImpl {
public:
    BkBufferImpl(Container &data)
    {
        m_buffer.Allocator = Alloc;
        m_buffer.UserData = &data;
    }
    operator BkBuffer* (void) const {
        BkBuffer *buf = const_cast<BkBuffer *>(&m_buffer);
        return buf;
    }
private:
    static void* BKAPI Alloc(size_t size, void *This) {
        Container *c = reinterpret_cast<Container *>(This);
        c->resize(size / sizeof(typename Container::value_type));
        return const_cast<typename Container::value_type *>(c->data());
    }
    BkBuffer m_buffer;
};

template <typename CharType>
inline BkBufferImpl<std::basic_string<CharType>> BkMakeBuffer(std::basic_string<CharType> &s)
{
    return BkBufferImpl<std::basic_string<CharType>>(s);
}

inline BkBufferImpl<std::vector<unsigned char>> BkMakeBuffer(std::vector<unsigned char> &v)
{
    return BkBufferImpl<std::vector<unsigned char>>(v);
}

/**
 * BkClientImpl
 */

template <class T, typename ClientType>
class BkClientImpl
{
public:
    operator ClientType* (void)
    {
        if (nullptr == m_rawClient.UserData)
        {
            m_rawClient.UserData = static_cast<T *>(this);
            static_cast<T *>(this)->Attach(m_rawClient);
        }
        return &m_rawClient;
    }
protected:
    BkClientImpl(void)
    {
        memset(&m_rawClient, 0, sizeof(ClientType));
    }
    static T* ToImpl(void *userData)
    {
        return reinterpret_cast<T *>(userData);
    }
private:
    virtual void Attach(ClientType &rawClient) = 0;

    ClientType m_rawClient;
};

/**
 * BkRequestClientImpl
 */

class BkRequestClientImpl : public BkClientImpl<BkRequestClientImpl, BkRequestClient>
{
    template <class T, typename C> friend class BkClientImpl;
protected:
    void Attach(BkRequestClient &rawClient) override
    {
        rawClient.RequestComplete = RequestCompleteImpl;
        rawClient.RequestFailed = RequestFailedImpl;
        rawClient.RequestRedirect = RequestRedirectImpl;
    }
private:
    virtual void RequestComplete(BkResponse response) = 0;
    virtual void RequestFailed(int errorCode) { assert(BK_ERR_SUCCESS == errorCode); }
    virtual bool_t RequestRedirect(BkResponse response) { return true; }

    static void BKAPI RequestCompleteImpl(BkResponse response, void *userData)
    {
        ToImpl(userData)->RequestComplete(response);
    }
    static void BKAPI RequestFailedImpl(int errorCode, void *userData)
    {
        ToImpl(userData)->RequestFailed(errorCode);
    }
    static bool_t BKAPI RequestRedirectImpl(BkResponse response, void *userData)
    {
        return ToImpl(userData)->RequestRedirect(response);
    }
};

/**
 * BkCrawlerClientImpl
 */

class BkCrawlerClientImpl : public BkClientImpl<BkCrawlerClientImpl, BkCrawlerClient>
{
    template <class T, typename C> friend class BkClientImpl;
protected:
    void Attach(BkCrawlerClient &rawClient) override
    {
        rawClient.GetConfig = GetConfigImpl;
        rawClient.RequestComplete = RequestCompleteImpl;
        rawClient.DocumentReady = DocumentReadyImpl;
        rawClient.Error = ErrorImpl;
    }
    virtual void RequestComplete(BkResponse response, BkWorkController controller)
    {
        BkControllerContinueWorking(controller);
    }
private:
    virtual std::string GetCrawlerConfig(int cfg)
    {
        assert(false); // Not implemented!
        return std::string();
    }
    virtual void DocumentReady(void) = 0;
    virtual void Error(int errorCode, const char *URL)
    {
        assert(BK_ERR_SUCCESS == errorCode);
    }

    static void BKAPI GetConfigImpl(int cfg, BkBuffer *dst, void *userData)
    {
        std::string s = ToImpl(userData)->GetCrawlerConfig(cfg);
        if (!s.empty())
            BkSetBufferData(dst, s.data(), s.length());
    }
    static void BKAPI RequestCompleteImpl(BkResponse response, BkWorkController controller, void *userData)
    {
        ToImpl(userData)->RequestComplete(response, controller);
    }
    static void BKAPI DocumentReadyImpl(void *userData)
    {
        ToImpl(userData)->DocumentReady();
    }
    static void BKAPI ErrorImpl(int errorCode, const char *URL, void *userData)
    {
        ToImpl(userData)->Error(errorCode, URL);
    }
};

} // namespace BlinKit

#endif // BLINKIT_SDK_BLINKIT_HPP
