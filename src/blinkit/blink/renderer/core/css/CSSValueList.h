// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CSSValueList.h
// Description: CSSValueList Class
//      Author: Ziming Li
//     Created: 2021-07-14
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights reserved.
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

#ifndef CSSValueList_h
#define CSSValueList_h

#include "core/CoreExport.h"
#include "core/css/CSSValue.h"

namespace blink {

class CORE_EXPORT CSSValueList : public CSSValue {
public:
    using iterator = std::vector<GCRefPtr<CSSValue>>::iterator;
    using const_iterator = std::vector<GCRefPtr<CSSValue>>::const_iterator;

    static GCRefPtr<CSSValueList> createCommaSeparated()
    {
        return BlinKit::GCWrapShared(new CSSValueList(CommaSeparator));
    }
    static GCRefPtr<CSSValueList> createSpaceSeparated()
    {
        return BlinKit::GCWrapShared(new CSSValueList(SpaceSeparator));
    }
    static GCRefPtr<CSSValueList> createSlashSeparated()
    {
        return BlinKit::GCWrapShared(new CSSValueList(SlashSeparator));
    }

    iterator begin() { return m_values.begin(); }
    iterator end() { return m_values.end(); }
    const_iterator begin() const { return m_values.begin(); }
    const_iterator end() const { return m_values.end(); }

    size_t length() const { return m_values.size(); }
    CSSValue* item(size_t index) { return m_values[index].get(); }
    const CSSValue* item(size_t index) const { return m_values[index].get(); }
    CSSValue* itemWithBoundsCheck(size_t index) { return index < m_values.size() ? m_values[index].get() : nullptr; }
    const CSSValue* itemWithBoundsCheck(size_t index) const { return index < m_values.size() ? m_values[index].get() : nullptr; }

    void append(CSSValue *value) { m_values.emplace_back(value); }
    void append(const GCRefPtr<CSSValue> &value) { m_values.emplace_back(value); }
    void prepend(CSSValue *value) { m_values.emplace(m_values.begin(), value); }
    void prepend(const GCRefPtr<CSSValue> &value) { m_values.emplace(m_values.begin(), value); }
    bool removeAll(CSSValue*);
    bool hasValue(CSSValue*) const;
    GCRefPtr<CSSValueList> copy();

    String customCSSText() const;
    bool equals(const CSSValueList&) const;

    bool hasFailedOrCanceledSubresources() const;

    DECLARE_TRACE_AFTER_DISPATCH();

protected:
    CSSValueList(ClassType, ValueListSeparator);

private:
    explicit CSSValueList(ValueListSeparator);

    std::vector<GCRefPtr<CSSValue>> m_values;
};

DEFINE_CSS_VALUE_TYPE_CASTS(CSSValueList, isValueList());

} // namespace blink

#endif // CSSValueList_h
