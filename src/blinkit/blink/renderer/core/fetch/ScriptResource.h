// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ScriptResourceClient.h
// Description: ScriptResourceClient Class
//      Author: Ziming Li
//     Created: 2021-07-15
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
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

    This class provides all functionality needed for loading images, style sheets and html
    pages from the web. It has a memory cache for these objects.
*/

#ifndef ScriptResource_h
#define ScriptResource_h

#include "core/CoreExport.h"
// BKTODO: #include "core/fetch/IntegrityMetadata.h"
#include "core/fetch/ResourceClient.h"
#include "core/fetch/TextResource.h"

namespace blink {

enum class ScriptIntegrityDisposition {
    NotChecked = 0,
    Failed,
    Passed
};

class FetchRequest;
class ScriptResource;

class CORE_EXPORT ScriptResourceClient : public ResourceClient {
public:
    ~ScriptResourceClient() override {}
    static ResourceClientType expectedType() { return ScriptType; }
    ResourceClientType resourceClientType() const final { return expectedType(); }

    virtual void notifyAppendData(ScriptResource* resource) { }
};

class CORE_EXPORT ScriptResource final : public TextResource {
public:
    using ClientType = ScriptResourceClient;
    static ResourcePtr<ScriptResource> fetch(FetchRequest&, ResourceFetcher*);

    // Public for testing
    ScriptResource(const ResourceRequest&, const String& charset);

    ~ScriptResource() override;

    void didAddClient(ResourceClient*) override;
    void appendData(const char*, size_t) override;

    // BKTODO: void onMemoryDump(WebMemoryDumpLevelOfDetail, WebProcessMemoryDump*) const override;

    void destroyDecodedDataForFailedRevalidation() override;

    const String& script();

    AtomicString mimeType() const;

    bool mimeTypeAllowedByNosniff() const;

#if 0 // BKTODO:
    void setIntegrityMetadata(const IntegrityMetadataSet& metadata) { m_integrityMetadata = metadata; }
    const IntegrityMetadataSet& integrityMetadata() const { return m_integrityMetadata; }
#endif
    // The argument must never be |NotChecked|.
    void setIntegrityDisposition(ScriptIntegrityDisposition);
    ScriptIntegrityDisposition integrityDisposition() { return m_integrityDisposition; }
    bool mustRefetchDueToIntegrityMetadata(const FetchRequest&) const override;

private:
    class ScriptResourceFactory : public ResourceFactory {
    public:
        ScriptResourceFactory()
            : ResourceFactory(Resource::Script) { }

        Resource* create(const ResourceRequest& request, const String& charset) const override
        {
            return new ScriptResource(request, charset);
        }
    };

    ScriptIntegrityDisposition m_integrityDisposition;
    // BKTODO: IntegrityMetadataSet m_integrityMetadata;

    AtomicString m_script;
};

DEFINE_RESOURCE_TYPE_CASTS(Script);

}

#endif
