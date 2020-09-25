// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: html_element.h
// Description: HTMLElement Class
//      Author: Ziming Li
//     Created: 2020-08-28
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_HTML_ELEMENT_H
#define BLINKIT_BLINK_HTML_ELEMENT_H

#pragma once

#include "third_party/blink/renderer/core/dom/element.h"

namespace blink {

class HTMLElement : public Element
{
};

} // namespace blink

#include "third_party/blink/renderer/core/html_element_type_helpers.h"

#endif // BLINKIT_BLINK_HTML_ELEMENT_H
