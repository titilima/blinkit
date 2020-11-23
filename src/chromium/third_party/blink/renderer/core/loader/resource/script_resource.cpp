// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_resource.cpp
// Description: ScriptResource Class
//      Author: Ziming Li
//     Created: 2020-02-03
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 1998 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2001 Dirk Mueller (mueller@kde.org)
    Copyright (C) 2002 Waldo Bastian (bastian@kde.org)
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

#include "script_resource.h"

#include "third_party/blink/renderer/platform/loader/fetch/resource_fetcher.h"

namespace blink {

ScriptResource::ScriptResourceFactory::ScriptResourceFactory(void)
    : ResourceFactory(ResourceType::kScript, TextResourceDecoderOptions::kPlainTextContent)
{
}

Resource* ScriptResource::ScriptResourceFactory::Create(
    const ResourceRequest &request,
    const ResourceLoaderOptions &options,
    const TextResourceDecoderOptions &decoderOptions) const
{
    return new ScriptResource(request, options, decoderOptions);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ScriptResource::ScriptResource(
    const ResourceRequest &resourceRequest,
    const ResourceLoaderOptions &options,
    const TextResourceDecoderOptions &decoderOptions)
    : TextResource(resourceRequest, ResourceType::kScript, options, decoderOptions)
{
}

ScriptResource::~ScriptResource(void) = default;

ScriptResource* ScriptResource::Fetch(FetchParameters &params, ResourceFetcher *fetcher, ResourceClient *client)
{
    Resource *resource = fetcher->RequestResource(params, ScriptResourceFactory(), client);
    return ToScriptResource(resource);
}

const std::string& ScriptResource::SourceText(void)
{
    ASSERT(IsLoaded());

    if (m_sourceText.empty() && Data())
    {
        String sourceText = DecodedText();
        ClearData();
        m_sourceText = sourceText.StdUtf8();
    }

    return m_sourceText;
}

}  // namespace blink
