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
#include "blinkit/blink/renderer/core/dom/Element.h"

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

BKEXPORT void BKAPI BkGetElementPosition(BkElement e, struct BkRect *dst)
{
    IntRect rc;
    const auto callback = [&rc](Element &e) {
        rc = e.pixelSnappedBoundingBox();
    };
    e->SafeAccess(callback);

    dst->location.x = rc.x();
    dst->location.y = rc.y();
    dst->size.width = rc.width();
    dst->size.height = rc.height();
}

} // extern "C"
