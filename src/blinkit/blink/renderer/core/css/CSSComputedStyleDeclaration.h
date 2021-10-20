// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSComputedStyleDeclaration.h
// Description: CSSComputedStyleDeclaration Class
//      Author: Ziming Li
//     Created: 2021-10-10
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2004 Zack Rusin <zack@kde.org>
 * Copyright (C) 2004, 2005, 2006, 2008, 2012 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#ifndef CSSComputedStyleDeclaration_h
#define CSSComputedStyleDeclaration_h

#include "core/CoreExport.h"
#include "core/css/CSSStyleDeclaration.h"
#include "core/style/ComputedStyleConstants.h"
#include "wtf/HashMap.h"
#include "wtf/RefPtr.h"
#include "wtf/text/AtomicString.h"
#include "wtf/text/AtomicStringHash.h"
#include "wtf/text/WTFString.h"

namespace blink {

class CSSPrimitiveValue;
class CSSValueList;
class CSSVariableData;
class ExceptionState;
class MutableStylePropertySet;
class Node;
class LayoutObject;
class ComputedStyle;
class ShadowData;
class ShadowList;
class StyleColor;
class StylePropertyShorthand;

class CORE_EXPORT CSSComputedStyleDeclaration final : public CSSStyleDeclaration {
public:
    static GCRefPtr<CSSComputedStyleDeclaration> create(PassRefPtrWillBeRawPtr<Node> node, bool allowVisitedStyle = false, const String& pseudoElementName = String())
    {
        return BlinKit::GCWrapShared(new CSSComputedStyleDeclaration(node, allowVisitedStyle, pseudoElementName));
    }
    ~CSSComputedStyleDeclaration() override;

    String getPropertyValue(CSSPropertyID) const;
    bool getPropertyPriority(CSSPropertyID) const;

    GCRefPtr<MutableStylePropertySet> copyProperties() const;

    GCRefPtr<CSSValue> getPropertyCSSValue(CSSPropertyID) const;
    GCRefPtr<CSSValue> getPropertyCSSValue(AtomicString customPropertyName) const;
    const HashMap<AtomicString, RefPtr<CSSVariableData>>* getVariables() const;

    GCRefPtr<CSSValue> getFontSizeCSSValuePreferringKeyword() const;
    bool isMonospaceFont() const;

    GCRefPtr<MutableStylePropertySet> copyPropertiesInSet(const Vector<CSSPropertyID>&) const;

    DECLARE_VIRTUAL_TRACE();

private:
    CSSComputedStyleDeclaration(PassRefPtrWillBeRawPtr<Node>, bool allowVisitedStyle, const String&);

    // The styled node is either the node passed into getComputedStyle, or the
    // PseudoElement for :before and :after if they exist.
    // FIXME: This should be styledElement since in JS getComputedStyle only works
    // on Elements, but right now editing creates these for text nodes. We should fix
    // that.
    Node* styledNode() const;

    // CSSOM functions. Don't make these public.
    CSSRule* parentRule() const override;
    unsigned length() const override;
    String item(unsigned index) const override;
    const ComputedStyle* computeComputedStyle() const;
    String getPropertyValue(const String& propertyName) override;
    String getPropertyPriority(const String& propertyName) override;
    String getPropertyShorthand(const String& propertyName) override;
    bool isPropertyImplicit(const String& propertyName) override;
    void setProperty(const String& propertyName, const String& value, const String& priority, ExceptionState&) override;
    String removeProperty(const String& propertyName, ExceptionState&) override;
    String cssFloat() const;
    void setCSSFloat(const String&, ExceptionState&);
    String cssText() const override;
    void setCSSText(const String&, ExceptionState&) override;
    GCRefPtr<CSSValue> getPropertyCSSValueInternal(CSSPropertyID) override;
    String getPropertyValueInternal(CSSPropertyID) override;
    void setPropertyInternal(CSSPropertyID, const String& customPropertyName, const String& value, bool important, ExceptionState&) override;

    bool cssPropertyMatches(CSSPropertyID, const CSSValue*) const override;

    GCRefPtr<Node> m_node;
    PseudoId m_pseudoElementSpecifier;
    bool m_allowVisitedStyle;
};

} // namespace blink

#endif // CSSComputedStyleDeclaration_h
