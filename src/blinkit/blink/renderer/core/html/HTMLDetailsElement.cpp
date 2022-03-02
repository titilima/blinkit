// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HTMLDetailsElement.cpp
// Description: HTMLDetailsElement Class
//      Author: Ziming Li
//     Created: 2021-11-06
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#include "./HTMLDetailsElement.h"

#include "blinkit/blink/renderer/bindings/core/v8/ExceptionStatePlaceholder.h"
#include "blinkit/blink/renderer/core/CSSPropertyNames.h"
#include "blinkit/blink/renderer/core/CSSValueKeywords.h"
#include "blinkit/blink/renderer/core/HTMLNames.h"
#include "blinkit/blink/renderer/core/dom/element_traversal.h"
#include "blinkit/blink/renderer/core/dom/Text.h"
#include "blinkit/blink/renderer/core/dom/shadow/ShadowRoot.h"
#include "blinkit/blink/renderer/core/events/Event.h"
#include "blinkit/blink/renderer/core/events/EventSender.h"
#include "blinkit/blink/renderer/core/frame/UseCounter.h"
#include "blinkit/blink/renderer/core/html/HTMLContentElement.h"
#include "blinkit/blink/renderer/core/html/HTMLDivElement.h"
#include "blinkit/blink/renderer/core/html/HTMLSummaryElement.h"
#include "blinkit/blink/renderer/core/html/shadow/DetailsMarkerControl.h"
#include "blinkit/blink/renderer/core/html/shadow/ShadowElementNames.h"
#include "blinkit/blink/renderer/core/layout/LayoutBlockFlow.h"
#include "blinkit/blink/renderer/platform/text/PlatformLocale.h"

namespace blink {

using namespace HTMLNames;

class FirstSummarySelectFilter final : public HTMLContentSelectFilter {
public:
    virtual ~FirstSummarySelectFilter() { }

    static PassOwnPtrWillBeRawPtr<FirstSummarySelectFilter> create()
    {
        return adoptPtrWillBeNoop(new FirstSummarySelectFilter());
    }

    bool canSelectNode(const WillBeHeapVector<RawPtrWillBeMember<Node>, 32>& siblings, int nth) const override
    {
        if (!siblings[nth]->hasTagName(HTMLNames::summaryTag))
            return false;
        for (int i = nth - 1; i >= 0; --i) {
            if (siblings[i]->hasTagName(HTMLNames::summaryTag))
                return false;
        }
        return true;
    }

    DEFINE_INLINE_VIRTUAL_TRACE()
    {
        HTMLContentSelectFilter::trace(visitor);
    }

private:
    FirstSummarySelectFilter() { }
};

static DetailsEventSender& detailsToggleEventSender()
{
    DEFINE_STATIC_LOCAL(OwnPtrWillBePersistent<DetailsEventSender>, sharedToggleEventSender, (DetailsEventSender::create(EventTypeNames::toggle)));
    return *sharedToggleEventSender;
}

PassRefPtrWillBeRawPtr<HTMLDetailsElement> HTMLDetailsElement::create(Document& document)
{
    RefPtrWillBeRawPtr<HTMLDetailsElement> details = adoptRefWillBeNoop(new HTMLDetailsElement(document));
    details->ensureUserAgentShadowRoot();
    return details.release();
}

HTMLDetailsElement::HTMLDetailsElement(Document& document)
    : HTMLElement(detailsTag, document)
    , m_isOpen(false)
{
    UseCounter::count(document, UseCounter::DetailsElement);
}

HTMLDetailsElement::~HTMLDetailsElement()
{
#if !ENABLE(OILPAN)
    detailsToggleEventSender().cancelEvent(this);
#endif
}

void HTMLDetailsElement::dispatchPendingEvent(DetailsEventSender* eventSender)
{
    ASSERT_UNUSED(eventSender, eventSender == &detailsToggleEventSender());
    dispatchEvent(Event::create(EventTypeNames::toggle));
}


LayoutObject* HTMLDetailsElement::createLayoutObject(const ComputedStyle&)
{
    return new LayoutBlockFlow(this);
}

void HTMLDetailsElement::didAddUserAgentShadowRoot(ShadowRoot& root)
{
    RefPtrWillBeRawPtr<HTMLSummaryElement> defaultSummary = HTMLSummaryElement::create(document());
    GCRefPtr<Text> text = Text::create(document(), locale().queryString(WebLocalizedString::DetailsLabel));
    defaultSummary->appendChild(text.get());

    RefPtrWillBeRawPtr<HTMLContentElement> summary = HTMLContentElement::create(document(), FirstSummarySelectFilter::create());
    summary->setIdAttribute(ShadowElementNames::detailsSummary());
    summary->appendChild(defaultSummary);
    root.appendChild(summary.release());

    RefPtrWillBeRawPtr<HTMLDivElement> content = HTMLDivElement::create(document());
    content->setIdAttribute(ShadowElementNames::detailsContent());
    content->appendChild(HTMLContentElement::create(document()));
    content->setInlineStyleProperty(CSSPropertyDisplay, CSSValueNone);
    root.appendChild(content.release());
}

Element* HTMLDetailsElement::findMainSummary() const
{
    if (HTMLSummaryElement* summary = Traversal<HTMLSummaryElement>::firstChild(*this))
        return summary;

    HTMLContentElement* content = toHTMLContentElement(userAgentShadowRoot()->firstChild());
    ASSERT(content->firstChild() && isHTMLSummaryElement(*content->firstChild()));
    return toElement(content->firstChild());
}

void HTMLDetailsElement::parseAttribute(const QualifiedName& name, const AtomicString& oldValue, const AtomicString& value)
{
    if (name == openAttr) {
        bool oldValue = m_isOpen;
        m_isOpen = !value.isNull();
        if (m_isOpen == oldValue)
            return;

        // Dispatch toggle event asynchronously.
        detailsToggleEventSender().cancelEvent(this);
        detailsToggleEventSender().dispatchEventSoon(this);

        Element* content = ensureUserAgentShadowRoot().getElementById(ShadowElementNames::detailsContent());
        ASSERT(content);
        if (m_isOpen)
            content->removeInlineStyleProperty(CSSPropertyDisplay);
        else
            content->setInlineStyleProperty(CSSPropertyDisplay, CSSValueNone);

        // Invalidate the LayoutDetailsMarker in order to turn the arrow signifying if the
        // details element is open or closed.
        Element* summary = findMainSummary();
        ASSERT(summary);

        Element* control = toHTMLSummaryElement(summary)->markerControl();
        if (control && control->layoutObject())
            control->layoutObject()->setShouldDoFullPaintInvalidation();

        return;
    }
    HTMLElement::parseAttribute(name, oldValue, value);
}

void HTMLDetailsElement::toggleOpen()
{
    setAttribute(openAttr, m_isOpen ? nullAtom : emptyAtom);
}

bool HTMLDetailsElement::isInteractiveContent() const
{
    return true;
}

}
