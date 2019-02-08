// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: BaseTextInputType.cpp
// Description: BaseTextInputType Class
//      Author: Ziming Li
//     Created: 2019-02-08
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * This file is part of the WebKit project.
 *
 * Copyright (C) 2009 Michelangelo De Simone <micdesim@gmail.com>
 * Copyright (C) 2010 Google Inc. All rights reserved.
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

#include "core/html/forms/BaseTextInputType.h"

#include <regex>
#include "core/HTMLNames.h"
#include "core/html/HTMLInputElement.h"

namespace blink {

using namespace HTMLNames;

int BaseTextInputType::maxLength() const
{
    return element().maxLength();
}

int BaseTextInputType::minLength() const
{
    return element().minLength();
}

bool BaseTextInputType::tooLong(const String& value, HTMLTextFormControlElement::NeedsToCheckDirtyFlag check) const
{
    int max = element().maxLength();
    if (max < 0)
        return false;
    if (check == HTMLTextFormControlElement::CheckDirtyFlag) {
        // Return false for the default value or a value set by a script even if
        // it is longer than maxLength.
        if (!element().hasDirtyValue() || !element().lastChangeWasUserEdit())
            return false;
    }
    return value.length() > static_cast<unsigned>(max);
}

bool BaseTextInputType::tooShort(const String& value, HTMLTextFormControlElement::NeedsToCheckDirtyFlag check) const
{
    int min = element().minLength();
    if (min <= 0)
        return false;
    if (check == HTMLTextFormControlElement::CheckDirtyFlag) {
        // Return false for the default value or a value set by a script even if
        // it is shorter than minLength.
        if (!element().hasDirtyValue() || !element().lastChangeWasUserEdit())
            return false;
    }
    // An empty string is excluded from minlength check.
    unsigned len = value.length();
    return len > 0 && len < static_cast<unsigned>(min);
}

bool BaseTextInputType::patternMismatch(const String& value) const
{
    const AtomicString& rawPattern = element().fastGetAttribute(patternAttr);
    // Empty values can't be mismatched
    if (rawPattern.isNull() || value.isEmpty())
        return false;

    std::string pattern = rawPattern.to_string();
    try {
        std::regex re(pattern);
    }
    catch (...)
    {
        return false;
    }

    pattern.insert(0, "^(?:");
    pattern.append(")$");

    std::regex re(pattern);
    return !std::regex_match(value.to_string(), re);
}

bool BaseTextInputType::supportsPlaceholder() const
{
    return true;
}

bool BaseTextInputType::supportsSelectionAPI() const
{
    return true;
}

bool BaseTextInputType::supportsAutocapitalize() const
{
    return true;
}

} // namespace blink
