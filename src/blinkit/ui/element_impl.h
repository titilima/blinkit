#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: element_impl.h
// Description: ElementImpl Class
//      Author: Ziming Li
//     Created: 2021-11-12
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_ELEMENT_IMPL_H
#define BLINKIT_BLINKIT_ELEMENT_IMPL_H

#include <functional>
#include "bk_ui.h"

namespace blink {
class Element;
class IntRect;
}

class ElementImpl
{
public:
    ElementImpl(blink::Element &element);

    void SafeAccess(const std::function<void(blink::Element &)> &callback) const;
private:
    blink::Element &m_element;
};

#endif // BLINKIT_BLINKIT_ELEMENT_IMPL_H
