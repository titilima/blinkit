// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: element_rare_data.h
// Description: ElementRareData Class
//      Author: Ziming Li
//     Created: 2019-11-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008, 2009, 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2008 David Smith <catfish.man@gmail.com>
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

#ifndef BLINKIT_BLINK_ELEMENT_RARE_DATA_H
#define BLINKIT_BLINK_ELEMENT_RARE_DATA_H

#pragma once

namespace blink {

class ElementRareData // BKTODO : public NodeRareData
{
public:
    ~ElementRareData(void);

    bool HasPseudoElements(void) const;
};

} // namespace blink

#endif // BLINKIT_BLINK_ELEMENT_RARE_DATA_H
