// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_xhr.h
// Description: DukXHR Class
//      Author: Ziming Li
//     Created: 2020-07-05
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_XHR_H
#define BLINKIT_BLINK_DUK_XHR_H

#pragma once

#include <unordered_map>
#include "bk_http.h"
#include "third_party/blink/renderer/bindings/core/duk/duk_script_object.h"

namespace BlinKit {

class DukXHR final : public DukScriptObject
{
public:
    static const char ProtoName[];

    static duk_ret_t Construct(duk_context *ctx);
    ~DukXHR(void);

    static void RegisterPrototype(PrototypeHelper &helper);

    duk_ret_t Open(duk_context *ctx, duk_idx_t idx, const std::string &method, const WTF::String &URL, bool async,
        const std::string username, const std::string &password);
    duk_ret_t SetRequestHeader(const std::string &name, const std::string &value);
    duk_ret_t Send(duk_context *ctx, duk_idx_t idx, const std::string &body);

    enum class ReadyState {
        Uninitialized = 0,
        Open          = 1,
        Sent          = 2,
        Receiving     = 3,
        Loaded        = 4
    };
    duk_ret_t GetReadyState(duk_context *ctx) const;
    void SetReadyState(duk_context *ctx, duk_idx_t idx, ReadyState newState);

    duk_ret_t GetStatus(duk_context *ctx) const;
    duk_ret_t GetResponseText(duk_context *ctx) const;
private:
    DukXHR(duk_context *ctx, duk_idx_t idx);

    void Reset(void);

    static duk_ret_t Finalizer(duk_context *ctx);
    static void FillPrototypeEntry(PrototypeEntry &entry);

    class Session;
    class AsyncSession;
    class SyncSession;
    std::unique_ptr<Session> CreateSession(duk_context *ctx, duk_idx_t idx, bool async);

    static void BKAPI RequestCompleteImpl(BkResponse response, void *userData);
    static void BKAPI RequestFailedImpl(int errorCode, void *userData);

    void *m_heapPtr;

    ReadyState m_readyState = ReadyState::Uninitialized;
    std::string m_method, m_URL, m_key;
    std::unordered_map<std::string, std::string> m_requestHeaders;

    std::unique_ptr<Session> m_currentSession;
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_XHR_H
