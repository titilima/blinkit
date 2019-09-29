// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ref_counted.h
// Description: Placeholders for WTF Part
//      Author: Ziming Li
//     Created: 2019-09-29
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights reserved.
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
 *
 */

#ifndef BLINKIT_BLINK_REF_COUNTED_H
#define BLINKIT_BLINK_REF_COUNTED_H

#pragma once

#include "base/memory/ref_counted.h"

namespace WTF {

template <class T>
class RefCounted : public base::RefCounted<T>
{
};

// Allows subclasses to use the default copy constructor.
template <class T>
class RefCountedCopyable : public RefCounted<T>
{
protected:
    RefCountedCopyable(void) = default;
    RefCountedCopyable(const RefCountedCopyable&) : RefCounted<T>() {}
};

}  // namespace WTF

using WTF::RefCounted;
using WTF::RefCountedCopyable;

#endif  // BLINKIT_BLINK_REF_COUNTED_H
