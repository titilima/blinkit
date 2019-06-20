// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_xhr.h
// Description: Bindings for XHR
//      Author: Ziming Li
//     Created: 2019-06-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_XHR_H
#define BLINKIT_JS_DUK_XHR_H

#pragma once

#include <condition_variable>
#include <mutex>
#include "platform/bindings/script_wrappable.h"

namespace BlinKit {

class PrototypeManager;

class DukXHR final : public blink::ScriptWrappable, public BkRequestClient
{
public:
    static const char ProtoName[];

    int GetReadyState(void) const { return m_readyState; }
    void SetReadyState(duk_context *ctx, int newState);

    duk_ret_t Open(duk_context *ctx, const std::string &method, const std::string &URL, bool async,
        const std::string username, const std::string &password);
    duk_ret_t Send(duk_context *ctx);
    duk_ret_t GetStatus(duk_context *ctx);
    duk_ret_t GetResponseText(duk_context *ctx);

    static DukXHR* Get(duk_context *ctx, duk_idx_t idx = -1);
    static duk_ret_t Constructor(duk_context *ctx);

    static void RegisterPrototype(duk_context *ctx, PrototypeManager &protos);
private:
    DukXHR(duk_context *ctx);
    ~DukXHR(void);

    static duk_ret_t Finalizer(duk_context *ctx);

    // BkRequestClient
    void BKAPI RequestComplete(const BkResponse &response) override;
    void BKAPI RequestFailed(int errorCode) override;

    int m_readyState = 0;
    void *m_heapPtr;
    bool m_signal = false;
    std::mutex m_mutex;
    std::condition_variable m_cv;

    class Session;
    std::unique_ptr<Session> m_session;
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_XHR_H
