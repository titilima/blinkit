// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HTMLMeterElement.cpp
// Description: HTMLMeterElement Class
//      Author: Ziming Li
//     Created: 2021-08-03
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "./HTMLMeterElement.h"

#include "blinkit/blink/renderer/bindings/core/exception_state_placeholder.h"
#include "blinkit/blink/renderer/core/HTMLNames.h"
#include "blinkit/blink/renderer/core/dom/ExceptionCode.h"
#include "blinkit/blink/renderer/core/dom/shadow/ShadowRoot.h"
#include "blinkit/blink/renderer/core/frame/UseCounter.h"
#include "blinkit/blink/renderer/core/html/parser/HTMLParserIdioms.h"
#include "blinkit/blink/renderer/core/html/shadow/MeterShadowElement.h"
#include "blinkit/blink/renderer/core/layout/LayoutMeter.h"
#include "blinkit/blink/renderer/core/layout/LayoutTheme.h"

namespace blink {

using namespace HTMLNames;

HTMLMeterElement::HTMLMeterElement(Document& document)
    : LabelableElement(meterTag, document)
{
    UseCounter::count(document, UseCounter::MeterElement);
}

HTMLMeterElement::~HTMLMeterElement()
{
}

PassRefPtrWillBeRawPtr<HTMLMeterElement> HTMLMeterElement::create(Document& document)
{
    RefPtrWillBeRawPtr<HTMLMeterElement> meter = adoptRefWillBeNoop(new HTMLMeterElement(document));
    meter->ensureUserAgentShadowRoot();
    return meter.release();
}

LayoutObject* HTMLMeterElement::createLayoutObject(const ComputedStyle& style)
{
    switch (style.appearance()) {
    case ContinuousCapacityLevelIndicatorPart:
        UseCounter::count(document(), UseCounter::MeterElementWithContinuousCapacityAppearance);
        break;
    case DiscreteCapacityLevelIndicatorPart:
        UseCounter::count(document(), UseCounter::MeterElementWithDiscreteCapacityAppearance);
        break;
    case MeterPart:
        UseCounter::count(document(), UseCounter::MeterElementWithMeterAppearance);
        break;
    case NoControlPart:
        UseCounter::count(document(), UseCounter::MeterElementWithNoneAppearance);
        break;
    case RatingLevelIndicatorPart:
        UseCounter::count(document(), UseCounter::MeterElementWithRatingAppearance);
        break;
    case RelevancyLevelIndicatorPart:
        UseCounter::count(document(), UseCounter::MeterElementWithRelevancyAppearance);
        break;
    default:
        break;
    }
    if (openShadowRoot() || !LayoutTheme::theme().supportsMeter(style.appearance()))
        return LayoutObject::createObject(this, style);
    return new LayoutMeter(this);
}

void HTMLMeterElement::parseAttribute(const QualifiedName& name, const AtomicString& oldValue, const AtomicString& value)
{
    if (name == valueAttr || name == minAttr || name == maxAttr || name == lowAttr || name == highAttr || name == optimumAttr)
        didElementStateChange();
    else
        LabelableElement::parseAttribute(name, oldValue, value);
}

double HTMLMeterElement::value() const
{
    double value = getFloatingPointAttribute(valueAttr, 0);
    return std::min(std::max(value, min()), max());
}

void HTMLMeterElement::setValue(double value)
{
    setFloatingPointAttribute(valueAttr, value);
}

double HTMLMeterElement::min() const
{
    return getFloatingPointAttribute(minAttr, 0);
}

void HTMLMeterElement::setMin(double min)
{
    setFloatingPointAttribute(minAttr, min);
}

double HTMLMeterElement::max() const
{
    return std::max(getFloatingPointAttribute(maxAttr, std::max(1.0, min())), min());
}

void HTMLMeterElement::setMax(double max)
{
    setFloatingPointAttribute(maxAttr, max);
}

double HTMLMeterElement::low() const
{
    double low = getFloatingPointAttribute(lowAttr, min());
    return std::min(std::max(low, min()), max());
}

void HTMLMeterElement::setLow(double low)
{
    setFloatingPointAttribute(lowAttr, low);
}

double HTMLMeterElement::high() const
{
    double high = getFloatingPointAttribute(highAttr, max());
    return std::min(std::max(high, low()), max());
}

void HTMLMeterElement::setHigh(double high)
{
    setFloatingPointAttribute(highAttr, high);
}

double HTMLMeterElement::optimum() const
{
    double optimum = getFloatingPointAttribute(optimumAttr, (max() + min()) / 2);
    return std::min(std::max(optimum, min()), max());
}

void HTMLMeterElement::setOptimum(double optimum)
{
    setFloatingPointAttribute(optimumAttr, optimum);
}

HTMLMeterElement::GaugeRegion HTMLMeterElement::gaugeRegion() const
{
    double lowValue = low();
    double highValue = high();
    double theValue = value();
    double optimumValue = optimum();

    if (optimumValue < lowValue) {
        // The optimum range stays under low
        if (theValue <= lowValue)
            return GaugeRegionOptimum;
        if (theValue <= highValue)
            return GaugeRegionSuboptimal;
        return GaugeRegionEvenLessGood;
    }

    if (highValue < optimumValue) {
        // The optimum range stays over high
        if (highValue <= theValue)
            return GaugeRegionOptimum;
        if (lowValue <= theValue)
            return GaugeRegionSuboptimal;
        return GaugeRegionEvenLessGood;
    }

    // The optimum range stays between high and low.
    // According to the standard, <meter> never show GaugeRegionEvenLessGood in this case
    // because the value is never less or greater than min or max.
    if (lowValue <= theValue && theValue <= highValue)
        return GaugeRegionOptimum;
    return GaugeRegionSuboptimal;
}

double HTMLMeterElement::valueRatio() const
{
    double min = this->min();
    double max = this->max();
    double value = this->value();

    if (max <= min)
        return 0;
    return (value - min) / (max - min);
}

void HTMLMeterElement::didElementStateChange()
{
    m_value->setWidthPercentage(valueRatio()*100);
    m_value->updatePseudo();
    if (LayoutMeter* layoutMeter = this->layoutMeter())
        layoutMeter->updateFromElement();
}

LayoutMeter* HTMLMeterElement::layoutMeter() const
{
    if (layoutObject() && layoutObject()->isMeter())
        return toLayoutMeter(layoutObject());

    LayoutObject* layoutObject = userAgentShadowRoot()->firstChild()->layoutObject();
    return toLayoutMeter(layoutObject);
}

void HTMLMeterElement::didAddUserAgentShadowRoot(ShadowRoot& root)
{
    ASSERT(!m_value);

    RefPtrWillBeRawPtr<MeterInnerElement> inner = MeterInnerElement::create(document());
    root.appendChild(inner);

    RefPtrWillBeRawPtr<MeterBarElement> bar = MeterBarElement::create(document());
    m_value = MeterValueElement::create(document());
    m_value->setWidthPercentage(0);
    m_value->updatePseudo();
    bar->appendChild(m_value);

    inner->appendChild(bar);
}

void HTMLMeterElement::willAddFirstAuthorShadowRoot()
{
    ASSERT(RuntimeEnabledFeatures::authorShadowDOMForAnyElementEnabled());
    lazyReattachIfAttached();
}

DEFINE_TRACE(HTMLMeterElement)
{
    visitor->trace(m_value);
    LabelableElement::trace(visitor);
}

} // namespace
