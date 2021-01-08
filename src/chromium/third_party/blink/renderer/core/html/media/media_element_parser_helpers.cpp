// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: media_element_parser_helpers.cpp
// Description: Media Element Parser Helpers
//      Author: Ziming Li
//     Created: 2021-01-08
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "media_element_parser_helpers.h"

#include "third_party/blink/renderer/core/html_element_type_helpers.h"

namespace blink {
namespace MediaElementParserHelpers {

bool IsMediaElement(const Element *element)
{
    return IsHTMLImageElement(element);
}

} // namespace MediaElementParserHelpers
} // namespace blink
