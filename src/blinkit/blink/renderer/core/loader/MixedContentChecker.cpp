// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: MixedContentChecker.cpp
// Description: MixedContentChecker Class
//      Author: Ziming Li
//     Created: 2021-07-31
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2012 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "./MixedContentChecker.h"

#include "blinkit/blink/public/platform/Platform.h"
#include "blinkit/blink/renderer/core/dom/document.h"
#include "blinkit/blink/renderer/core/frame/LocalFrame.h"
#include "blinkit/blink/renderer/core/frame/Settings.h"
#include "blinkit/blink/renderer/core/frame/UseCounter.h"
// BKTODO: #include "core/inspector/ConsoleMessage.h"
#include "blinkit/blink/renderer/core/loader/DocumentLoader.h"
#include "blinkit/blink/renderer/core/loader/FrameLoader.h"
#include "blinkit/blink/renderer/core/loader/frame_loader_client.h"
#include "blinkit/blink/renderer/platform/RuntimeEnabledFeatures.h"
// BKTODO: #include "platform/weborigin/SchemeRegistry.h"
// BKTODO: #include "platform/weborigin/SecurityOrigin.h"
#include "blinkit/blink/renderer/wtf/text/StringBuilder.h"

namespace blink {

static void measureStricterVersionOfIsMixedContent(LocalFrame* frame, const KURL& url)
{
    ASSERT(false); // BKTODO:
#if 0
    // We're currently only checking for mixed content in `https://*` contexts.
    // What about other "secure" contexts the SchemeRegistry knows about? We'll
    // use this method to measure the occurance of non-webby mixed content to
    // make sure we're not breaking the world without realizing it.
    SecurityOrigin* origin = frame->document()->securityOrigin();
    if (MixedContentChecker::isMixedContent(origin, url)) {
        if (frame->document()->securityOrigin()->protocol() != "https")
            UseCounter::count(frame, UseCounter::MixedContentInNonHTTPSFrameThatRestrictsMixedContent);
    } else if (!SecurityOrigin::isSecure(url) && SchemeRegistry::shouldTreatURLSchemeAsSecure(origin->protocol())) {
        UseCounter::count(frame, UseCounter::MixedContentInSecureFrameThatDoesNotRestrictMixedContent);
    }
#endif
}

bool requestIsSubframeSubresource(LocalFrame* frame, WebURLRequest::FrameType frameType)
{
    ASSERT(false); // BKTODO: return (frame && frame != frame->tree().top() && frameType != WebURLRequest::FrameTypeNested);
    return false;
}

#if 0 // BKTODO:
// static
bool MixedContentChecker::isMixedContent(SecurityOrigin* securityOrigin, const KURL& url)
{
    if (!SchemeRegistry::shouldTreatURLSchemeAsRestrictingMixedContent(securityOrigin->protocol()))
        return false;

    // We're in a secure context, so |url| is mixed content if it's insecure.
    return !SecurityOrigin::isSecure(url);
}
#endif

// static
LocalFrame* MixedContentChecker::inWhichFrameIsContentMixed(LocalFrame* frame, WebURLRequest::FrameType frameType, const KURL& url)
{
    // We only care about subresource loads; top-level navigations cannot be mixed content. Neither can frameless requests.
    if (frameType == WebURLRequest::FrameTypeTopLevel || !frame)
        return nullptr;

    ASSERT(false); // BKTODO:
#if 0
    // Check the top frame first.
    if (Frame* top = frame->tree().top()) {
        // FIXME: We need a way to access the top-level frame's SecurityOrigin when that frame
        // is in a different process from the current frame. Until that is done, we bail out.
        if (!top->isLocalFrame())
            return nullptr;

        LocalFrame* localTop = toLocalFrame(top);
        measureStricterVersionOfIsMixedContent(localTop, url);
        if (isMixedContent(localTop->document()->securityOrigin(), url))
            return localTop;
    }

    measureStricterVersionOfIsMixedContent(frame, url);
    if (isMixedContent(frame->document()->securityOrigin(), url))
        return frame;
#endif

    // No mixed content, no problem.
    return nullptr;
}

// static
MixedContentChecker::ContextType MixedContentChecker::contextTypeFromContext(WebURLRequest::RequestContext context, LocalFrame* frame)
{
    switch (context) {
    // "Optionally-blockable" mixed content
    case WebURLRequest::RequestContextAudio:
    case WebURLRequest::RequestContextFavicon:
    case WebURLRequest::RequestContextImage:
    case WebURLRequest::RequestContextVideo:
        return ContextTypeOptionallyBlockable;

#if 0 // BKTODO:
    // Plugins! Oh how dearly we love plugin-loaded content!
    case WebURLRequest::RequestContextPlugin: {
        Settings* settings = frame->settings();
        return settings && settings->strictMixedContentCheckingForPlugin() ? ContextTypeBlockable : ContextTypeOptionallyBlockable;
    }
#endif

    // "Blockable" mixed content
    case WebURLRequest::RequestContextBeacon:
    case WebURLRequest::RequestContextCSPReport:
    case WebURLRequest::RequestContextEmbed:
    case WebURLRequest::RequestContextEventSource:
    case WebURLRequest::RequestContextFetch:
    case WebURLRequest::RequestContextFont:
    case WebURLRequest::RequestContextForm:
    case WebURLRequest::RequestContextFrame:
    case WebURLRequest::RequestContextHyperlink:
    case WebURLRequest::RequestContextIframe:
    case WebURLRequest::RequestContextImageSet:
    case WebURLRequest::RequestContextImport:
    case WebURLRequest::RequestContextLocation:
    case WebURLRequest::RequestContextManifest:
    case WebURLRequest::RequestContextObject:
    case WebURLRequest::RequestContextPing:
    case WebURLRequest::RequestContextScript:
    case WebURLRequest::RequestContextServiceWorker:
    case WebURLRequest::RequestContextSharedWorker:
    case WebURLRequest::RequestContextStyle:
    case WebURLRequest::RequestContextSubresource:
    case WebURLRequest::RequestContextTrack:
    case WebURLRequest::RequestContextWorker:
    case WebURLRequest::RequestContextXMLHttpRequest:
    case WebURLRequest::RequestContextXSLT:
        return ContextTypeBlockable;

    // FIXME: Contexts that we should block, but don't currently. https://crbug.com/388650
    case WebURLRequest::RequestContextDownload:
    case WebURLRequest::RequestContextInternal:
    case WebURLRequest::RequestContextPrefetch:
        return ContextTypeShouldBeBlockable;

    case WebURLRequest::RequestContextUnspecified:
        ASSERT_NOT_REACHED();
    }
    ASSERT_NOT_REACHED();
    return ContextTypeBlockable;
}

// static
const char* MixedContentChecker::typeNameFromContext(WebURLRequest::RequestContext context)
{
    switch (context) {
    case WebURLRequest::RequestContextAudio:
        return "audio file";
    case WebURLRequest::RequestContextBeacon:
        return "Beacon endpoint";
    case WebURLRequest::RequestContextCSPReport:
        return "Content Security Policy reporting endpoint";
    case WebURLRequest::RequestContextDownload:
        return "download";
    case WebURLRequest::RequestContextEmbed:
        return "plugin resource";
    case WebURLRequest::RequestContextEventSource:
        return "EventSource endpoint";
    case WebURLRequest::RequestContextFavicon:
        return "favicon";
    case WebURLRequest::RequestContextFetch:
        return "resource";
    case WebURLRequest::RequestContextFont:
        return "font";
    case WebURLRequest::RequestContextForm:
        return "form action";
    case WebURLRequest::RequestContextFrame:
        return "frame";
    case WebURLRequest::RequestContextHyperlink:
        return "resource";
    case WebURLRequest::RequestContextIframe:
        return "frame";
    case WebURLRequest::RequestContextImage:
        return "image";
    case WebURLRequest::RequestContextImageSet:
        return "image";
    case WebURLRequest::RequestContextImport:
        return "HTML Import";
    case WebURLRequest::RequestContextInternal:
        return "resource";
    case WebURLRequest::RequestContextLocation:
        return "resource";
    case WebURLRequest::RequestContextManifest:
        return "manifest";
    case WebURLRequest::RequestContextObject:
        return "plugin resource";
    case WebURLRequest::RequestContextPing:
        return "hyperlink auditing endpoint";
    case WebURLRequest::RequestContextPlugin:
        return "plugin data";
    case WebURLRequest::RequestContextPrefetch:
        return "prefetch resource";
    case WebURLRequest::RequestContextScript:
        return "script";
    case WebURLRequest::RequestContextServiceWorker:
        return "Service Worker script";
    case WebURLRequest::RequestContextSharedWorker:
        return "Shared Worker script";
    case WebURLRequest::RequestContextStyle:
        return "stylesheet";
    case WebURLRequest::RequestContextSubresource:
        return "resource";
    case WebURLRequest::RequestContextTrack:
        return "Text Track";
    case WebURLRequest::RequestContextUnspecified:
        return "resource";
    case WebURLRequest::RequestContextVideo:
        return "video";
    case WebURLRequest::RequestContextWorker:
        return "Worker script";
    case WebURLRequest::RequestContextXMLHttpRequest:
        return "XMLHttpRequest endpoint";
    case WebURLRequest::RequestContextXSLT:
        return "XSLT";
    }
    ASSERT_NOT_REACHED();
    return "resource";
}

// static
void MixedContentChecker::logToConsoleAboutFetch(LocalFrame* frame, const KURL& url, WebURLRequest::RequestContext requestContext, bool allowed)
{
    ASSERT(false); // BKTODO:
#if 0
    String message = String::format(
        "Mixed Content: The page at '%s' was loaded over HTTPS, but requested an insecure %s '%s'. %s",
        frame->document()->url().elidedString().utf8().data(), typeNameFromContext(requestContext), url.elidedString().utf8().data(),
        allowed ? "This content should also be served over HTTPS." : "This request has been blocked; the content must be served over HTTPS.");
    MessageLevel messageLevel = allowed ? WarningMessageLevel : ErrorMessageLevel;
    frame->document()->addConsoleMessage(ConsoleMessage::create(SecurityMessageSource, messageLevel, message));
#endif
}

// static
void MixedContentChecker::count(LocalFrame* frame, WebURLRequest::RequestContext requestContext)
{
    ASSERT(false); // BKTODO:
#if 0
    UseCounter::count(frame, UseCounter::MixedContentPresent);

    // Roll blockable content up into a single counter, count unblocked types individually so we
    // can determine when they can be safely moved to the blockable category:
    ContextType contextType = contextTypeFromContext(requestContext, frame);
    if (contextType == ContextTypeBlockable) {
        UseCounter::count(frame, UseCounter::MixedContentBlockable);
        return;
    }

    UseCounter::Feature feature;
    switch (requestContext) {
    case WebURLRequest::RequestContextAudio:
        feature = UseCounter::MixedContentAudio;
        break;
    case WebURLRequest::RequestContextDownload:
        feature = UseCounter::MixedContentDownload;
        break;
    case WebURLRequest::RequestContextFavicon:
        feature = UseCounter::MixedContentFavicon;
        break;
    case WebURLRequest::RequestContextImage:
        feature = UseCounter::MixedContentImage;
        break;
    case WebURLRequest::RequestContextInternal:
        feature = UseCounter::MixedContentInternal;
        break;
    case WebURLRequest::RequestContextPlugin:
        feature = UseCounter::MixedContentPlugin;
        break;
    case WebURLRequest::RequestContextPrefetch:
        feature = UseCounter::MixedContentPrefetch;
        break;
    case WebURLRequest::RequestContextVideo:
        feature = UseCounter::MixedContentVideo;
        break;

    default:
        ASSERT_NOT_REACHED();
        return;
    }
    UseCounter::count(frame, feature);
#endif
}

// static
bool MixedContentChecker::shouldBlockFetch(LocalFrame* frame, WebURLRequest::RequestContext requestContext, WebURLRequest::FrameType frameType, const KURL& url, MixedContentChecker::ReportingStatus reportingStatus)
{
    LocalFrame* mixedFrame = inWhichFrameIsContentMixed(frame, frameType, url);
    if (!mixedFrame)
        return false;

    ASSERT(false); // BKTODO:
    return false;
#if 0
    MixedContentChecker::count(mixedFrame, requestContext);

    Settings* settings = mixedFrame->settings();
    FrameLoaderClient* client = mixedFrame->loader().client();
    SecurityOrigin* securityOrigin = mixedFrame->document()->securityOrigin();
    bool allowed = false;

    // If we're in strict mode, we'll automagically fail everything, and intentionally skip
    // the client checks in order to prevent degrading the site's security UI.
    bool strictMode = mixedFrame->securityContext()->shouldEnforceStrictMixedContentChecking() || settings->strictMixedContentChecking();

    ContextType contextType = contextTypeFromContext(requestContext, mixedFrame);

    // If we're loading the main resource of a subframe, we need to take a close look at the loaded URL.
    // If we're dealing with a CORS-enabled scheme, then block mixed frames as active content. Otherwise,
    // treat frames as passive content.
    //
    // FIXME: Remove this temporary hack once we have a reasonable API for launching external applications
    // via URLs. http://crbug.com/318788 and https://crbug.com/393481
    if (frameType == WebURLRequest::FrameTypeNested && !SchemeRegistry::shouldTreatURLSchemeAsCORSEnabled(url.protocol()))
        contextType = ContextTypeOptionallyBlockable;

    switch (contextType) {
    case ContextTypeOptionallyBlockable:
        allowed = !strictMode && client->allowDisplayingInsecureContent(settings && settings->allowDisplayOfInsecureContent(), url);
        if (allowed)
            client->didDisplayInsecureContent();
        break;

    case ContextTypeBlockable: {
        // Strictly block subresources in subframes, unless all insecure
        // content is allowed.
        if (!settings->allowRunningOfInsecureContent() && requestIsSubframeSubresource(frame, frameType)) {
            UseCounter::count(mixedFrame, UseCounter::BlockableMixedContentInSubframeBlocked);
            allowed = false;
            break;
        }

        bool shouldAskEmbedder = !strictMode && settings && (!settings->strictlyBlockBlockableMixedContent() || settings->allowRunningOfInsecureContent());
        allowed = shouldAskEmbedder && client->allowRunningInsecureContent(settings && settings->allowRunningOfInsecureContent(), securityOrigin, url);
        if (allowed) {
            client->didRunInsecureContent(securityOrigin, url);
            UseCounter::count(mixedFrame, UseCounter::MixedContentBlockableAllowed);
        }
        break;
    }

    case ContextTypeShouldBeBlockable:
        allowed = !strictMode;
        if (allowed)
            client->didDisplayInsecureContent();
        break;
    case ContextTypeNotMixedContent:
        ASSERT_NOT_REACHED();
        break;
    };

    if (reportingStatus == SendReport)
        logToConsoleAboutFetch(frame, url, requestContext, allowed);
    return !allowed;
#endif
}

// static
void MixedContentChecker::logToConsoleAboutWebSocket(LocalFrame* frame, const KURL& url, bool allowed)
{
    ASSERT(false); // BKTODO:
#if 0
    String message = String::format(
        "Mixed Content: The page at '%s' was loaded over HTTPS, but attempted to connect to the insecure WebSocket endpoint '%s'. %s",
        frame->document()->url().elidedString().utf8().data(), url.elidedString().utf8().data(),
        allowed ? "This endpoint should be available via WSS. Insecure access is deprecated." : "This request has been blocked; this endpoint must be available over WSS.");
    MessageLevel messageLevel = allowed ? WarningMessageLevel : ErrorMessageLevel;
    frame->document()->addConsoleMessage(ConsoleMessage::create(SecurityMessageSource, messageLevel, message));
#endif
}

// static
bool MixedContentChecker::shouldBlockWebSocket(LocalFrame* frame, const KURL& url, MixedContentChecker::ReportingStatus reportingStatus)
{
    LocalFrame* mixedFrame = inWhichFrameIsContentMixed(frame, WebURLRequest::FrameTypeNone, url);
    if (!mixedFrame)
        return false;

    ASSERT(false); // BKTODO:
    return false;
#if 0
    UseCounter::count(mixedFrame, UseCounter::MixedContentPresent);
    UseCounter::count(mixedFrame, UseCounter::MixedContentWebSocket);

    Settings* settings = mixedFrame->settings();
    FrameLoaderClient* client = mixedFrame->loader().client();
    SecurityOrigin* securityOrigin = mixedFrame->document()->securityOrigin();
    bool allowed = false;

    // If we're in strict mode, we'll automagically fail everything, and intentionally skip
    // the client checks in order to prevent degrading the site's security UI.
    bool strictMode = mixedFrame->document()->shouldEnforceStrictMixedContentChecking() || settings->strictMixedContentChecking();
    if (!strictMode) {
        bool allowedPerSettings = settings && settings->allowRunningOfInsecureContent();
        allowed = client->allowRunningInsecureContent(allowedPerSettings, securityOrigin, url);
    }

    if (allowed)
        client->didRunInsecureContent(securityOrigin, url);

    if (reportingStatus == SendReport)
        logToConsoleAboutWebSocket(frame, url, allowed);
    return !allowed;
#endif
}

bool MixedContentChecker::isMixedFormAction(LocalFrame* frame, const KURL& url, ReportingStatus reportingStatus)
{
    // For whatever reason, some folks handle forms via JavaScript, and submit to `javascript:void(0)`
    // rather than calling `preventDefault()`. We special-case `javascript:` URLs here, as they don't
    // introduce MixedContent for form submissions.
    if (url.protocolIs("javascript"))
        return false;

    LocalFrame* mixedFrame = inWhichFrameIsContentMixed(frame, WebURLRequest::FrameTypeNone, url);
    if (!mixedFrame)
        return false;

    ASSERT(false); // BKTODO:
#if 0
    UseCounter::count(mixedFrame, UseCounter::MixedContentPresent);

    mixedFrame->loader().client()->didDisplayInsecureContent();

    if (reportingStatus == SendReport) {
        String message = String::format(
            "Mixed Content: The page at '%s' was loaded over a secure connection, but contains a form which targets an insecure endpoint '%s'. This endpoint should be made available over a secure connection.",
            frame->document()->url().elidedString().utf8().data(), url.elidedString().utf8().data());
        mixedFrame->document()->addConsoleMessage(ConsoleMessage::create(SecurityMessageSource, WarningMessageLevel, message));
    }
#endif

    return true;
}

void MixedContentChecker::checkMixedPrivatePublic(LocalFrame* frame, const AtomicString& resourceIPAddress)
{
    if (!frame || !frame->document() || !frame->document()->loader())
        return;

#if 0 // BKTODO:
    // Just count these for the moment, don't block them.
    if (Platform::current()->isReservedIPAddress(resourceIPAddress) && !frame->document()->isHostedInReservedIPRange())
        UseCounter::count(frame->document(), UseCounter::MixedContentPrivateHostnameInPublicHostname);
#endif
}

LocalFrame* MixedContentChecker::effectiveFrameForFrameType(LocalFrame* frame, WebURLRequest::FrameType frameType)
{
    // If we're loading the main resource of a subframe, ensure that we check
    // against the parent of the active frame, rather than the frame itself.
    LocalFrame* effectiveFrame = frame;
    if (frameType == WebURLRequest::FrameTypeNested) {
        ASSERT(false); // BKTODO:
#if 0
        // FIXME: Deal with RemoteFrames.
        Frame* parentFrame = effectiveFrame->tree().parent();
        ASSERT(parentFrame);
        if (parentFrame->isLocalFrame())
            effectiveFrame = toLocalFrame(parentFrame);
#endif
    }
    return effectiveFrame;
}

void MixedContentChecker::handleCertificateError(LocalFrame* frame, const ResourceRequest& request, const ResourceResponse& response)
{
    ASSERT(false); // BKTODO:
#if 0
    WebURLRequest::FrameType frameType = request.frameType();
    LocalFrame* effectiveFrame = effectiveFrameForFrameType(frame, frameType);
    if (frameType == WebURLRequest::FrameTypeTopLevel || !effectiveFrame)
        return;

    FrameLoaderClient* client = effectiveFrame->loader().client();
    WebURLRequest::RequestContext requestContext = request.requestContext();
    ContextType contextType = MixedContentChecker::contextTypeFromContext(requestContext, frame);
    if (contextType == ContextTypeBlockable) {
        client->didRunContentWithCertificateErrors(response.url(), response.getSecurityInfo(), effectiveFrame->document()->url(), effectiveFrame->loader().documentLoader()->response().getSecurityInfo());
    } else {
        // contextTypeFromContext() never returns NotMixedContent (it
        // computes the type of mixed content, given that the content is
        // mixed).
        ASSERT(contextType != ContextTypeNotMixedContent);
        client->didDisplayContentWithCertificateErrors(response.url(), response.getSecurityInfo(), effectiveFrame->document()->url(), effectiveFrame->loader().documentLoader()->response().getSecurityInfo());
    }
#endif
}

MixedContentChecker::ContextType MixedContentChecker::contextTypeForInspector(LocalFrame* frame, const ResourceRequest& request)
{
    ASSERT(false); // BKTODO:
    exit(0);
#if 0
    LocalFrame* effectiveFrame = effectiveFrameForFrameType(frame, request.frameType());

    LocalFrame* mixedFrame = inWhichFrameIsContentMixed(effectiveFrame, request.frameType(), request.url());
    if (!mixedFrame)
        return ContextTypeNotMixedContent;

    // See comment in shouldBlockFetch() about loading the main resource of a subframe.
    if (request.frameType() == WebURLRequest::FrameTypeNested && !SchemeRegistry::shouldTreatURLSchemeAsCORSEnabled(request.url().protocol())) {
        return ContextTypeOptionallyBlockable;
    }

    return contextTypeFromContext(request.requestContext(), mixedFrame);
#endif
}

} // namespace blink
