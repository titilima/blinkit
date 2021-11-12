// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: HTMLProgressElement.h
// Description: HTMLProgressElement Class
//      Author: Ziming Li
//     Created: 2021-11-11
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef HTMLProgressElement_h
#define HTMLProgressElement_h

#include "core/html/LabelableElement.h"

namespace blink {

class ProgressValueElement;
class LayoutProgress;

class HTMLProgressElement final : public LabelableElement
{
public:
    static const double IndeterminatePosition;
    static const double InvalidPosition;

    static GCRefPtr<HTMLProgressElement> create(Document&);

    double value() const;
    void setValue(double);

    double max() const;
    void setMax(double);

    double position() const;

    bool canContainRangeEndPoint() const override { return false; }
private:
    explicit HTMLProgressElement(Document&);
    ~HTMLProgressElement() override;

    bool areAuthorShadowsAllowed() const override { return false; }
    void willAddFirstAuthorShadowRoot() override;
    bool shouldAppearIndeterminate() const override;
    bool supportLabels() const override { return true; }

    LayoutObject* createLayoutObject(const ComputedStyle&) override;
    LayoutProgress* layoutProgress() const;

    void parseAttribute(const QualifiedName&, const AtomicString&, const AtomicString&) override;

    void attach(const AttachContext& = AttachContext()) override;

    void didElementStateChange();
    void didAddUserAgentShadowRoot(ShadowRoot&) override;
    bool isDeterminate() const;

    ProgressValueElement *m_value = nullptr;
};

} // namespace blink

#endif // HTMLProgressElement_h
