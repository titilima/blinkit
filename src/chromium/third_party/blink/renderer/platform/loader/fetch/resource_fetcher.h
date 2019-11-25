// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: resource_fetcher.h
// Description: ResourceFetcher Class
//      Author: Ziming Li
//     Created: 2019-10-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 1998 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2001 Dirk Mueller <mueller@kde.org>
    Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All
    rights reserved.
    Copyright (C) 2009 Torch Mobile Inc. http://www.torchmobile.com/

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

#ifndef BLINKIT_BLINK_RESOURCE_FETCHER_H
#define BLINKIT_BLINK_RESOURCE_FETCHER_H

#pragma once

#include "base/memory/ptr_util.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/noncopyable.h"

namespace blink {

class FetchContext;

class ResourceFetcher : public GarbageCollectedFinalized<ResourceFetcher>
{
    WTF_MAKE_NONCOPYABLE(ResourceFetcher);
public:
    static std::unique_ptr<ResourceFetcher> Create(std::unique_ptr<FetchContext> &context)
    {
        return base::WrapUnique(new ResourceFetcher(context));
    }
    ~ResourceFetcher(void);
private:
    ResourceFetcher(std::unique_ptr<FetchContext> &context);

    std::unique_ptr<FetchContext> m_context;
};

}  // namespace blink

#endif  // BLINKIT_BLINK_RESOURCE_FETCHER_H
