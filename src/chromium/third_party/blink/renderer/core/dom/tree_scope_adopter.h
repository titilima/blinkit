// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: tree_scope_adopter.h
// Description: TreeScopeAdopter Class
//      Author: Ziming Li
//     Created: 2019-11-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All
 * rights reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
 * Copyright (C) 2011 Google Inc. All rights reserved.
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

#ifndef BLINKIT_BLINK_TREE_SCOPE_ADOPTER_H
#define BLINKIT_BLINK_TREE_SCOPE_ADOPTER_H

#pragma once

#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class Node;
class TreeScope;

class TreeScopeAdopter
{
    STACK_ALLOCATED();
public:
    TreeScopeAdopter(Node &toAdopt, TreeScope &newScope);

    bool NeedsScopeChange(void) const { return m_oldScope != m_newScope; }
    void Execute(void) const;
private:
    Member<Node> m_toAdopt;
    Member<TreeScope> m_newScope;
    Member<TreeScope> m_oldScope;
};

} // namespace blink

#endif // BLINKIT_BLINK_TREE_SCOPE_ADOPTER_H
