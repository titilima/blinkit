// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: PropertySetCSSStyleDeclaration.cpp
// Description: PropertySetCSSStyleDeclaration Classes
//      Author: Ziming Li
//     Created: 2021-10-10
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2012 Apple Inc. All rights reserved.
 * Copyright (C) 2011 Research In Motion Limited. All rights reserved.
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
 */

#include "./PropertySetCSSStyleDeclaration.h"

#include "blinkit/blink/renderer/bindings/core/exception_state.h"
#include "blinkit/blink/renderer/core/HTMLNames.h"
#include "blinkit/blink/renderer/core/StylePropertyShorthand.h"
#include "blinkit/blink/renderer/core/css/CSSCustomPropertyDeclaration.h"
#include "blinkit/blink/renderer/core/css/CSSKeyframesRule.h"
#include "blinkit/blink/renderer/core/css/CSSStyleSheet.h"
#include "blinkit/blink/renderer/core/css/StylePropertySet.h"
#include "blinkit/blink/renderer/core/css/parser/CSSVariableParser.h"
#include "blinkit/blink/renderer/core/dom/element.h"
#include "blinkit/blink/renderer/core/dom/MutationObserverInterestGroup.h"
#include "blinkit/blink/renderer/core/dom/MutationRecord.h"
#include "blinkit/blink/renderer/core/dom/StyleEngine.h"
#include "blinkit/blink/renderer/core/inspector/InspectorInstrumentation.h"
#include "blinkit/blink/renderer/platform/RuntimeEnabledFeatures.h"

namespace blink {

namespace {

class StyleAttributeMutationScope {
    WTF_MAKE_NONCOPYABLE(StyleAttributeMutationScope);
    STACK_ALLOCATED();
public:
    StyleAttributeMutationScope(AbstractPropertySetCSSStyleDeclaration* decl)
    {
        ++s_scopeCount;

        if (s_scopeCount != 1) {
            ASSERT(s_currentDecl == decl);
            return;
        }

        ASSERT(!s_currentDecl);
        s_currentDecl = decl;

        if (!s_currentDecl->parentElement())
            return;

        bool shouldReadOldValue = false;

        m_mutationRecipients = MutationObserverInterestGroup::createForAttributesMutation(*s_currentDecl->parentElement(), HTMLNames::styleAttr);
        if (m_mutationRecipients && m_mutationRecipients->isOldValueRequested())
            shouldReadOldValue = true;

        AtomicString oldValue;
        if (shouldReadOldValue)
            oldValue = s_currentDecl->parentElement()->getAttribute(HTMLNames::styleAttr);

        if (m_mutationRecipients) {
            AtomicString requestedOldValue = m_mutationRecipients->isOldValueRequested() ? oldValue : nullAtom;
            m_mutation = MutationRecord::createAttributes(s_currentDecl->parentElement(), HTMLNames::styleAttr, requestedOldValue);
        }
    }

    ~StyleAttributeMutationScope()
    {
        --s_scopeCount;
        if (s_scopeCount)
            return;

        if (m_mutation && s_shouldDeliver)
            m_mutationRecipients->enqueueMutationRecord(m_mutation);

        s_shouldDeliver = false;

        // We have to clear internal state before calling Inspector's code.
        AbstractPropertySetCSSStyleDeclaration* localCopyStyleDecl = s_currentDecl;
        s_currentDecl = 0;

        if (!s_shouldNotifyInspector)
            return;

        s_shouldNotifyInspector = false;
        if (localCopyStyleDecl->parentElement())
            InspectorInstrumentation::didInvalidateStyleAttr(localCopyStyleDecl->parentElement());
    }

    void enqueueMutationRecord()
    {
        s_shouldDeliver = true;
    }

    void didInvalidateStyleAttr()
    {
        s_shouldNotifyInspector = true;
    }

private:
    static unsigned s_scopeCount;
    static AbstractPropertySetCSSStyleDeclaration* s_currentDecl;
    static bool s_shouldNotifyInspector;
    static bool s_shouldDeliver;

    OwnPtrWillBeMember<MutationObserverInterestGroup> m_mutationRecipients;
    RefPtrWillBeMember<MutationRecord> m_mutation;
};

unsigned StyleAttributeMutationScope::s_scopeCount = 0;
AbstractPropertySetCSSStyleDeclaration* StyleAttributeMutationScope::s_currentDecl = 0;
bool StyleAttributeMutationScope::s_shouldNotifyInspector = false;
bool StyleAttributeMutationScope::s_shouldDeliver = false;

} // namespace

DEFINE_TRACE(PropertySetCSSStyleDeclaration)
{
    visitor->trace(m_propertySet);
    AbstractPropertySetCSSStyleDeclaration::trace(visitor);
}

unsigned AbstractPropertySetCSSStyleDeclaration::length() const
{
    return propertySet().propertyCount();
}

String AbstractPropertySetCSSStyleDeclaration::item(unsigned i) const
{
    if (i >= propertySet().propertyCount())
        return "";
    StylePropertySet::PropertyReference property = propertySet().propertyAt(i);
    if (RuntimeEnabledFeatures::cssVariablesEnabled() && property.id() == CSSPropertyVariable)
        return toCSSCustomPropertyDeclaration(property.value())->name();
    return getPropertyName(property.id());
}

String AbstractPropertySetCSSStyleDeclaration::cssText() const
{
    return propertySet().asText();
}

void AbstractPropertySetCSSStyleDeclaration::setCSSText(const String& text, ExceptionState&)
{
    StyleAttributeMutationScope mutationScope(this);
    willMutate();

    propertySet().parseDeclarationList(text, contextStyleSheet());

    didMutate(PropertyChanged);

    mutationScope.enqueueMutationRecord();
}

String AbstractPropertySetCSSStyleDeclaration::getPropertyValue(const String& propertyName)
{
    CSSPropertyID propertyID = cssPropertyID(propertyName);
    if (!propertyID) {
        if (!RuntimeEnabledFeatures::cssVariablesEnabled() || !CSSVariableParser::isValidVariableName(propertyName))
            return String();
        return propertySet().getPropertyValue(AtomicString(propertyName));
    }
    return propertySet().getPropertyValue(propertyID);
}

String AbstractPropertySetCSSStyleDeclaration::getPropertyPriority(const String& propertyName)
{
    bool important = false;
    CSSPropertyID propertyID = cssPropertyID(propertyName);
    if (!propertyID) {
        if (!RuntimeEnabledFeatures::cssVariablesEnabled() || !CSSVariableParser::isValidVariableName(propertyName))
            return String();
        important = propertySet().propertyIsImportant(AtomicString(propertyName));
    } else {
        important = propertySet().propertyIsImportant(propertyID);
    }
    return important ? "important" : "";
}

String AbstractPropertySetCSSStyleDeclaration::getPropertyShorthand(const String& propertyName)
{
    CSSPropertyID propertyID = cssPropertyID(propertyName);

    // Custom properties don't have shorthands, so we can ignore them here.
    if (!propertyID)
        return String();
    if (isShorthandProperty(propertyID))
        return String();
    CSSPropertyID shorthandID = propertySet().getPropertyShorthand(propertyID);
    if (!shorthandID)
        return String();
    return getPropertyNameString(shorthandID);
}

bool AbstractPropertySetCSSStyleDeclaration::isPropertyImplicit(const String& propertyName)
{
    CSSPropertyID propertyID = cssPropertyID(propertyName);

    // Custom properties don't have shorthands, so we can ignore them here.
    if (!propertyID)
        return false;
    return propertySet().isPropertyImplicit(propertyID);
}

void AbstractPropertySetCSSStyleDeclaration::setProperty(const String& propertyName, const String& value, const String& priority, ExceptionState& exceptionState)
{
    CSSPropertyID propertyID = unresolvedCSSPropertyID(propertyName);
    if (!propertyID) {
        if (!RuntimeEnabledFeatures::cssVariablesEnabled() || !CSSVariableParser::isValidVariableName(propertyName))
            return;
        propertyID = CSSPropertyVariable;
    }

    bool important = equalIgnoringCase(priority, "important");
    if (!important && !priority.isEmpty())
        return;

    setPropertyInternal(propertyID, propertyName, value, important, exceptionState);
}

String AbstractPropertySetCSSStyleDeclaration::removeProperty(const String& propertyName, ExceptionState& exceptionState)
{
    CSSPropertyID propertyID = cssPropertyID(propertyName);
    if (!propertyID) {
        if (!RuntimeEnabledFeatures::cssVariablesEnabled() || !CSSVariableParser::isValidVariableName(propertyName))
            return String();
        propertyID = CSSPropertyVariable;
    }

    StyleAttributeMutationScope mutationScope(this);
    willMutate();

    String result;
    bool changed = false;
    if (propertyID == CSSPropertyVariable)
        changed = propertySet().removeProperty(AtomicString(propertyName), &result);
    else
        changed = propertySet().removeProperty(propertyID, &result);

    didMutate(changed ? PropertyChanged : NoChanges);

    if (changed)
        mutationScope.enqueueMutationRecord();
    return result;
}

GCRefPtr<CSSValue> AbstractPropertySetCSSStyleDeclaration::getPropertyCSSValueInternal(CSSPropertyID propertyID)
{
    return propertySet().getPropertyCSSValue(propertyID);
}

String AbstractPropertySetCSSStyleDeclaration::getPropertyValueInternal(CSSPropertyID propertyID)
{
    return propertySet().getPropertyValue(propertyID);
}

void AbstractPropertySetCSSStyleDeclaration::setPropertyInternal(CSSPropertyID unresolvedProperty, const String& customPropertyName, const String& value, bool important, ExceptionState&)
{
    StyleAttributeMutationScope mutationScope(this);
    willMutate();

    bool changed = false;
    if (unresolvedProperty == CSSPropertyVariable)
        changed = propertySet().setProperty(AtomicString(customPropertyName), value, important, contextStyleSheet());
    else
        changed = propertySet().setProperty(unresolvedProperty, value, important, contextStyleSheet());

    didMutate(changed ? PropertyChanged : NoChanges);

    if (!changed)
        return;

    Element* parent = parentElement();
    if (parent)
        parent->document().styleEngine().attributeChangedForElement(HTMLNames::styleAttr, *parent);
    mutationScope.enqueueMutationRecord();
}

StyleSheetContents* AbstractPropertySetCSSStyleDeclaration::contextStyleSheet() const
{
    CSSStyleSheet* cssStyleSheet = parentStyleSheet();
    return cssStyleSheet ? cssStyleSheet->contents() : nullptr;
}

bool AbstractPropertySetCSSStyleDeclaration::cssPropertyMatches(CSSPropertyID propertyID, const CSSValue* propertyValue) const
{
    return propertySet().propertyMatches(propertyID, propertyValue);
}

DEFINE_TRACE(AbstractPropertySetCSSStyleDeclaration)
{
    CSSStyleDeclaration::trace(visitor);
}

StyleRuleCSSStyleDeclaration::StyleRuleCSSStyleDeclaration(MutableStylePropertySet& propertySetArg, CSSRule* parentRule)
    : PropertySetCSSStyleDeclaration(propertySetArg)
    , m_parentRule(parentRule)
{
}

StyleRuleCSSStyleDeclaration::~StyleRuleCSSStyleDeclaration(void) = default;

void StyleRuleCSSStyleDeclaration::willMutate()
{
    if (m_parentRule && m_parentRule->parentStyleSheet())
        m_parentRule->parentStyleSheet()->willMutateRules();
}

void StyleRuleCSSStyleDeclaration::didMutate(MutationType type)
{
    // Style sheet mutation needs to be signaled even if the change failed. willMutateRules/didMutateRules must pair.
    if (m_parentRule && m_parentRule->parentStyleSheet())
        m_parentRule->parentStyleSheet()->didMutateRules();
}

CSSStyleSheet* StyleRuleCSSStyleDeclaration::parentStyleSheet() const
{
    return m_parentRule ? m_parentRule->parentStyleSheet() : nullptr;
}

void StyleRuleCSSStyleDeclaration::reattach(MutableStylePropertySet& propertySet)
{
    m_propertySet = &propertySet;
}

DEFINE_TRACE(StyleRuleCSSStyleDeclaration)
{
    visitor->trace(m_parentRule);
    PropertySetCSSStyleDeclaration::trace(visitor);
}

MutableStylePropertySet& InlineCSSStyleDeclaration::propertySet() const
{
    return m_parentElement->ensureMutableInlineStyle();
}

void InlineCSSStyleDeclaration::didMutate(MutationType type)
{
    if (type == NoChanges)
        return;

    if (!m_parentElement)
        return;

    m_parentElement->clearMutableInlineStyleIfEmpty();
    m_parentElement->setNeedsStyleRecalc(LocalStyleChange, StyleChangeReasonForTracing::create(StyleChangeReason::InlineCSSStyleMutated));
    m_parentElement->invalidateStyleAttribute();
    StyleAttributeMutationScope(this).didInvalidateStyleAttr();
}

CSSStyleSheet* InlineCSSStyleDeclaration::parentStyleSheet() const
{
    return m_parentElement ? &m_parentElement->document().elementSheet() : nullptr;
}

} // namespace blink
