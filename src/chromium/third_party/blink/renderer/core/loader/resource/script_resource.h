// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_resource.h
// Description: ScriptResource Class
//      Author: Ziming Li
//     Created: 2020-02-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 1998 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2001 Dirk Mueller <mueller@kde.org>
    Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
    Copyright (C) 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights reserved.

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

#ifndef BLINKIT_BLINK_SCRIPT_RESOURCE_H
#define BLINKIT_BLINK_SCRIPT_RESOURCE_H

#pragma once

#include "third_party/blink/renderer/core/loader/resource/text_resource.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_client.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_loader_options.h"
#include "third_party/blink/renderer/platform/loader/fetch/text_resource_decoder_options.h"

namespace blink {

class FetchParameters;
class ResourceFetcher;

class ScriptResource final : public TextResource
{
public:
    static ScriptResource* Fetch(FetchParameters &params, ResourceFetcher *fetcher, ResourceClient *client);
    ~ScriptResource(void) override;

    const std::string& SourceText(void);
private:
    class ScriptResourceFactory : public ResourceFactory
    {
    public:
        ScriptResourceFactory(void);

        Resource* Create(const ResourceRequest &request, const ResourceLoaderOptions &options,
            const TextResourceDecoderOptions &decoderOptions) const override;
    };

    ScriptResource(const ResourceRequest &resourceRequest, const ResourceLoaderOptions &options,
        const TextResourceDecoderOptions &decoderOptions);

    std::string m_sourceText;
};

inline bool IsScriptResource(const Resource &resource)
{
    return resource.GetType() == ResourceType::kScript;
}

inline ScriptResource* ToScriptResource(Resource *resource)
{
    ASSERT(!resource || IsScriptResource(*resource));
    return static_cast<ScriptResource *>(resource);
}

} // namespace blink

#endif // BLINKIT_BLINK_SCRIPT_RESOURCE_H
