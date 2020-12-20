// -------------------------------------------------
// BlinKit - base Library
// -------------------------------------------------
//   File Name: optional.h
// Description: Placeholders for Optional Stuff
//      Author: Ziming Li
//     Created: 2020-08-29
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BASE_OPTIONAL_H
#define BLINKIT_BASE_OPTIONAL_H

#pragma once

#include <optional>

namespace base {

template <typename T> using Optional = std::optional<T>;

using std::make_optional;
using std::nullopt;

// Helper for returning the optional value's address, or nullptr.
template <class T>
inline T* OptionalOrNullptr(base::Optional<T> &optional)
{
    return optional.has_value() ? &optional.value() : nullptr;
}

template <class T>
const T* OptionalOrNullptr(const base::Optional<T> &optional)
{
    return optional.has_value() ? &optional.value() : nullptr;
}

} // namespace base

#endif // BLINKIT_BASE_OPTIONAL_H
