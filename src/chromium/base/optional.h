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

using std::nullopt;

} // namespace base

#endif // BLINKIT_BASE_OPTIONAL_H
