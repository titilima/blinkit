// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: heap_retained.h
// Description: HeapRetained Class
//      Author: Ziming Li
//     Created: 2020-07-14
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_HEAP_RETAINED_H
#define BLINKIT_BLINKIT_HEAP_RETAINED_H

#pragma once

#include <functional>
#include <string>
#include "duktape/duktape.h"

namespace BlinKit {

class HeapRetained
{
public:
    virtual ~HeapRetained(void);
protected:
    HeapRetained(const std::string &keyPrefix);

    void Retain(duk_context *ctx, duk_idx_t idx);
    void Release(duk_context *ctx);
    duk_idx_t PushTo(duk_context *ctx) const;

    typedef std::function<void(duk_context *, duk_idx_t)> Worker;
    bool SafeAccess(duk_context *ctx, const Worker &worker);
private:
    void *m_heapPtr;
    std::string m_heapKey;
};

class HeapRetainedValue : public HeapRetained
{
public:
    ~HeapRetainedValue(void) override;
protected:
    enum class Type { OBJECT, ARRAY };
    HeapRetainedValue(duk_context *ctx, Type type, const char *keyPrefix);

    duk_context *m_ctx;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_HEAP_RETAINED_H
