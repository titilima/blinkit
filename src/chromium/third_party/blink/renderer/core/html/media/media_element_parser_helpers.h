// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: media_element_parser_helpers.h
// Description: Media Element Parser Helpers
//      Author: Ziming Li
//     Created: 2021-01-08
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_MEDIA_ELEMENT_PARSER_HELPERS_H
#define BLINKIT_BLINK_MEDIA_ELEMENT_PARSER_HELPERS_H

#pragma once

namespace blink {

class Document;
class Element;

namespace MediaElementParserHelpers {

bool IsMediaElement(const Element *element);

inline constexpr bool IsUnsizedMediaEnabled(const Document &) { return true; }

} // namespace MediaElementParserHelpers
} // namespace blink

#endif // BLINKIT_BLINK_MEDIA_ELEMENT_PARSER_HELPERS_H
