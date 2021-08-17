// Copyright (c) 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef HTMLElementTypeHelpers_h
#define HTMLElementTypeHelpers_h

#include "core/dom/Element.h"
#include "HTMLNames.h"
#include "platform/RuntimeEnabledFeatures.h"

namespace blink {
// Type checking.
class HTMLAnchorElement;
void isHTMLAnchorElement(const HTMLAnchorElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLAnchorElement(const HTMLAnchorElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLAnchorElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::aTag);
}
inline bool isHTMLAnchorElement(const HTMLElement* element) { return element && isHTMLAnchorElement(*element); }
template<typename T> inline bool isHTMLAnchorElement(const PassRefPtr<T>& node) { return isHTMLAnchorElement(node.get()); }
template<typename T> inline bool isHTMLAnchorElement(const RefPtr<T>& node) { return isHTMLAnchorElement(node.get()); }
inline bool isHTMLAnchorElement(const Node& node) { return node.isHTMLElement() && isHTMLAnchorElement(toHTMLElement(node)); }
inline bool isHTMLAnchorElement(const Node* node) { return node && isHTMLAnchorElement(*node); }
template <> inline bool isElementOfType<const HTMLAnchorElement>(const Node& node) { return isHTMLAnchorElement(node); }
template <> inline bool isElementOfType<const HTMLAnchorElement>(const HTMLElement& element) { return isHTMLAnchorElement(element); }

class HTMLAreaElement;
void isHTMLAreaElement(const HTMLAreaElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLAreaElement(const HTMLAreaElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLAreaElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::areaTag);
}
inline bool isHTMLAreaElement(const HTMLElement* element) { return element && isHTMLAreaElement(*element); }
template<typename T> inline bool isHTMLAreaElement(const PassRefPtr<T>& node) { return isHTMLAreaElement(node.get()); }
template<typename T> inline bool isHTMLAreaElement(const RefPtr<T>& node) { return isHTMLAreaElement(node.get()); }
inline bool isHTMLAreaElement(const Node& node) { return node.isHTMLElement() && isHTMLAreaElement(toHTMLElement(node)); }
inline bool isHTMLAreaElement(const Node* node) { return node && isHTMLAreaElement(*node); }
template <> inline bool isElementOfType<const HTMLAreaElement>(const Node& node) { return isHTMLAreaElement(node); }
template <> inline bool isElementOfType<const HTMLAreaElement>(const HTMLElement& element) { return isHTMLAreaElement(element); }

class HTMLBRElement;
void isHTMLBRElement(const HTMLBRElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLBRElement(const HTMLBRElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLBRElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::brTag);
}
inline bool isHTMLBRElement(const HTMLElement* element) { return element && isHTMLBRElement(*element); }
template<typename T> inline bool isHTMLBRElement(const PassRefPtr<T>& node) { return isHTMLBRElement(node.get()); }
template<typename T> inline bool isHTMLBRElement(const RefPtr<T>& node) { return isHTMLBRElement(node.get()); }
inline bool isHTMLBRElement(const Node& node) { return node.isHTMLElement() && isHTMLBRElement(toHTMLElement(node)); }
inline bool isHTMLBRElement(const Node* node) { return node && isHTMLBRElement(*node); }
template <> inline bool isElementOfType<const HTMLBRElement>(const Node& node) { return isHTMLBRElement(node); }
template <> inline bool isElementOfType<const HTMLBRElement>(const HTMLElement& element) { return isHTMLBRElement(element); }

class HTMLBaseElement;
void isHTMLBaseElement(const HTMLBaseElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLBaseElement(const HTMLBaseElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLBaseElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::baseTag);
}
inline bool isHTMLBaseElement(const HTMLElement* element) { return element && isHTMLBaseElement(*element); }
template<typename T> inline bool isHTMLBaseElement(const PassRefPtr<T>& node) { return isHTMLBaseElement(node.get()); }
template<typename T> inline bool isHTMLBaseElement(const RefPtr<T>& node) { return isHTMLBaseElement(node.get()); }
inline bool isHTMLBaseElement(const Node& node) { return node.isHTMLElement() && isHTMLBaseElement(toHTMLElement(node)); }
inline bool isHTMLBaseElement(const Node* node) { return node && isHTMLBaseElement(*node); }
template <> inline bool isElementOfType<const HTMLBaseElement>(const Node& node) { return isHTMLBaseElement(node); }
template <> inline bool isElementOfType<const HTMLBaseElement>(const HTMLElement& element) { return isHTMLBaseElement(element); }

class HTMLBodyElement;
void isHTMLBodyElement(const HTMLBodyElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLBodyElement(const HTMLBodyElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLBodyElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::bodyTag);
}
inline bool isHTMLBodyElement(const HTMLElement* element) { return element && isHTMLBodyElement(*element); }
template<typename T> inline bool isHTMLBodyElement(const PassRefPtr<T>& node) { return isHTMLBodyElement(node.get()); }
template<typename T> inline bool isHTMLBodyElement(const RefPtr<T>& node) { return isHTMLBodyElement(node.get()); }
inline bool isHTMLBodyElement(const Node& node) { return node.isHTMLElement() && isHTMLBodyElement(toHTMLElement(node)); }
inline bool isHTMLBodyElement(const Node* node) { return node && isHTMLBodyElement(*node); }
template <> inline bool isElementOfType<const HTMLBodyElement>(const Node& node) { return isHTMLBodyElement(node); }
template <> inline bool isElementOfType<const HTMLBodyElement>(const HTMLElement& element) { return isHTMLBodyElement(element); }

class HTMLContentElement;
void isHTMLContentElement(const HTMLContentElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLContentElement(const HTMLContentElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLContentElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::contentTag);
}
inline bool isHTMLContentElement(const HTMLElement* element) { return element && isHTMLContentElement(*element); }
template<typename T> inline bool isHTMLContentElement(const PassRefPtr<T>& node) { return isHTMLContentElement(node.get()); }
template<typename T> inline bool isHTMLContentElement(const RefPtr<T>& node) { return isHTMLContentElement(node.get()); }
inline bool isHTMLContentElement(const Node& node) { return node.isHTMLElement() && isHTMLContentElement(toHTMLElement(node)); }
inline bool isHTMLContentElement(const Node* node) { return node && isHTMLContentElement(*node); }
template <> inline bool isElementOfType<const HTMLContentElement>(const Node& node) { return isHTMLContentElement(node); }
template <> inline bool isElementOfType<const HTMLContentElement>(const HTMLElement& element) { return isHTMLContentElement(element); }

class HTMLDListElement;
void isHTMLDListElement(const HTMLDListElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLDListElement(const HTMLDListElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLDListElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::dlTag);
}
inline bool isHTMLDListElement(const HTMLElement* element) { return element && isHTMLDListElement(*element); }
template<typename T> inline bool isHTMLDListElement(const PassRefPtr<T>& node) { return isHTMLDListElement(node.get()); }
template<typename T> inline bool isHTMLDListElement(const RefPtr<T>& node) { return isHTMLDListElement(node.get()); }
inline bool isHTMLDListElement(const Node& node) { return node.isHTMLElement() && isHTMLDListElement(toHTMLElement(node)); }
inline bool isHTMLDListElement(const Node* node) { return node && isHTMLDListElement(*node); }
template <> inline bool isElementOfType<const HTMLDListElement>(const Node& node) { return isHTMLDListElement(node); }
template <> inline bool isElementOfType<const HTMLDListElement>(const HTMLElement& element) { return isHTMLDListElement(element); }

class HTMLDataListElement;
void isHTMLDataListElement(const HTMLDataListElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLDataListElement(const HTMLDataListElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLDataListElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::datalistTag);
}
inline bool isHTMLDataListElement(const HTMLElement* element) { return element && isHTMLDataListElement(*element); }
template<typename T> inline bool isHTMLDataListElement(const PassRefPtr<T>& node) { return isHTMLDataListElement(node.get()); }
template<typename T> inline bool isHTMLDataListElement(const RefPtr<T>& node) { return isHTMLDataListElement(node.get()); }
inline bool isHTMLDataListElement(const Node& node) { return node.isHTMLElement() && isHTMLDataListElement(toHTMLElement(node)); }
inline bool isHTMLDataListElement(const Node* node) { return node && isHTMLDataListElement(*node); }
template <> inline bool isElementOfType<const HTMLDataListElement>(const Node& node) { return isHTMLDataListElement(node); }
template <> inline bool isElementOfType<const HTMLDataListElement>(const HTMLElement& element) { return isHTMLDataListElement(element); }

class HTMLDetailsElement;
void isHTMLDetailsElement(const HTMLDetailsElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLDetailsElement(const HTMLDetailsElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLDetailsElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::detailsTag);
}
inline bool isHTMLDetailsElement(const HTMLElement* element) { return element && isHTMLDetailsElement(*element); }
template<typename T> inline bool isHTMLDetailsElement(const PassRefPtr<T>& node) { return isHTMLDetailsElement(node.get()); }
template<typename T> inline bool isHTMLDetailsElement(const RefPtr<T>& node) { return isHTMLDetailsElement(node.get()); }
inline bool isHTMLDetailsElement(const Node& node) { return node.isHTMLElement() && isHTMLDetailsElement(toHTMLElement(node)); }
inline bool isHTMLDetailsElement(const Node* node) { return node && isHTMLDetailsElement(*node); }
template <> inline bool isElementOfType<const HTMLDetailsElement>(const Node& node) { return isHTMLDetailsElement(node); }
template <> inline bool isElementOfType<const HTMLDetailsElement>(const HTMLElement& element) { return isHTMLDetailsElement(element); }

class HTMLDirectoryElement;
void isHTMLDirectoryElement(const HTMLDirectoryElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLDirectoryElement(const HTMLDirectoryElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLDirectoryElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::dirTag);
}
inline bool isHTMLDirectoryElement(const HTMLElement* element) { return element && isHTMLDirectoryElement(*element); }
template<typename T> inline bool isHTMLDirectoryElement(const PassRefPtr<T>& node) { return isHTMLDirectoryElement(node.get()); }
template<typename T> inline bool isHTMLDirectoryElement(const RefPtr<T>& node) { return isHTMLDirectoryElement(node.get()); }
inline bool isHTMLDirectoryElement(const Node& node) { return node.isHTMLElement() && isHTMLDirectoryElement(toHTMLElement(node)); }
inline bool isHTMLDirectoryElement(const Node* node) { return node && isHTMLDirectoryElement(*node); }
template <> inline bool isElementOfType<const HTMLDirectoryElement>(const Node& node) { return isHTMLDirectoryElement(node); }
template <> inline bool isElementOfType<const HTMLDirectoryElement>(const HTMLElement& element) { return isHTMLDirectoryElement(element); }

class HTMLDivElement;
void isHTMLDivElement(const HTMLDivElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLDivElement(const HTMLDivElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLDivElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::divTag);
}
inline bool isHTMLDivElement(const HTMLElement* element) { return element && isHTMLDivElement(*element); }
template<typename T> inline bool isHTMLDivElement(const PassRefPtr<T>& node) { return isHTMLDivElement(node.get()); }
template<typename T> inline bool isHTMLDivElement(const RefPtr<T>& node) { return isHTMLDivElement(node.get()); }
inline bool isHTMLDivElement(const Node& node) { return node.isHTMLElement() && isHTMLDivElement(toHTMLElement(node)); }
inline bool isHTMLDivElement(const Node* node) { return node && isHTMLDivElement(*node); }
template <> inline bool isElementOfType<const HTMLDivElement>(const Node& node) { return isHTMLDivElement(node); }
template <> inline bool isElementOfType<const HTMLDivElement>(const HTMLElement& element) { return isHTMLDivElement(element); }

class HTMLFontElement;
void isHTMLFontElement(const HTMLFontElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLFontElement(const HTMLFontElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLFontElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::fontTag);
}
inline bool isHTMLFontElement(const HTMLElement* element) { return element && isHTMLFontElement(*element); }
template<typename T> inline bool isHTMLFontElement(const PassRefPtr<T>& node) { return isHTMLFontElement(node.get()); }
template<typename T> inline bool isHTMLFontElement(const RefPtr<T>& node) { return isHTMLFontElement(node.get()); }
inline bool isHTMLFontElement(const Node& node) { return node.isHTMLElement() && isHTMLFontElement(toHTMLElement(node)); }
inline bool isHTMLFontElement(const Node* node) { return node && isHTMLFontElement(*node); }
template <> inline bool isElementOfType<const HTMLFontElement>(const Node& node) { return isHTMLFontElement(node); }
template <> inline bool isElementOfType<const HTMLFontElement>(const HTMLElement& element) { return isHTMLFontElement(element); }

class HTMLFormElement;
void isHTMLFormElement(const HTMLFormElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLFormElement(const HTMLFormElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLFormElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::formTag);
}
inline bool isHTMLFormElement(const HTMLElement* element) { return element && isHTMLFormElement(*element); }
template<typename T> inline bool isHTMLFormElement(const PassRefPtr<T>& node) { return isHTMLFormElement(node.get()); }
template<typename T> inline bool isHTMLFormElement(const RefPtr<T>& node) { return isHTMLFormElement(node.get()); }
inline bool isHTMLFormElement(const Node& node) { return node.isHTMLElement() && isHTMLFormElement(toHTMLElement(node)); }
inline bool isHTMLFormElement(const Node* node) { return node && isHTMLFormElement(*node); }
template <> inline bool isElementOfType<const HTMLFormElement>(const Node& node) { return isHTMLFormElement(node); }
template <> inline bool isElementOfType<const HTMLFormElement>(const HTMLElement& element) { return isHTMLFormElement(element); }

class HTMLHRElement;
void isHTMLHRElement(const HTMLHRElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLHRElement(const HTMLHRElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLHRElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::hrTag);
}
inline bool isHTMLHRElement(const HTMLElement* element) { return element && isHTMLHRElement(*element); }
template<typename T> inline bool isHTMLHRElement(const PassRefPtr<T>& node) { return isHTMLHRElement(node.get()); }
template<typename T> inline bool isHTMLHRElement(const RefPtr<T>& node) { return isHTMLHRElement(node.get()); }
inline bool isHTMLHRElement(const Node& node) { return node.isHTMLElement() && isHTMLHRElement(toHTMLElement(node)); }
inline bool isHTMLHRElement(const Node* node) { return node && isHTMLHRElement(*node); }
template <> inline bool isElementOfType<const HTMLHRElement>(const Node& node) { return isHTMLHRElement(node); }
template <> inline bool isElementOfType<const HTMLHRElement>(const HTMLElement& element) { return isHTMLHRElement(element); }

class HTMLHeadElement;
void isHTMLHeadElement(const HTMLHeadElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLHeadElement(const HTMLHeadElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLHeadElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::headTag);
}
inline bool isHTMLHeadElement(const HTMLElement* element) { return element && isHTMLHeadElement(*element); }
template<typename T> inline bool isHTMLHeadElement(const PassRefPtr<T>& node) { return isHTMLHeadElement(node.get()); }
template<typename T> inline bool isHTMLHeadElement(const RefPtr<T>& node) { return isHTMLHeadElement(node.get()); }
inline bool isHTMLHeadElement(const Node& node) { return node.isHTMLElement() && isHTMLHeadElement(toHTMLElement(node)); }
inline bool isHTMLHeadElement(const Node* node) { return node && isHTMLHeadElement(*node); }
template <> inline bool isElementOfType<const HTMLHeadElement>(const Node& node) { return isHTMLHeadElement(node); }
template <> inline bool isElementOfType<const HTMLHeadElement>(const HTMLElement& element) { return isHTMLHeadElement(element); }

class HTMLHtmlElement;
void isHTMLHtmlElement(const HTMLHtmlElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLHtmlElement(const HTMLHtmlElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLHtmlElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::htmlTag);
}
inline bool isHTMLHtmlElement(const HTMLElement* element) { return element && isHTMLHtmlElement(*element); }
template<typename T> inline bool isHTMLHtmlElement(const PassRefPtr<T>& node) { return isHTMLHtmlElement(node.get()); }
template<typename T> inline bool isHTMLHtmlElement(const RefPtr<T>& node) { return isHTMLHtmlElement(node.get()); }
inline bool isHTMLHtmlElement(const Node& node) { return node.isHTMLElement() && isHTMLHtmlElement(toHTMLElement(node)); }
inline bool isHTMLHtmlElement(const Node* node) { return node && isHTMLHtmlElement(*node); }
template <> inline bool isElementOfType<const HTMLHtmlElement>(const Node& node) { return isHTMLHtmlElement(node); }
template <> inline bool isElementOfType<const HTMLHtmlElement>(const HTMLElement& element) { return isHTMLHtmlElement(element); }

class HTMLLIElement;
void isHTMLLIElement(const HTMLLIElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLLIElement(const HTMLLIElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLLIElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::liTag);
}
inline bool isHTMLLIElement(const HTMLElement* element) { return element && isHTMLLIElement(*element); }
template<typename T> inline bool isHTMLLIElement(const PassRefPtr<T>& node) { return isHTMLLIElement(node.get()); }
template<typename T> inline bool isHTMLLIElement(const RefPtr<T>& node) { return isHTMLLIElement(node.get()); }
inline bool isHTMLLIElement(const Node& node) { return node.isHTMLElement() && isHTMLLIElement(toHTMLElement(node)); }
inline bool isHTMLLIElement(const Node* node) { return node && isHTMLLIElement(*node); }
template <> inline bool isElementOfType<const HTMLLIElement>(const Node& node) { return isHTMLLIElement(node); }
template <> inline bool isElementOfType<const HTMLLIElement>(const HTMLElement& element) { return isHTMLLIElement(element); }

class HTMLLegendElement;
void isHTMLLegendElement(const HTMLLegendElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLLegendElement(const HTMLLegendElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLLegendElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::legendTag);
}
inline bool isHTMLLegendElement(const HTMLElement* element) { return element && isHTMLLegendElement(*element); }
template<typename T> inline bool isHTMLLegendElement(const PassRefPtr<T>& node) { return isHTMLLegendElement(node.get()); }
template<typename T> inline bool isHTMLLegendElement(const RefPtr<T>& node) { return isHTMLLegendElement(node.get()); }
inline bool isHTMLLegendElement(const Node& node) { return node.isHTMLElement() && isHTMLLegendElement(toHTMLElement(node)); }
inline bool isHTMLLegendElement(const Node* node) { return node && isHTMLLegendElement(*node); }
template <> inline bool isElementOfType<const HTMLLegendElement>(const Node& node) { return isHTMLLegendElement(node); }
template <> inline bool isElementOfType<const HTMLLegendElement>(const HTMLElement& element) { return isHTMLLegendElement(element); }

class HTMLMarqueeElement;
void isHTMLMarqueeElement(const HTMLMarqueeElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLMarqueeElement(const HTMLMarqueeElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLMarqueeElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::marqueeTag);
}
inline bool isHTMLMarqueeElement(const HTMLElement* element) { return element && isHTMLMarqueeElement(*element); }
template<typename T> inline bool isHTMLMarqueeElement(const PassRefPtr<T>& node) { return isHTMLMarqueeElement(node.get()); }
template<typename T> inline bool isHTMLMarqueeElement(const RefPtr<T>& node) { return isHTMLMarqueeElement(node.get()); }
inline bool isHTMLMarqueeElement(const Node& node) { return node.isHTMLElement() && isHTMLMarqueeElement(toHTMLElement(node)); }
inline bool isHTMLMarqueeElement(const Node* node) { return node && isHTMLMarqueeElement(*node); }
template <> inline bool isElementOfType<const HTMLMarqueeElement>(const Node& node) { return isHTMLMarqueeElement(node); }
template <> inline bool isElementOfType<const HTMLMarqueeElement>(const HTMLElement& element) { return isHTMLMarqueeElement(element); }

class HTMLMenuElement;
void isHTMLMenuElement(const HTMLMenuElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLMenuElement(const HTMLMenuElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLMenuElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::menuTag);
}
inline bool isHTMLMenuElement(const HTMLElement* element) { return element && isHTMLMenuElement(*element); }
template<typename T> inline bool isHTMLMenuElement(const PassRefPtr<T>& node) { return isHTMLMenuElement(node.get()); }
template<typename T> inline bool isHTMLMenuElement(const RefPtr<T>& node) { return isHTMLMenuElement(node.get()); }
inline bool isHTMLMenuElement(const Node& node) { return node.isHTMLElement() && isHTMLMenuElement(toHTMLElement(node)); }
inline bool isHTMLMenuElement(const Node* node) { return node && isHTMLMenuElement(*node); }
template <> inline bool isElementOfType<const HTMLMenuElement>(const Node& node) { return isHTMLMenuElement(node); }
template <> inline bool isElementOfType<const HTMLMenuElement>(const HTMLElement& element) { return isHTMLMenuElement(element); }

class HTMLMenuItemElement;
void isHTMLMenuItemElement(const HTMLMenuItemElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLMenuItemElement(const HTMLMenuItemElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLMenuItemElement(const HTMLElement& element) {
    if (!RuntimeEnabledFeatures::contextMenuEnabled())
        return false;
    return element.hasTagName(HTMLNames::menuitemTag);
}
inline bool isHTMLMenuItemElement(const HTMLElement* element) { return element && isHTMLMenuItemElement(*element); }
template<typename T> inline bool isHTMLMenuItemElement(const PassRefPtr<T>& node) { return isHTMLMenuItemElement(node.get()); }
template<typename T> inline bool isHTMLMenuItemElement(const RefPtr<T>& node) { return isHTMLMenuItemElement(node.get()); }
inline bool isHTMLMenuItemElement(const Node& node) { return node.isHTMLElement() && isHTMLMenuItemElement(toHTMLElement(node)); }
inline bool isHTMLMenuItemElement(const Node* node) { return node && isHTMLMenuItemElement(*node); }
template <> inline bool isElementOfType<const HTMLMenuItemElement>(const Node& node) { return isHTMLMenuItemElement(node); }
template <> inline bool isElementOfType<const HTMLMenuItemElement>(const HTMLElement& element) { return isHTMLMenuItemElement(element); }

class HTMLMetaElement;
void isHTMLMetaElement(const HTMLMetaElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLMetaElement(const HTMLMetaElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLMetaElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::metaTag);
}
inline bool isHTMLMetaElement(const HTMLElement* element) { return element && isHTMLMetaElement(*element); }
template<typename T> inline bool isHTMLMetaElement(const PassRefPtr<T>& node) { return isHTMLMetaElement(node.get()); }
template<typename T> inline bool isHTMLMetaElement(const RefPtr<T>& node) { return isHTMLMetaElement(node.get()); }
inline bool isHTMLMetaElement(const Node& node) { return node.isHTMLElement() && isHTMLMetaElement(toHTMLElement(node)); }
inline bool isHTMLMetaElement(const Node* node) { return node && isHTMLMetaElement(*node); }
template <> inline bool isElementOfType<const HTMLMetaElement>(const Node& node) { return isHTMLMetaElement(node); }
template <> inline bool isElementOfType<const HTMLMetaElement>(const HTMLElement& element) { return isHTMLMetaElement(element); }

class HTMLMeterElement;
void isHTMLMeterElement(const HTMLMeterElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLMeterElement(const HTMLMeterElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLMeterElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::meterTag);
}
inline bool isHTMLMeterElement(const HTMLElement* element) { return element && isHTMLMeterElement(*element); }
template<typename T> inline bool isHTMLMeterElement(const PassRefPtr<T>& node) { return isHTMLMeterElement(node.get()); }
template<typename T> inline bool isHTMLMeterElement(const RefPtr<T>& node) { return isHTMLMeterElement(node.get()); }
inline bool isHTMLMeterElement(const Node& node) { return node.isHTMLElement() && isHTMLMeterElement(toHTMLElement(node)); }
inline bool isHTMLMeterElement(const Node* node) { return node && isHTMLMeterElement(*node); }
template <> inline bool isElementOfType<const HTMLMeterElement>(const Node& node) { return isHTMLMeterElement(node); }
template <> inline bool isElementOfType<const HTMLMeterElement>(const HTMLElement& element) { return isHTMLMeterElement(element); }

class HTMLOListElement;
void isHTMLOListElement(const HTMLOListElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLOListElement(const HTMLOListElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLOListElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::olTag);
}
inline bool isHTMLOListElement(const HTMLElement* element) { return element && isHTMLOListElement(*element); }
template<typename T> inline bool isHTMLOListElement(const PassRefPtr<T>& node) { return isHTMLOListElement(node.get()); }
template<typename T> inline bool isHTMLOListElement(const RefPtr<T>& node) { return isHTMLOListElement(node.get()); }
inline bool isHTMLOListElement(const Node& node) { return node.isHTMLElement() && isHTMLOListElement(toHTMLElement(node)); }
inline bool isHTMLOListElement(const Node* node) { return node && isHTMLOListElement(*node); }
template <> inline bool isElementOfType<const HTMLOListElement>(const Node& node) { return isHTMLOListElement(node); }
template <> inline bool isElementOfType<const HTMLOListElement>(const HTMLElement& element) { return isHTMLOListElement(element); }

class HTMLOptGroupElement;
void isHTMLOptGroupElement(const HTMLOptGroupElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLOptGroupElement(const HTMLOptGroupElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLOptGroupElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::optgroupTag);
}
inline bool isHTMLOptGroupElement(const HTMLElement* element) { return element && isHTMLOptGroupElement(*element); }
template<typename T> inline bool isHTMLOptGroupElement(const PassRefPtr<T>& node) { return isHTMLOptGroupElement(node.get()); }
template<typename T> inline bool isHTMLOptGroupElement(const RefPtr<T>& node) { return isHTMLOptGroupElement(node.get()); }
inline bool isHTMLOptGroupElement(const Node& node) { return node.isHTMLElement() && isHTMLOptGroupElement(toHTMLElement(node)); }
inline bool isHTMLOptGroupElement(const Node* node) { return node && isHTMLOptGroupElement(*node); }
template <> inline bool isElementOfType<const HTMLOptGroupElement>(const Node& node) { return isHTMLOptGroupElement(node); }
template <> inline bool isElementOfType<const HTMLOptGroupElement>(const HTMLElement& element) { return isHTMLOptGroupElement(element); }

class HTMLOptionElement;
void isHTMLOptionElement(const HTMLOptionElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLOptionElement(const HTMLOptionElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLOptionElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::optionTag);
}
inline bool isHTMLOptionElement(const HTMLElement* element) { return element && isHTMLOptionElement(*element); }
template<typename T> inline bool isHTMLOptionElement(const PassRefPtr<T>& node) { return isHTMLOptionElement(node.get()); }
template<typename T> inline bool isHTMLOptionElement(const RefPtr<T>& node) { return isHTMLOptionElement(node.get()); }
inline bool isHTMLOptionElement(const Node& node) { return node.isHTMLElement() && isHTMLOptionElement(toHTMLElement(node)); }
inline bool isHTMLOptionElement(const Node* node) { return node && isHTMLOptionElement(*node); }
template <> inline bool isElementOfType<const HTMLOptionElement>(const Node& node) { return isHTMLOptionElement(node); }
template <> inline bool isElementOfType<const HTMLOptionElement>(const HTMLElement& element) { return isHTMLOptionElement(element); }

class HTMLParagraphElement;
void isHTMLParagraphElement(const HTMLParagraphElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLParagraphElement(const HTMLParagraphElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLParagraphElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::pTag);
}
inline bool isHTMLParagraphElement(const HTMLElement* element) { return element && isHTMLParagraphElement(*element); }
template<typename T> inline bool isHTMLParagraphElement(const PassRefPtr<T>& node) { return isHTMLParagraphElement(node.get()); }
template<typename T> inline bool isHTMLParagraphElement(const RefPtr<T>& node) { return isHTMLParagraphElement(node.get()); }
inline bool isHTMLParagraphElement(const Node& node) { return node.isHTMLElement() && isHTMLParagraphElement(toHTMLElement(node)); }
inline bool isHTMLParagraphElement(const Node* node) { return node && isHTMLParagraphElement(*node); }
template <> inline bool isElementOfType<const HTMLParagraphElement>(const Node& node) { return isHTMLParagraphElement(node); }
template <> inline bool isElementOfType<const HTMLParagraphElement>(const HTMLElement& element) { return isHTMLParagraphElement(element); }

class HTMLPictureElement;
void isHTMLPictureElement(const HTMLPictureElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLPictureElement(const HTMLPictureElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLPictureElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::pictureTag);
}
inline bool isHTMLPictureElement(const HTMLElement* element) { return element && isHTMLPictureElement(*element); }
template<typename T> inline bool isHTMLPictureElement(const PassRefPtr<T>& node) { return isHTMLPictureElement(node.get()); }
template<typename T> inline bool isHTMLPictureElement(const RefPtr<T>& node) { return isHTMLPictureElement(node.get()); }
inline bool isHTMLPictureElement(const Node& node) { return node.isHTMLElement() && isHTMLPictureElement(toHTMLElement(node)); }
inline bool isHTMLPictureElement(const Node* node) { return node && isHTMLPictureElement(*node); }
template <> inline bool isElementOfType<const HTMLPictureElement>(const Node& node) { return isHTMLPictureElement(node); }
template <> inline bool isElementOfType<const HTMLPictureElement>(const HTMLElement& element) { return isHTMLPictureElement(element); }

class HTMLProgressElement;
void isHTMLProgressElement(const HTMLProgressElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLProgressElement(const HTMLProgressElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLProgressElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::progressTag);
}
inline bool isHTMLProgressElement(const HTMLElement* element) { return element && isHTMLProgressElement(*element); }
template<typename T> inline bool isHTMLProgressElement(const PassRefPtr<T>& node) { return isHTMLProgressElement(node.get()); }
template<typename T> inline bool isHTMLProgressElement(const RefPtr<T>& node) { return isHTMLProgressElement(node.get()); }
inline bool isHTMLProgressElement(const Node& node) { return node.isHTMLElement() && isHTMLProgressElement(toHTMLElement(node)); }
inline bool isHTMLProgressElement(const Node* node) { return node && isHTMLProgressElement(*node); }
template <> inline bool isElementOfType<const HTMLProgressElement>(const Node& node) { return isHTMLProgressElement(node); }
template <> inline bool isElementOfType<const HTMLProgressElement>(const HTMLElement& element) { return isHTMLProgressElement(element); }

class HTMLShadowElement;
void isHTMLShadowElement(const HTMLShadowElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLShadowElement(const HTMLShadowElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLShadowElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::shadowTag);
}
inline bool isHTMLShadowElement(const HTMLElement* element) { return element && isHTMLShadowElement(*element); }
template<typename T> inline bool isHTMLShadowElement(const PassRefPtr<T>& node) { return isHTMLShadowElement(node.get()); }
template<typename T> inline bool isHTMLShadowElement(const RefPtr<T>& node) { return isHTMLShadowElement(node.get()); }
inline bool isHTMLShadowElement(const Node& node) { return node.isHTMLElement() && isHTMLShadowElement(toHTMLElement(node)); }
inline bool isHTMLShadowElement(const Node* node) { return node && isHTMLShadowElement(*node); }
template <> inline bool isElementOfType<const HTMLShadowElement>(const Node& node) { return isHTMLShadowElement(node); }
template <> inline bool isElementOfType<const HTMLShadowElement>(const HTMLElement& element) { return isHTMLShadowElement(element); }

class HTMLSlotElement;
void isHTMLSlotElement(const HTMLSlotElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLSlotElement(const HTMLSlotElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLSlotElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::slotTag);
}
inline bool isHTMLSlotElement(const HTMLElement* element) { return element && isHTMLSlotElement(*element); }
template<typename T> inline bool isHTMLSlotElement(const PassRefPtr<T>& node) { return isHTMLSlotElement(node.get()); }
template<typename T> inline bool isHTMLSlotElement(const RefPtr<T>& node) { return isHTMLSlotElement(node.get()); }
inline bool isHTMLSlotElement(const Node& node) { return node.isHTMLElement() && isHTMLSlotElement(toHTMLElement(node)); }
inline bool isHTMLSlotElement(const Node* node) { return node && isHTMLSlotElement(*node); }
template <> inline bool isElementOfType<const HTMLSlotElement>(const Node& node) { return isHTMLSlotElement(node); }
template <> inline bool isElementOfType<const HTMLSlotElement>(const HTMLElement& element) { return isHTMLSlotElement(element); }

class HTMLSpanElement;
void isHTMLSpanElement(const HTMLSpanElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLSpanElement(const HTMLSpanElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLSpanElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::spanTag);
}
inline bool isHTMLSpanElement(const HTMLElement* element) { return element && isHTMLSpanElement(*element); }
template<typename T> inline bool isHTMLSpanElement(const PassRefPtr<T>& node) { return isHTMLSpanElement(node.get()); }
template<typename T> inline bool isHTMLSpanElement(const RefPtr<T>& node) { return isHTMLSpanElement(node.get()); }
inline bool isHTMLSpanElement(const Node& node) { return node.isHTMLElement() && isHTMLSpanElement(toHTMLElement(node)); }
inline bool isHTMLSpanElement(const Node* node) { return node && isHTMLSpanElement(*node); }
template <> inline bool isElementOfType<const HTMLSpanElement>(const Node& node) { return isHTMLSpanElement(node); }
template <> inline bool isElementOfType<const HTMLSpanElement>(const HTMLElement& element) { return isHTMLSpanElement(element); }

class HTMLTableCaptionElement;
void isHTMLTableCaptionElement(const HTMLTableCaptionElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLTableCaptionElement(const HTMLTableCaptionElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLTableCaptionElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::captionTag);
}
inline bool isHTMLTableCaptionElement(const HTMLElement* element) { return element && isHTMLTableCaptionElement(*element); }
template<typename T> inline bool isHTMLTableCaptionElement(const PassRefPtr<T>& node) { return isHTMLTableCaptionElement(node.get()); }
template<typename T> inline bool isHTMLTableCaptionElement(const RefPtr<T>& node) { return isHTMLTableCaptionElement(node.get()); }
inline bool isHTMLTableCaptionElement(const Node& node) { return node.isHTMLElement() && isHTMLTableCaptionElement(toHTMLElement(node)); }
inline bool isHTMLTableCaptionElement(const Node* node) { return node && isHTMLTableCaptionElement(*node); }
template <> inline bool isElementOfType<const HTMLTableCaptionElement>(const Node& node) { return isHTMLTableCaptionElement(node); }
template <> inline bool isElementOfType<const HTMLTableCaptionElement>(const HTMLElement& element) { return isHTMLTableCaptionElement(element); }

class HTMLTableElement;
void isHTMLTableElement(const HTMLTableElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLTableElement(const HTMLTableElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLTableElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::tableTag);
}
inline bool isHTMLTableElement(const HTMLElement* element) { return element && isHTMLTableElement(*element); }
template<typename T> inline bool isHTMLTableElement(const PassRefPtr<T>& node) { return isHTMLTableElement(node.get()); }
template<typename T> inline bool isHTMLTableElement(const RefPtr<T>& node) { return isHTMLTableElement(node.get()); }
inline bool isHTMLTableElement(const Node& node) { return node.isHTMLElement() && isHTMLTableElement(toHTMLElement(node)); }
inline bool isHTMLTableElement(const Node* node) { return node && isHTMLTableElement(*node); }
template <> inline bool isElementOfType<const HTMLTableElement>(const Node& node) { return isHTMLTableElement(node); }
template <> inline bool isElementOfType<const HTMLTableElement>(const HTMLElement& element) { return isHTMLTableElement(element); }

class HTMLTableRowElement;
void isHTMLTableRowElement(const HTMLTableRowElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLTableRowElement(const HTMLTableRowElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLTableRowElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::trTag);
}
inline bool isHTMLTableRowElement(const HTMLElement* element) { return element && isHTMLTableRowElement(*element); }
template<typename T> inline bool isHTMLTableRowElement(const PassRefPtr<T>& node) { return isHTMLTableRowElement(node.get()); }
template<typename T> inline bool isHTMLTableRowElement(const RefPtr<T>& node) { return isHTMLTableRowElement(node.get()); }
inline bool isHTMLTableRowElement(const Node& node) { return node.isHTMLElement() && isHTMLTableRowElement(toHTMLElement(node)); }
inline bool isHTMLTableRowElement(const Node* node) { return node && isHTMLTableRowElement(*node); }
template <> inline bool isElementOfType<const HTMLTableRowElement>(const Node& node) { return isHTMLTableRowElement(node); }
template <> inline bool isElementOfType<const HTMLTableRowElement>(const HTMLElement& element) { return isHTMLTableRowElement(element); }

class HTMLTemplateElement;
void isHTMLTemplateElement(const HTMLTemplateElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLTemplateElement(const HTMLTemplateElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLTemplateElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::templateTag);
}
inline bool isHTMLTemplateElement(const HTMLElement* element) { return element && isHTMLTemplateElement(*element); }
template<typename T> inline bool isHTMLTemplateElement(const PassRefPtr<T>& node) { return isHTMLTemplateElement(node.get()); }
template<typename T> inline bool isHTMLTemplateElement(const RefPtr<T>& node) { return isHTMLTemplateElement(node.get()); }
inline bool isHTMLTemplateElement(const Node& node) { return node.isHTMLElement() && isHTMLTemplateElement(toHTMLElement(node)); }
inline bool isHTMLTemplateElement(const Node* node) { return node && isHTMLTemplateElement(*node); }
template <> inline bool isElementOfType<const HTMLTemplateElement>(const Node& node) { return isHTMLTemplateElement(node); }
template <> inline bool isElementOfType<const HTMLTemplateElement>(const HTMLElement& element) { return isHTMLTemplateElement(element); }

class HTMLTitleElement;
void isHTMLTitleElement(const HTMLTitleElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLTitleElement(const HTMLTitleElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLTitleElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::titleTag);
}
inline bool isHTMLTitleElement(const HTMLElement* element) { return element && isHTMLTitleElement(*element); }
template<typename T> inline bool isHTMLTitleElement(const PassRefPtr<T>& node) { return isHTMLTitleElement(node.get()); }
template<typename T> inline bool isHTMLTitleElement(const RefPtr<T>& node) { return isHTMLTitleElement(node.get()); }
inline bool isHTMLTitleElement(const Node& node) { return node.isHTMLElement() && isHTMLTitleElement(toHTMLElement(node)); }
inline bool isHTMLTitleElement(const Node* node) { return node && isHTMLTitleElement(*node); }
template <> inline bool isElementOfType<const HTMLTitleElement>(const Node& node) { return isHTMLTitleElement(node); }
template <> inline bool isElementOfType<const HTMLTitleElement>(const HTMLElement& element) { return isHTMLTitleElement(element); }

class HTMLUListElement;
void isHTMLUListElement(const HTMLUListElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLUListElement(const HTMLUListElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLUListElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::ulTag);
}
inline bool isHTMLUListElement(const HTMLElement* element) { return element && isHTMLUListElement(*element); }
template<typename T> inline bool isHTMLUListElement(const PassRefPtr<T>& node) { return isHTMLUListElement(node.get()); }
template<typename T> inline bool isHTMLUListElement(const RefPtr<T>& node) { return isHTMLUListElement(node.get()); }
inline bool isHTMLUListElement(const Node& node) { return node.isHTMLElement() && isHTMLUListElement(toHTMLElement(node)); }
inline bool isHTMLUListElement(const Node* node) { return node && isHTMLUListElement(*node); }
template <> inline bool isElementOfType<const HTMLUListElement>(const Node& node) { return isHTMLUListElement(node); }
template <> inline bool isElementOfType<const HTMLUListElement>(const HTMLElement& element) { return isHTMLUListElement(element); }

class HTMLButtonElement;
void isHTMLButtonElement(const HTMLButtonElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLButtonElement(const HTMLButtonElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLButtonElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::buttonTag);
}
inline bool isHTMLButtonElement(const HTMLElement* element) { return element && isHTMLButtonElement(*element); }
template<typename T> inline bool isHTMLButtonElement(const PassRefPtr<T>& node) { return isHTMLButtonElement(node.get()); }
template<typename T> inline bool isHTMLButtonElement(const RefPtr<T>& node) { return isHTMLButtonElement(node.get()); }
inline bool isHTMLButtonElement(const Node& node) { return node.isHTMLElement() && isHTMLButtonElement(toHTMLElement(node)); }
inline bool isHTMLButtonElement(const Node* node) { return node && isHTMLButtonElement(*node); }
template <> inline bool isElementOfType<const HTMLButtonElement>(const Node& node) { return isHTMLButtonElement(node); }
template <> inline bool isElementOfType<const HTMLButtonElement>(const HTMLElement& element) { return isHTMLButtonElement(element); }

class HTMLFieldSetElement;
void isHTMLFieldSetElement(const HTMLFieldSetElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLFieldSetElement(const HTMLFieldSetElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLFieldSetElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::fieldsetTag);
}
inline bool isHTMLFieldSetElement(const HTMLElement* element) { return element && isHTMLFieldSetElement(*element); }
template<typename T> inline bool isHTMLFieldSetElement(const PassRefPtr<T>& node) { return isHTMLFieldSetElement(node.get()); }
template<typename T> inline bool isHTMLFieldSetElement(const RefPtr<T>& node) { return isHTMLFieldSetElement(node.get()); }
inline bool isHTMLFieldSetElement(const Node& node) { return node.isHTMLElement() && isHTMLFieldSetElement(toHTMLElement(node)); }
inline bool isHTMLFieldSetElement(const Node* node) { return node && isHTMLFieldSetElement(*node); }
template <> inline bool isElementOfType<const HTMLFieldSetElement>(const Node& node) { return isHTMLFieldSetElement(node); }
template <> inline bool isElementOfType<const HTMLFieldSetElement>(const HTMLElement& element) { return isHTMLFieldSetElement(element); }

class HTMLLabelElement;
void isHTMLLabelElement(const HTMLLabelElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLLabelElement(const HTMLLabelElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLLabelElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::labelTag);
}
inline bool isHTMLLabelElement(const HTMLElement* element) { return element && isHTMLLabelElement(*element); }
template<typename T> inline bool isHTMLLabelElement(const PassRefPtr<T>& node) { return isHTMLLabelElement(node.get()); }
template<typename T> inline bool isHTMLLabelElement(const RefPtr<T>& node) { return isHTMLLabelElement(node.get()); }
inline bool isHTMLLabelElement(const Node& node) { return node.isHTMLElement() && isHTMLLabelElement(toHTMLElement(node)); }
inline bool isHTMLLabelElement(const Node* node) { return node && isHTMLLabelElement(*node); }
template <> inline bool isElementOfType<const HTMLLabelElement>(const Node& node) { return isHTMLLabelElement(node); }
template <> inline bool isElementOfType<const HTMLLabelElement>(const HTMLElement& element) { return isHTMLLabelElement(element); }

class HTMLSelectElement;
void isHTMLSelectElement(const HTMLSelectElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLSelectElement(const HTMLSelectElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLSelectElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::selectTag);
}
inline bool isHTMLSelectElement(const HTMLElement* element) { return element && isHTMLSelectElement(*element); }
template<typename T> inline bool isHTMLSelectElement(const PassRefPtr<T>& node) { return isHTMLSelectElement(node.get()); }
template<typename T> inline bool isHTMLSelectElement(const RefPtr<T>& node) { return isHTMLSelectElement(node.get()); }
inline bool isHTMLSelectElement(const Node& node) { return node.isHTMLElement() && isHTMLSelectElement(toHTMLElement(node)); }
inline bool isHTMLSelectElement(const Node* node) { return node && isHTMLSelectElement(*node); }
template <> inline bool isElementOfType<const HTMLSelectElement>(const Node& node) { return isHTMLSelectElement(node); }
template <> inline bool isElementOfType<const HTMLSelectElement>(const HTMLElement& element) { return isHTMLSelectElement(element); }

class HTMLTextAreaElement;
void isHTMLTextAreaElement(const HTMLTextAreaElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLTextAreaElement(const HTMLTextAreaElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLTextAreaElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::textareaTag);
}
inline bool isHTMLTextAreaElement(const HTMLElement* element) { return element && isHTMLTextAreaElement(*element); }
template<typename T> inline bool isHTMLTextAreaElement(const PassRefPtr<T>& node) { return isHTMLTextAreaElement(node.get()); }
template<typename T> inline bool isHTMLTextAreaElement(const RefPtr<T>& node) { return isHTMLTextAreaElement(node.get()); }
inline bool isHTMLTextAreaElement(const Node& node) { return node.isHTMLElement() && isHTMLTextAreaElement(toHTMLElement(node)); }
inline bool isHTMLTextAreaElement(const Node* node) { return node && isHTMLTextAreaElement(*node); }
template <> inline bool isElementOfType<const HTMLTextAreaElement>(const Node& node) { return isHTMLTextAreaElement(node); }
template <> inline bool isElementOfType<const HTMLTextAreaElement>(const HTMLElement& element) { return isHTMLTextAreaElement(element); }

class HTMLLinkElement;
void isHTMLLinkElement(const HTMLLinkElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLLinkElement(const HTMLLinkElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLLinkElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::linkTag);
}
inline bool isHTMLLinkElement(const HTMLElement* element) { return element && isHTMLLinkElement(*element); }
template<typename T> inline bool isHTMLLinkElement(const PassRefPtr<T>& node) { return isHTMLLinkElement(node.get()); }
template<typename T> inline bool isHTMLLinkElement(const RefPtr<T>& node) { return isHTMLLinkElement(node.get()); }
inline bool isHTMLLinkElement(const Node& node) { return node.isHTMLElement() && isHTMLLinkElement(toHTMLElement(node)); }
inline bool isHTMLLinkElement(const Node* node) { return node && isHTMLLinkElement(*node); }
template <> inline bool isElementOfType<const HTMLLinkElement>(const Node& node) { return isHTMLLinkElement(node); }
template <> inline bool isElementOfType<const HTMLLinkElement>(const HTMLElement& element) { return isHTMLLinkElement(element); }

class HTMLScriptElement;
void isHTMLScriptElement(const HTMLScriptElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLScriptElement(const HTMLScriptElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLScriptElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::scriptTag);
}
inline bool isHTMLScriptElement(const HTMLElement* element) { return element && isHTMLScriptElement(*element); }
template<typename T> inline bool isHTMLScriptElement(const PassRefPtr<T>& node) { return isHTMLScriptElement(node.get()); }
template<typename T> inline bool isHTMLScriptElement(const RefPtr<T>& node) { return isHTMLScriptElement(node.get()); }
inline bool isHTMLScriptElement(const Node& node) { return node.isHTMLElement() && isHTMLScriptElement(toHTMLElement(node)); }
inline bool isHTMLScriptElement(const Node* node) { return node && isHTMLScriptElement(*node); }
template <> inline bool isElementOfType<const HTMLScriptElement>(const Node& node) { return isHTMLScriptElement(node); }
template <> inline bool isElementOfType<const HTMLScriptElement>(const HTMLElement& element) { return isHTMLScriptElement(element); }

class HTMLStyleElement;
void isHTMLStyleElement(const HTMLStyleElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLStyleElement(const HTMLStyleElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLStyleElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::styleTag);
}
inline bool isHTMLStyleElement(const HTMLElement* element) { return element && isHTMLStyleElement(*element); }
template<typename T> inline bool isHTMLStyleElement(const PassRefPtr<T>& node) { return isHTMLStyleElement(node.get()); }
template<typename T> inline bool isHTMLStyleElement(const RefPtr<T>& node) { return isHTMLStyleElement(node.get()); }
inline bool isHTMLStyleElement(const Node& node) { return node.isHTMLElement() && isHTMLStyleElement(toHTMLElement(node)); }
inline bool isHTMLStyleElement(const Node* node) { return node && isHTMLStyleElement(*node); }
template <> inline bool isElementOfType<const HTMLStyleElement>(const Node& node) { return isHTMLStyleElement(node); }
template <> inline bool isElementOfType<const HTMLStyleElement>(const HTMLElement& element) { return isHTMLStyleElement(element); }

class HTMLImageElement;
void isHTMLImageElement(const HTMLImageElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLImageElement(const HTMLImageElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLImageElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::imgTag);
}
inline bool isHTMLImageElement(const HTMLElement* element) { return element && isHTMLImageElement(*element); }
template<typename T> inline bool isHTMLImageElement(const PassRefPtr<T>& node) { return isHTMLImageElement(node.get()); }
template<typename T> inline bool isHTMLImageElement(const RefPtr<T>& node) { return isHTMLImageElement(node.get()); }
inline bool isHTMLImageElement(const Node& node) { return node.isHTMLElement() && isHTMLImageElement(toHTMLElement(node)); }
inline bool isHTMLImageElement(const Node* node) { return node && isHTMLImageElement(*node); }
template <> inline bool isElementOfType<const HTMLImageElement>(const Node& node) { return isHTMLImageElement(node); }
template <> inline bool isElementOfType<const HTMLImageElement>(const HTMLElement& element) { return isHTMLImageElement(element); }

class HTMLInputElement;
void isHTMLInputElement(const HTMLInputElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLInputElement(const HTMLInputElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLInputElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::inputTag);
}
inline bool isHTMLInputElement(const HTMLElement* element) { return element && isHTMLInputElement(*element); }
template<typename T> inline bool isHTMLInputElement(const PassRefPtr<T>& node) { return isHTMLInputElement(node.get()); }
template<typename T> inline bool isHTMLInputElement(const RefPtr<T>& node) { return isHTMLInputElement(node.get()); }
inline bool isHTMLInputElement(const Node& node) { return node.isHTMLElement() && isHTMLInputElement(toHTMLElement(node)); }
inline bool isHTMLInputElement(const Node* node) { return node && isHTMLInputElement(*node); }
template <> inline bool isElementOfType<const HTMLInputElement>(const Node& node) { return isHTMLInputElement(node); }
template <> inline bool isElementOfType<const HTMLInputElement>(const HTMLElement& element) { return isHTMLInputElement(element); }

class HTMLBDIElement;
void isHTMLBDIElement(const HTMLBDIElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLBDIElement(const HTMLBDIElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLBDIElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::bdiTag);
}
inline bool isHTMLBDIElement(const HTMLElement* element) { return element && isHTMLBDIElement(*element); }
template<typename T> inline bool isHTMLBDIElement(const PassRefPtr<T>& node) { return isHTMLBDIElement(node.get()); }
template<typename T> inline bool isHTMLBDIElement(const RefPtr<T>& node) { return isHTMLBDIElement(node.get()); }
inline bool isHTMLBDIElement(const Node& node) { return node.isHTMLElement() && isHTMLBDIElement(toHTMLElement(node)); }
inline bool isHTMLBDIElement(const Node* node) { return node && isHTMLBDIElement(*node); }
template <> inline bool isElementOfType<const HTMLBDIElement>(const Node& node) { return isHTMLBDIElement(node); }
template <> inline bool isElementOfType<const HTMLBDIElement>(const HTMLElement& element) { return isHTMLBDIElement(element); }

class HTMLSummaryElement;
void isHTMLSummaryElement(const HTMLSummaryElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLSummaryElement(const HTMLSummaryElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLSummaryElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::summaryTag);
}
inline bool isHTMLSummaryElement(const HTMLElement* element) { return element && isHTMLSummaryElement(*element); }
template<typename T> inline bool isHTMLSummaryElement(const PassRefPtr<T>& node) { return isHTMLSummaryElement(node.get()); }
template<typename T> inline bool isHTMLSummaryElement(const RefPtr<T>& node) { return isHTMLSummaryElement(node.get()); }
inline bool isHTMLSummaryElement(const Node& node) { return node.isHTMLElement() && isHTMLSummaryElement(toHTMLElement(node)); }
inline bool isHTMLSummaryElement(const Node* node) { return node && isHTMLSummaryElement(*node); }
template <> inline bool isElementOfType<const HTMLSummaryElement>(const Node& node) { return isHTMLSummaryElement(node); }
template <> inline bool isElementOfType<const HTMLSummaryElement>(const HTMLElement& element) { return isHTMLSummaryElement(element); }

class HTMLWBRElement;
void isHTMLWBRElement(const HTMLWBRElement&); // Catch unnecessary runtime check of type known at compile time.
void isHTMLWBRElement(const HTMLWBRElement*); // Catch unnecessary runtime check of type known at compile time.

inline bool isHTMLWBRElement(const HTMLElement& element) {
    return element.hasTagName(HTMLNames::wbrTag);
}
inline bool isHTMLWBRElement(const HTMLElement* element) { return element && isHTMLWBRElement(*element); }
template<typename T> inline bool isHTMLWBRElement(const PassRefPtr<T>& node) { return isHTMLWBRElement(node.get()); }
template<typename T> inline bool isHTMLWBRElement(const RefPtr<T>& node) { return isHTMLWBRElement(node.get()); }
inline bool isHTMLWBRElement(const Node& node) { return node.isHTMLElement() && isHTMLWBRElement(toHTMLElement(node)); }
inline bool isHTMLWBRElement(const Node* node) { return node && isHTMLWBRElement(*node); }
template <> inline bool isElementOfType<const HTMLWBRElement>(const Node& node) { return isHTMLWBRElement(node); }
template <> inline bool isElementOfType<const HTMLWBRElement>(const HTMLElement& element) { return isHTMLWBRElement(element); }

// Using macros because the types are forward-declared and we don't want to use reinterpret_cast in the
// casting functions above. reinterpret_cast would be unsafe due to multiple inheritence.

#define toHTMLAnchorElement(x) blink::toElement<blink::HTMLAnchorElement>(x)
#define toHTMLAreaElement(x) blink::toElement<blink::HTMLAreaElement>(x)
#define toHTMLBRElement(x) blink::toElement<blink::HTMLBRElement>(x)
#define toHTMLBaseElement(x) blink::toElement<blink::HTMLBaseElement>(x)
#define toHTMLBodyElement(x) blink::toElement<blink::HTMLBodyElement>(x)
#define toHTMLContentElement(x) blink::toElement<blink::HTMLContentElement>(x)
#define toHTMLDListElement(x) blink::toElement<blink::HTMLDListElement>(x)
#define toHTMLDataListElement(x) blink::toElement<blink::HTMLDataListElement>(x)
#define toHTMLDetailsElement(x) blink::toElement<blink::HTMLDetailsElement>(x)
#define toHTMLDirectoryElement(x) blink::toElement<blink::HTMLDirectoryElement>(x)
#define toHTMLDivElement(x) blink::toElement<blink::HTMLDivElement>(x)
#define toHTMLFontElement(x) blink::toElement<blink::HTMLFontElement>(x)
#define toHTMLFormElement(x) blink::toElement<blink::HTMLFormElement>(x)
#define toHTMLHRElement(x) blink::toElement<blink::HTMLHRElement>(x)
#define toHTMLHeadElement(x) blink::toElement<blink::HTMLHeadElement>(x)
#define toHTMLHtmlElement(x) blink::toElement<blink::HTMLHtmlElement>(x)
#define toHTMLLIElement(x) blink::toElement<blink::HTMLLIElement>(x)
#define toHTMLLegendElement(x) blink::toElement<blink::HTMLLegendElement>(x)
#define toHTMLMarqueeElement(x) blink::toElement<blink::HTMLMarqueeElement>(x)
#define toHTMLMenuElement(x) blink::toElement<blink::HTMLMenuElement>(x)
#define toHTMLMenuItemElement(x) blink::toElement<blink::HTMLMenuItemElement>(x)
#define toHTMLMetaElement(x) blink::toElement<blink::HTMLMetaElement>(x)
#define toHTMLMeterElement(x) blink::toElement<blink::HTMLMeterElement>(x)
#define toHTMLOListElement(x) blink::toElement<blink::HTMLOListElement>(x)
#define toHTMLOptGroupElement(x) blink::toElement<blink::HTMLOptGroupElement>(x)
#define toHTMLOptionElement(x) blink::toElement<blink::HTMLOptionElement>(x)
#define toHTMLParagraphElement(x) blink::toElement<blink::HTMLParagraphElement>(x)
#define toHTMLPictureElement(x) blink::toElement<blink::HTMLPictureElement>(x)
#define toHTMLProgressElement(x) blink::toElement<blink::HTMLProgressElement>(x)
#define toHTMLShadowElement(x) blink::toElement<blink::HTMLShadowElement>(x)
#define toHTMLSlotElement(x) blink::toElement<blink::HTMLSlotElement>(x)
#define toHTMLSpanElement(x) blink::toElement<blink::HTMLSpanElement>(x)
#define toHTMLTableCaptionElement(x) blink::toElement<blink::HTMLTableCaptionElement>(x)
#define toHTMLTableElement(x) blink::toElement<blink::HTMLTableElement>(x)
#define toHTMLTableRowElement(x) blink::toElement<blink::HTMLTableRowElement>(x)
#define toHTMLTemplateElement(x) blink::toElement<blink::HTMLTemplateElement>(x)
#define toHTMLTitleElement(x) blink::toElement<blink::HTMLTitleElement>(x)
#define toHTMLUListElement(x) blink::toElement<blink::HTMLUListElement>(x)
#define toHTMLButtonElement(x) blink::toElement<blink::HTMLButtonElement>(x)
#define toHTMLFieldSetElement(x) blink::toElement<blink::HTMLFieldSetElement>(x)
#define toHTMLLabelElement(x) blink::toElement<blink::HTMLLabelElement>(x)
#define toHTMLSelectElement(x) blink::toElement<blink::HTMLSelectElement>(x)
#define toHTMLTextAreaElement(x) blink::toElement<blink::HTMLTextAreaElement>(x)
#define toHTMLLinkElement(x) blink::toElement<blink::HTMLLinkElement>(x)
#define toHTMLScriptElement(x) blink::toElement<blink::HTMLScriptElement>(x)
#define toHTMLStyleElement(x) blink::toElement<blink::HTMLStyleElement>(x)
#define toHTMLImageElement(x) blink::toElement<blink::HTMLImageElement>(x)
#define toHTMLInputElement(x) blink::toElement<blink::HTMLInputElement>(x)
#define toHTMLBDIElement(x) blink::toElement<blink::HTMLBDIElement>(x)
#define toHTMLSummaryElement(x) blink::toElement<blink::HTMLSummaryElement>(x)
#define toHTMLWBRElement(x) blink::toElement<blink::HTMLWBRElement>(x)
} // namespace blink

#endif
