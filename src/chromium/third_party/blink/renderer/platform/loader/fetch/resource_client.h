// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: resource_client.h
// Description: ResourceClient Class
//      Author: Ziming Li
//     Created: 2019-09-28
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 1998 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2001 Dirk Mueller <mueller@kde.org>
    Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All
    rights reserved.

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

    This class provides all functionality needed for loading images, style
    sheets and html pages from the web. It has a memory cache for these objects.
*/

#ifndef BLINKIT_BLINK_RESOURCE_CLIENT_H
#define BLINKIT_BLINK_RESOURCE_CLIENT_H

#pragma once

#include "third_party/blink/renderer/platform/heap/handle.h"

namespace base {
class SingleThreadTaskRunner;
}

namespace blink {

class Resource;

class ResourceClient : public GarbageCollectedMixin
{
public:
    virtual ~ResourceClient(void);
    void Trace(Visitor *visitor) override;

    // DataReceived() is called each time a chunk of data is received.
    // For cache hits, the data is replayed before NotifyFinished() is called.
    // For successful revalidation responses, the data is NOT replayed, because
    // the Resource may not be in an entirely consistent state in the middle of
    // completing the revalidation, when DataReceived() would have to be called.
    // Some RawResourceClients depends on receiving all bytes via DataReceived(),
    // but RawResources forbid revalidation attempts, so they still are guaranteed
    // to get all data via DataReceived().
    virtual void DataReceived(Resource *resource, const char *data, size_t length) {}
    virtual void NotifyFinished(Resource *resource) {}

    virtual bool IsFontResourceClient(void) const { return false; }
    virtual bool IsRawResourceClient(void) const { return false; }

    Resource* GetResource(void) const { return m_resource.Get(); }

#ifndef BLINKIT_CRAWLER_ONLY
    // Name for debugging, e.g. shown in memory-infra.
    // BKTODO: Check if necessary.
    virtual String DebugName(void) const = 0;
#endif
protected:
    ResourceClient(void) = default;

    void ClearResource(void) { SetResource(nullptr, nullptr); }
private:
    friend class ResourceFetcher;

    void SetResource(Resource *newResource, base::SingleThreadTaskRunner *taskRunner);

    Member<Resource> m_resource;
};

}  // namespace blink

#endif // BLINKIT_BLINK_RESOURCE_CLIENT_H
