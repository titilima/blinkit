// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: container_node.h
// Description: ContainerNode Class
//      Author: Ziming Li
//     Created: 2019-09-16
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2004, 2005, 2006, 2007, 2009, 2010, 2011, 2013 Apple Inc. All
 * rights reserved.
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

#ifndef BLINKIT_BLINK_CONTAINER_NODE_H
#define BLINKIT_BLINK_CONTAINER_NODE_H

#include "third_party/blink/renderer/core/dom/node.h"

namespace blink {

class ContainerNode : public Node
{
protected:
    ContainerNode(TreeScope *treeScope, ConstructionType type = kCreateContainer);
};

}  // namespace blink

#endif  // BLINKIT_BLINK_CONTAINER_NODE_H
