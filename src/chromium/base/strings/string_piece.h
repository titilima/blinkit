// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: string_piece.h
// Description: StringPiece Classes & Helpers
//      Author: Ziming Li
//     Created: 2018-08-16
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BASE_STRING_PIECE_H
#define BLINKIT_BASE_STRING_PIECE_H

#pragma once

#include <string_view>
#include "base/strings/string16.h"

namespace base {

template <typename StringType> class BasicStringPiece;

typedef BasicStringPiece<std::string>   StringPiece;
typedef BasicStringPiece<string16>      StringPiece16;

template <typename StringType>
class BasicStringPiece : public std::basic_string_view<typename StringType::value_type>
{
public:
    typedef std::basic_string_view<typename StringType::value_type> BaseClass;

    using value_type = typename BaseClass::value_type;
    using size_type  = typename BaseClass::size_type;

    BasicStringPiece(void) = default;
    BasicStringPiece(const BaseClass &s) : BaseClass(s) {}
    BasicStringPiece(const value_type *s) : BaseClass(s) {}
    BasicStringPiece(const value_type *s, size_type count) : BaseClass(s, count) {}
    BasicStringPiece(const StringType &s) : BaseClass(s) {}
    BasicStringPiece(const typename StringType::const_iterator &begin, const typename StringType::const_iterator &end)
        : BaseClass(&*begin, std::distance(begin, end)) {}

    StringType as_string(void) const
    {
        return BaseClass::empty() ? StringType() : StringType(BaseClass::data(), BaseClass::length());
    }
    void AppendToString(StringType *target) const
    {
        if (!BaseClass::empty())
            target->append(BaseClass::data(), BaseClass::length());
    }
    void CopyToString(StringType *target) const
    {
        if (BaseClass::empty())
            target->clear();
        else
            target->assign(BaseClass::data(), BaseClass::length());
    }
    BasicStringPiece substr(size_type pos = 0, size_type count = BaseClass::npos) const
    {
        return BaseClass::substr(pos, count);
    }
};

} // namespace base

#endif // BLINKIT_BASE_STRING_PIECE_H
