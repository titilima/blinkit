// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_binding_impl.cpp
// Description: Binding Implementations
//      Author: Ziming Li
//     Created: 2019-05-31
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_binding_impl.h"

#include "bindings/duk_element.h"
#include "bindings/duk_exception_state.h"
#include "wrappers/duk.h"

using namespace blink;

namespace BlinKit {
namespace Crawler {

duk_ret_t ElementAttributeGetter(duk_context *ctx, const char *name)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);
    return Duk::PushString(ctx, element->getAttribute(AtomicString(name)));
}

duk_ret_t ElementAttributeSetter(duk_context *ctx, const char *name)
{
    duk_push_this(ctx);
    Element *element = DukEventTarget::GetNativeThis<Element>(ctx);

    DukExceptionState es(ctx, name, DukElement::ProtoName);
    element->setAttribute(AtomicString(name), Duk::ToAtomicString(ctx, 0), es);
    if (es.hadException())
        es.throwIfNeeded();
    return 0;
}

duk_ret_t NothingToDo(duk_context *ctx)
{
    return 0;
}

} // namespace Crawler
} // namespace BlinKit
