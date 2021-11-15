// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: element_impl.cpp
// Description: ElementImpl Class
//      Author: Ziming Li
//     Created: 2021-11-12
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./element_impl.h"

#include "blinkit/app/app_impl.h"
#include "blinkit/blink/renderer/bindings/core/v8/ExceptionStatePlaceholder.h"
#include "blinkit/blink/renderer/core/dom/Element.h"
#include "blinkit/ui/rendering_scheduler.h"

using namespace blink;
using namespace BlinKit;

ElementImpl::ElementImpl(Element &element) : m_element(element)
{
    // Nothing
}

void ElementImpl::SafeAccess(const std::function<void(Element &)> &callback) const
{
    const auto callbackWrapper = [this, &callback] {
        callback(m_element);
    };
    AppImpl::Get().GetAppCaller().SyncCall(BLINK_FROM_HERE, callbackWrapper);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT bool_t BKAPI BkGetElementAttribute(BkElement e, const char *name, struct BkBuffer *dst)
{
    bool ret = false;

    std::optional<std::string> s;
    if (nullptr != dst)
        s.emplace(std::string());

    const auto callback = [name, &ret, &s](Element &e) {
        const AtomicString &value = e.getAttribute(AtomicString::fromUTF8(name));
        if (value.isNull())
            return;

        ret = true;
        if (s.has_value())
            s.emplace(value.stdUTF8());
    };
    e->SafeAccess(callback);

    return ret;
}


BKEXPORT void BKAPI BkGetElementPosition(BkElement e, struct BkRect *dst, unsigned)
{
    IntRect rc;
    const auto callback = [&rc](Element &e) {
        rc = e.boundsInViewport();
    };
    e->SafeAccess(callback);

    dst->location.x = rc.x();
    dst->location.y = rc.y();
    dst->size.width = rc.width();
    dst->size.height = rc.height();
}

BKEXPORT void BKAPI BkSetElementAttribute(BkElement e, const char *name, const char *value)
{
    const auto callback = [name, value](Element &e) {
        RenderingScheduler _(e.document());
        const AtomicString n = AtomicString::fromUTF8(name);
        if (nullptr == value)
            e.removeAttribute(n);
        else
            e.setAttribute(n, AtomicString::fromUTF8(value), ASSERT_NO_EXCEPTION);
    };
    e->SafeAccess(callback);
}

BKEXPORT void BKAPI BkSetElementIntegalAttribute(BkElement e, const char *name, int value)
{
    const auto callback = [name, value](Element &e) {
        RenderingScheduler _(e.document());
        e.setAttribute(AtomicString::fromUTF8(name), AtomicString::number(value), ASSERT_NO_EXCEPTION);
    };
    e->SafeAccess(callback);
}

} // extern "C"
