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

#include "blinkit/blink/renderer/bindings/core/v8/ExceptionStatePlaceholder.h"
#include "blinkit/blink/renderer/core/dom/DOMTokenList.h"
#include "blinkit/blink/renderer/core/dom/Element.h"
#include "blinkit/blink/renderer/core/html/HTMLInputElement.h"

using namespace blink;
using namespace BlinKit;

ElementImpl::ElementImpl(Element &element) : m_element(element)
{
    // Nothing
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern "C" {

BKEXPORT void BKAPI BkAddClassToElement(BkElement e, const char *className)
{
    Element &re = e->GetRawElement();

    DOMTokenList &classList = re.classList();
    classList.add(AtomicString::fromUTF8(className), ASSERT_NO_EXCEPTION);
}

BKEXPORT bool_t BKAPI BkGetElementAttribute(BkElement e, const char *name, struct BkBuffer *dst)
{
    const AtomicString &value = e->GetRawElement().getAttribute(AtomicString::fromUTF8(name));
    if (value.isNull())
        return false;

    if (nullptr != dst)
    {
        std::string s = value.stdUTF8();
        BkSetBufferData(dst, s.data(), s.length());
    }
    return true;
}

BKEXPORT void BKAPI BkGetElementPosition(BkElement e, struct BkRect *dst, unsigned)
{
    IntRect rc = e->GetRawElement().boundsInViewport();

    dst->location.x = rc.x();
    dst->location.y = rc.y();
    dst->size.width = rc.width();
    dst->size.height = rc.height();
}

BKEXPORT bool_t BKAPI BkGetElementProperty(BkElement e, int propId, struct BkBuffer *dst)
{
    Element &re = e->GetRawElement();
    switch (propId)
    {
        case BK_PROP_TEXTFIELD_VALUE:
            if (isHTMLInputElement(re))
            {
                HTMLInputElement &input = toHTMLInputElement(re);
                if (input.isTextField())
                {
                    std::string s = input.value().stdUtf8();
                    BkSetBufferData(dst, s.data(), s.length());
                    return true;
                }
            }
            break;
        default:
            NOTREACHED();
    }
    return false;
}

BKEXPORT void BKAPI BkRemoveClassFromElement(BkElement e, const char *className)
{
    Element &re = e->GetRawElement();

    DOMTokenList &classList = re.classList();
    classList.remove(AtomicString::fromUTF8(className), ASSERT_NO_EXCEPTION);
}

BKEXPORT void BKAPI BkSetElementAttribute(BkElement e, const char *name, const char *value)
{
    Element &re = e->GetRawElement();

    const AtomicString n = AtomicString::fromUTF8(name);
    if (nullptr == value)
        re.removeAttribute(n);
    else
        re.setAttribute(n, AtomicString::fromUTF8(value), ASSERT_NO_EXCEPTION);
}

BKEXPORT void BKAPI BkSetElementIntegalAttribute(BkElement e, const char *name, int value)
{
    Element &re = e->GetRawElement();

    re.setAttribute(AtomicString::fromUTF8(name), AtomicString::number(value), ASSERT_NO_EXCEPTION);
}

BKEXPORT bool_t BKAPI BkSetElementProperty(BkElement e, int propId, const char *value, bool notify)
{
    Element &re = e->GetRawElement();
    switch (propId)
    {
        case BK_PROP_TEXTFIELD_VALUE:
            if (isHTMLInputElement(re))
            {
                HTMLInputElement &input = toHTMLInputElement(re);
                if (input.isTextField())
                {
                    input.setValue(String::fromUTF8(value), notify ? DispatchChangeEvent : DispatchNoEvent);
                    return true;
                }
            }
            break;
        default:
            NOTREACHED();
    }
    return false;
}

BKEXPORT void BKAPI BkToggleElementClass(BkElement e, const char *className)
{
    Element &re = e->GetRawElement();

    DOMTokenList &classList = re.classList();
    classList.toggle(AtomicString::fromUTF8(className), ASSERT_NO_EXCEPTION);
}

} // extern "C"
