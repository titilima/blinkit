#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: TemporaryChange.h
// Description: TemporaryChange Class
//      Author: Ziming Li
//     Created: 2021-07-03
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_TEMPORARY_CHANGE_H
#define BLINKIT_BLINKIT_TEMPORARY_CHANGE_H

#include "third_party/zed/include/zed/utility.hpp"

namespace WTF {
template<typename T> using TemporaryChange = zed::scoped_swap<T>;
}
using WTF::TemporaryChange;

#endif // BLINKIT_BLINKIT_TEMPORARY_CHANGE_H
