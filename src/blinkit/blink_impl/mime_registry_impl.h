// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: mime_registry_impl.h
// Description: MimeRegistryImpl Class
//      Author: Ziming Li
//     Created: 2019-03-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_MIME_REGISTRY_IMPL_H
#define BLINKIT_BLINKIT_MIME_REGISTRY_IMPL_H

#pragma once

#include <string_view>
#include <unordered_set>
#include "public/platform/WebMimeRegistry.h"

namespace BlinKit {

class MimeRegistryImpl final : public blink::WebMimeRegistry
{
public:
    MimeRegistryImpl(void);
private:
    // blink::WebMimeRegistry
    SupportsType supportsMIMEType(const blink::WebString &mimeType) override final;
    SupportsType supportsImageMIMEType(const blink::WebString &mimeType) override final;
    SupportsType supportsImagePrefixedMIMEType(const blink::WebString &mimeType) override final;
    SupportsType supportsJavaScriptMIMEType(const blink::WebString &mimeType) override final;
    SupportsType supportsMediaMIMEType(const blink::WebString &mimeType, const blink::WebString &codecs, const blink::WebString &keySystem) override final;
    bool supportsMediaSourceMIMEType(const blink::WebString &mimeType, const blink::WebString &codecs) override final;
    SupportsType supportsNonImageMIMEType(const blink::WebString &mimeType) override final;
    blink::WebString mimeTypeForExtension(const blink::WebString &fileExtension) override final;
    blink::WebString wellKnownMimeTypeForExtension(const blink::WebString &fileExtension) override final;

    std::unordered_map<std::string, std::string_view> m_extMap;
    std::unordered_set<std::string_view> m_supportedMimeTypes;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_MIME_REGISTRY_IMPL_H
