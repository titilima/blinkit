// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ResourceClientWalker.h
// Description: ResourceClientWalker Class
//      Author: Ziming Li
//     Created: 2021-07-23
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 1998 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2001 Dirk Mueller <mueller@kde.org>
    Copyright (C) 2004, 2005, 2006, 2007 Apple Inc. All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

    This class provides all functionality needed for loading images, style sheets and html
    pages from the web. It has a memory cache for these objects.
*/

#ifndef ResourceClientWalker_h
#define ResourceClientWalker_h

#include "core/fetch/ResourceClient.h"
#include "wtf/Allocator.h"
#include "wtf/HashCountedSet.h"
#include "wtf/Vector.h"

namespace blink {

// Call this "walker" instead of iterator so people won't expect Qt or STL-style iterator interface.
// Just keep calling next() on this. It's safe from deletions of items.
template<typename T> class ResourceClientWalker {
    STACK_ALLOCATED();
public:
    ResourceClientWalker(const HashCountedSet<ResourceClient*>& set)
        : m_clientSet(set), m_clientVector(set.size()), m_index(0)
    {
        size_t clientIndex = 0;
        for (const auto& resourceClient : set)
            m_clientVector[clientIndex++] = resourceClient.first;
    }

    T* next()
    {
        size_t size = m_clientVector.size();
        while (m_index < size) {
            ResourceClient* next = m_clientVector[m_index++];
            if (m_clientSet.contains(next)) {
                ASSERT(T::expectedType() == ResourceClient::expectedType() || next->resourceClientType() == T::expectedType());
                return static_cast<T*>(next);
            }
        }

        return 0;
    }
private:
    const HashCountedSet<ResourceClient*>& m_clientSet;
    Vector<ResourceClient*> m_clientVector;
    size_t m_index;
};

}

#endif
