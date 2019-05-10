// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: frame_loader_client_impl.cpp
// Description: FrameLoaderClientImpl Class
//      Author: Ziming Li
//     Created: 2019-04-10
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "frame_loader_client_impl.h"

#include "core/loader/DocumentLoader.h"
#include "public/platform/WebApplicationCacheHost.h"
#include "app/app_constants.h"
#include "app/app_impl.h"

using namespace blink;

namespace BlinKit {

bool FrameLoaderClientImpl::allowDisplayingInsecureContent(bool enabledPerSettings, const KURL &)
{
    assert(false); // BKTODO:
    return false;
}

bool FrameLoaderClientImpl::allowImage(bool enabledPerSettings, const KURL &)
{
    assert(false); // BKTODO:
    return false;
}

bool FrameLoaderClientImpl::allowMedia(const KURL &)
{
    assert(false); // BKTODO:
    return false;
}

bool FrameLoaderClientImpl::allowPlugins(bool enabledPerSettings)
{
    assert(false); // BKTODO:
    return false;
}

bool FrameLoaderClientImpl::allowRunningInsecureContent(bool enabledPerSettings, SecurityOrigin *, const KURL &)
{
    assert(false); // BKTODO:
    return false;
}

bool FrameLoaderClientImpl::allowScript(bool enabledPerSettings)
{
    assert(false); // BKTODO:
    return false;
}

bool FrameLoaderClientImpl::allowScriptExtension(const String &extensionName, int extensionGroup, int worldId)
{
    assert(false); // BKTODO:
    return false;
}

bool FrameLoaderClientImpl::allowScriptFromSource(bool enabledPerSettings, const KURL &)
{
    assert(false); // BKTODO:
    return false;
}

bool FrameLoaderClientImpl::allowWebGL(bool enabledPerSettings)
{
    assert(false); // BKTODO:
    return false;
}

unsigned FrameLoaderClientImpl::backForwardLength(void)
{
    assert(false); // BKTODO:
    return 0;
}

bool FrameLoaderClientImpl::canCreatePluginWithoutRenderer(const String &mimeType) const
{
    assert(false); // BKTODO:
    return false;
}

WebCookieJar* FrameLoaderClientImpl::cookieJar(void) const
{
    return AppImpl::Get().cookieJar();
}

PassRefPtrWillBeRawPtr<DocumentLoader> FrameLoaderClientImpl::createDocumentLoader(LocalFrame *frame, const ResourceRequest &request, const SubstituteData &data)
{
    return DocumentLoader::create(frame, request, data);
}

NavigationPolicy FrameLoaderClientImpl::decidePolicyForNavigation(const ResourceRequest &, DocumentLoader *, NavigationType, NavigationPolicy, bool shouldReplaceCurrentEntry)
{
    return NavigationPolicyCurrentTab;
}

void FrameLoaderClientImpl::didAccessInitialDocument(void)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didChangeName(const String&)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didChangePerformanceTiming(void)
{
    assert(false); // BKTODO: Remove DocumentTiming.
}

void FrameLoaderClientImpl::didChangeScrollOffset(void)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didDetectXSS(const KURL &, bool didBlockEntirePage)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didDispatchPingLoader(const KURL &)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didDisplayContentWithCertificateErrors(const KURL &, const CString &securityInfo, const WebURL &mainResourceUrl, const CString &mainResourceSecurityInfo)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didDisplayInsecureContent(void)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didEnforceStrictMixedContentChecking(void)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didLoseWebGLContext(int)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didNotAllowPlugins(void)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didNotAllowScript(void)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didRemoveAllPendingStylesheet(void)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didRequestAutocomplete(HTMLFormElement *)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didRunContentWithCertificateErrors(const KURL &, const CString &securityInfo, const WebURL &mainResourceUrl, const CString &mainResourceSecurityInfo)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didRunInsecureContent(SecurityOrigin *, const KURL &)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didStartLoading(LoadStartType)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didStopLoading(void)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didUpdateCurrentHistoryItem(void)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::didUseKeygen(void)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::dispatchDidChangeIcons(IconType)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::dispatchDidChangeManifest(void)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::dispatchDidChangeResourcePriority(unsigned long identifier, ResourceLoadPriority, int intraPriorityValue)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::dispatchDidChangeThemeColor(void)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::dispatchDidFailLoad(const ResourceError &, HistoryCommitType)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::dispatchDidFailProvisionalLoad(const ResourceError &, HistoryCommitType)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::dispatchDidLoadResourceFromMemoryCache(const ResourceRequest &, const ResourceResponse &)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::dispatchDidNavigateWithinPage(HistoryItem*, HistoryCommitType)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::dispatchDidReceiveServerRedirectForProvisionalLoad(void)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::dispatchDidReceiveTitle(const String &)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::dispatchWillOpenWebSocket(WebSocketHandle *)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::dispatchWillSendSubmitEvent(HTMLFormElement *)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::dispatchWillStartUsingPeerConnectionHandler(WebRTCPeerConnectionHandler *)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::dispatchWillSubmitForm(HTMLFormElement *)
{
    assert(false); // BKTODO:
}

String FrameLoaderClientImpl::doNotTrackValue(void)
{
    assert(false); // BKTODO:
    return emptyString();
}

void FrameLoaderClientImpl::frameFocused(void) const
{
    assert(false); // BKTODO:
}

bool FrameLoaderClientImpl::hasPendingNavigation(void)
{
    assert(false); // BKTODO:
    return false;
}

bool FrameLoaderClientImpl::inShadowTree(void) const
{
    assert(false); // BKTODO:
    return false;
}

void FrameLoaderClientImpl::loadURLExternally(const ResourceRequest &, NavigationPolicy, const String &suggestedName, bool replacesCurrentHistoryItem)
{
    assert(false); // BKTODO:
}

bool FrameLoaderClientImpl::navigateBackForward(int offset) const
{
    assert(false); // BKTODO:
    return false;
}

ObjectContentType FrameLoaderClientImpl::objectContentType(const KURL &, const String & mimeType, bool shouldPreferPlugInsForImages)
{
    assert(false); // BKTODO:
    return ObjectContentNone;
}

Frame* FrameLoaderClientImpl::opener(void) const
{
    assert(false); // BKTODO:
    return nullptr;
}

void FrameLoaderClientImpl::progressEstimateChanged(double progressEstimate)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::selectorMatchChanged(const Vector<String> &addedSelectors, const Vector<String> &removedSelectors)
{
    assert(false); // BKTODO:
}

int64_t FrameLoaderClientImpl::serviceWorkerID(DocumentLoader&)
{
    return 0;
}

void FrameLoaderClientImpl::suddenTerminationDisablerChanged(bool present, SuddenTerminationDisablerType)
{
    assert(false); // BKTODO:
}

void FrameLoaderClientImpl::transitionToCommittedForNewPage(void)
{
    // Nothing to do for crawlers.
}

String FrameLoaderClientImpl::userAgent(void)
{
    return String::fromUTF8(AppConstants::DefaultUserAgent);
}

} // namespace BlinKit
