#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: SkewTransformOperation.h
// Description: SkewTransformOperation Class
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

#ifndef SkewTransformOperation_h
#define SkewTransformOperation_h

#include "blinkit/blink/renderer/platform/transforms/TransformOperation.h"

namespace blink {

class SkewTransformOperation final : public TransformOperation
{
public:
    static PassRefPtr<SkewTransformOperation> create(double angleX, double angleY, OperationType type)
    {
        return adoptRef(new SkewTransformOperation(angleX, angleY, type));
    }

    double angleX() const { return m_angleX; }
    double angleY() const { return m_angleY; }

    virtual bool canBlendWith(const TransformOperation& other) const;
private:
    OperationType type() const override { return m_type; }

    bool operator==(const TransformOperation& o) const override
    {
        if (!isSameType(o))
            return false;
        const SkewTransformOperation* s = static_cast<const SkewTransformOperation*>(&o);
        return m_angleX == s->m_angleX && m_angleY == s->m_angleY;
    }

    void apply(TransformationMatrix& transform, const FloatSize&) const override
    {
        transform.skew(m_angleX, m_angleY);
    }

    PassRefPtr<TransformOperation> blend(const TransformOperation* from, double progress, bool blendToIdentity = false) override;
    PassRefPtr<TransformOperation> zoom(double factor) final { return this; }

    SkewTransformOperation(double angleX, double angleY, OperationType type)
        : m_angleX(angleX)
        , m_angleY(angleY)
        , m_type(type)
    {
    }

    double m_angleX;
    double m_angleY;
    OperationType m_type;
};

} // namespace blink

#endif // SkewTransformOperation_h
