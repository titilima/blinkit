// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: MIMETypeRegistry.cpp
// Description: MIMETypeRegistry Class
//      Author: Ziming Li
//     Created: 2021-07-31
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (c) 2008, 2009, Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "platform/MIMETypeRegistry.h"

// BKTODO: #include "platform/plugins/PluginData.h"
#include "public/platform/Platform.h"
// BKTODO: #include "public/platform/WebMimeRegistry.h"
#include "wtf/text/CString.h"

namespace blink {

namespace MIMETypes {
static const char ImageGIF[] = "image/gif";
static const char ImagePNG[] = "image/png";
static const char TextCSS[] = "text/css";
static const char TextHTML[] = "text/html";
}

String MIMETypeRegistry::getMIMETypeForExtension(const String &ext)
{
    if (0 == equalIgnoringCase(ext, "css"))
        return String::fromUTF8(MIMETypes::TextCSS);
    if (0 == equalIgnoringCase(ext, "gif"))
        return String::fromUTF8(MIMETypes::ImageGIF);
    if (0 == equalIgnoringCase(ext, "html"))
        return String::fromUTF8(MIMETypes::TextHTML);
    if (0 == equalIgnoringCase(ext, "png"))
        return String::fromUTF8(MIMETypes::ImagePNG);
#ifndef NDEBUG
    const std::string s = ext.stdUtf8();
    ASSERT(false); // BKTODO:
#endif
    return String();
}

String MIMETypeRegistry::getWellKnownMIMETypeForExtension(const String &ext)
{
    // This method must be thread safe and should not consult the OS/registry.
    ASSERT(false); // BKTODO: return Platform::current()->mimeRegistry()->wellKnownMimeTypeForExtension(ext);
    return String();
}

String MIMETypeRegistry::getMIMETypeForPath(const String& path)
{
    int pos = path.reverseFind('.');
    if (pos < 0)
        return "application/octet-stream";
    String extension = path.substring(pos + 1);
    String mimeType = getMIMETypeForExtension(extension);
    if (mimeType.isEmpty()) {
        // If there's no mimetype registered for the extension, check to see
        // if a plugin can handle the extension.
        ASSERT(false); // BKTODO: mimeType = getPluginMimeTypeFromExtension(extension);
    }
    if (mimeType.isEmpty())
        return "application/octet-stream";
    return mimeType;
}

bool MIMETypeRegistry::isSupportedImageMIMEType(const String& mimeType)
{
    ASSERT(false); // BKTODO:
    return false;
#if 0
    return Platform::current()->mimeRegistry()->supportsImageMIMEType(mimeType.lower())
        != WebMimeRegistry::IsNotSupported;
#endif
}

bool MIMETypeRegistry::isSupportedImageResourceMIMEType(const String& mimeType)
{
    return isSupportedImageMIMEType(mimeType);
}

bool MIMETypeRegistry::isSupportedImagePrefixedMIMEType(const String& mimeType)
{
    ASSERT(false); // BKTODO:
    return false;
#if 0
    return Platform::current()->mimeRegistry()->supportsImagePrefixedMIMEType(mimeType.lower())
        != WebMimeRegistry::IsNotSupported;
#endif
}

bool MIMETypeRegistry::isSupportedImageMIMETypeForEncoding(const String& mimeType)
{
    if (equalIgnoringCase(mimeType, "image/jpeg") || equalIgnoringCase(mimeType, "image/png"))
        return true;
    if (equalIgnoringCase(mimeType, "image/webp"))
        return true;
    return false;
}

bool MIMETypeRegistry::isSupportedJavaScriptMIMEType(const String& mimeType)
{
    if (equalIgnoringCase(mimeType, "text/javascript"))
        return true;
#ifndef NDEBUG
    std::string s = mimeType.stdUtf8();
#endif
    ASSERT(false); // BKTODO:
    return false;
}

bool MIMETypeRegistry::isSupportedNonImageMIMEType(const String& mimeType)
{
    ASSERT(false); // BKTODO:
    return false;
#if 0
    return Platform::current()->mimeRegistry()->supportsNonImageMIMEType(mimeType.lower())
        != WebMimeRegistry::IsNotSupported;
#endif
}

bool MIMETypeRegistry::isSupportedMediaSourceMIMEType(const String& mimeType, const String& codecs)
{
    ASSERT(false); // BKTODO:
    return false;
#if 0
    return !mimeType.isEmpty()
        && Platform::current()->mimeRegistry()->supportsMediaSourceMIMEType(mimeType.lower(), codecs);
#endif
}

bool MIMETypeRegistry::isJavaAppletMIMEType(const String& mimeType)
{
    // Since this set is very limited and is likely to remain so we won't bother with the overhead
    // of using a hash set.
    // Any of the MIME types below may be followed by any number of specific versions of the JVM,
    // which is why we use startsWith()
    return mimeType.startsWith("application/x-java-applet", TextCaseInsensitive)
        || mimeType.startsWith("application/x-java-bean", TextCaseInsensitive)
        || mimeType.startsWith("application/x-java-vm", TextCaseInsensitive);
}

bool MIMETypeRegistry::isSupportedStyleSheetMIMEType(const String& mimeType)
{
    return equalIgnoringCase(mimeType, "text/css");
}

} // namespace blink
