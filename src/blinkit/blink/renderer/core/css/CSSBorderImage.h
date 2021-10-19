// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSBorderImage.h
// Description: CSSBorderImage Stuff
//      Author: Ziming Li
//     Created: 2021-10-19
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies)
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
 */

#ifndef CSSBorderImage_h
#define CSSBorderImage_h

#include "core/css/CSSBorderImageSliceValue.h"
#include "core/css/CSSValueList.h"

namespace blink {

GCRefPtr<CSSValueList> createBorderImageValue(
    const GCRefPtr<CSSValue> &image,
    const GCRefPtr<CSSValue> &imageSlice,
    const GCRefPtr<CSSValue> &borderSlice,
    const GCRefPtr<CSSValue> &outset,
    const GCRefPtr<CSSValue> &repeat
);

} // namespace blink

#endif // CSSBorderImage_h
