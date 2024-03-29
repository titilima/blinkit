#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: IdentityTransformOperation.h
// Description: IdentityTransformOperation Class
//      Author: Ziming Li
//     Created: 2022-01-10
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2000 Lars Knoll (knoll@kde.org)
 *           (C) 2000 Antti Koivisto (koivisto@kde.org)
 *           (C) 2000 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2003, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Graham Dennis (graham.dennis@gmail.com)
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

#ifndef IdentityTransformOperation_h
#define IdentityTransformOperation_h

#include "blinkit/blink/renderer/platform/transforms/TransformOperation.h"

namespace blink {

class IdentityTransformOperation final : public TransformOperation
{
public:
    static PassRefPtr<IdentityTransformOperation> create()
    {
        return adoptRef(new IdentityTransformOperation());
    }

    virtual bool canBlendWith(const TransformOperation& other) const
    {
        return isSameType(other);
    }

private:
    OperationType type() const override { return Identity; }

    bool operator==(const TransformOperation& o) const override
    {
        return isSameType(o);
    }

    void apply(TransformationMatrix&, const FloatSize&) const override { }

    PassRefPtr<TransformOperation> blend(const TransformOperation*, double, bool = false) override
    {
        return this;
    }

    PassRefPtr<TransformOperation> zoom(double factor) final
    {
        return this;
    }

    IdentityTransformOperation()
    {
    }

};

} // namespace blink

#endif // IdentityTransformOperation_h
