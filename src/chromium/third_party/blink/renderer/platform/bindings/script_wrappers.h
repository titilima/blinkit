// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_wrappers.h
// Description: Script Wrapper Classes
//      Author: Ziming Li
//     Created: 2020-03-15
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_SCRIPT_WRAPPERS_H
#define BLINKIT_BLINK_SCRIPT_WRAPPERS_H

#pragma once

#include "base/macros.h"
#include "duktape/duktape.h"
#include "third_party/blink/renderer/platform/wtf/allocator.h"

namespace blink {
class Node;
class ScriptWrappable;
}

namespace BlinKit {

class PushWrapper
{
    STACK_ALLOCATED();
    DISALLOW_COPY_AND_ASSIGN(PushWrapper);
public:
    PushWrapper(duk_context *ctx, blink::ScriptWrappable *nativeObject);
    ~PushWrapper(void);
private:
    duk_context *m_ctx;
    blink::ScriptWrappable *m_nativeObject;
};

class NodePushWrapper : public PushWrapper
{
public:
    NodePushWrapper(duk_context *ctx, blink::Node *node);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_SCRIPT_WRAPPERS_H
