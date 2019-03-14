// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: mime_registry_impl.cpp
// Description: MimeRegistryImpl Class
//      Author: Ziming Li
//     Created: 2019-03-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "mime_registry_impl.h"

#include "public/platform/WebString.h"

using namespace blink;

namespace BlinKit {

static const std::string_view ImageGIF("image/gif");
static const std::string_view ImagePNG("image/png");
static const std::string_view TextCSS("text/css");
static const std::string_view TextHTML("text/html");
static const std::string_view TextJavascript("text/javascript");

MimeRegistryImpl::MimeRegistryImpl(void)
{
    m_extMap["gif"] = ImageGIF;
    m_extMap["png"] = ImagePNG;
    m_extMap["css"] = TextCSS;
    m_extMap["htm"] = TextHTML;
    m_extMap["html"] = TextHTML;
    m_extMap["js"] = TextJavascript;

    m_supportedMimeTypes.insert(TextHTML);
}

WebString MimeRegistryImpl::mimeTypeForExtension(const WebString &fileExtension)
{
    std::string s = fileExtension.utf8();

    auto it = m_extMap.find(s);
    if (m_extMap.end() == it)
        return WebString();
    return WebString::fromUTF8(it->second.data(), it->second.length());
}

WebMimeRegistry::SupportsType MimeRegistryImpl::supportsImageMIMEType(const WebString &mimeType)
{
    assert(false); // TODO:
    return IsNotSupported;
}

WebMimeRegistry::SupportsType MimeRegistryImpl::supportsImagePrefixedMIMEType(const WebString &mimeType)
{
    assert(false); // TODO:
    return IsNotSupported;
}

WebMimeRegistry::SupportsType MimeRegistryImpl::supportsJavaScriptMIMEType(const WebString &mimeType)
{
    if (mimeType.equals("text/javascript")
        || mimeType.equals("application/javascript")
        || mimeType.equals("application/x-javascript"))
    {
        return IsSupported;
    }

#ifdef _DEBUG
    if (mimeType.equals("text/html"))
        return IsNotSupported;

    std::string s = mimeType.utf8();
    assert(false); // BKTODO:
#endif

    return IsNotSupported;
}

WebMimeRegistry::SupportsType MimeRegistryImpl::supportsMediaMIMEType(const WebString &mimeType, const WebString &codecs, const WebString &keySystem)
{
    return IsNotSupported;
}

bool MimeRegistryImpl::supportsMediaSourceMIMEType(const WebString &mimeType, const WebString &codecs)
{
    return false;
}

WebMimeRegistry::SupportsType MimeRegistryImpl::supportsMIMEType(const WebString &mimeType)
{
    std::string s = mimeType.utf8();

    auto it = m_supportedMimeTypes.find(s);
    assert(std::end(m_supportedMimeTypes) != it);
    return std::end(m_supportedMimeTypes) != it ? IsSupported : IsNotSupported;
}

WebMimeRegistry::SupportsType MimeRegistryImpl::supportsNonImageMIMEType(const WebString &mimeType)
{
    assert(false); // TODO:
    return IsNotSupported;
}

WebString MimeRegistryImpl::wellKnownMimeTypeForExtension(const WebString &fileExtension)
{
    assert(false); // TODO:
    return WebString();
}

} // namespace BlinKit
