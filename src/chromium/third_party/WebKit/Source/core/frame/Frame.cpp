// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: Frame.cpp
// Description: Frame Class
//      Author: Ziming Li
//     Created: 2019-02-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1998, 1999 Torben Weis <weis@kde.org>
 *                     1999 Lars Knoll <knoll@kde.org>
 *                     1999 Antti Koivisto <koivisto@kde.org>
 *                     2000 Simon Hausmann <hausmann@kde.org>
 *                     2000 Stefan Schimanski <1Stein@gmx.de>
 *                     2001 George Staikos <staikos@kde.org>
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All rights reserved.
 * Copyright (C) 2005 Alexey Proskuryakov <ap@nypop.com>
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2008 Eric Seidel <eric@webkit.org>
 * Copyright (C) 2008 Google Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "core/frame/Frame.h"

#include "core/dom/DocumentType.h"
#include "core/events/Event.h"
#include "core/frame/LocalDOMWindow.h"
#include "core/frame/FrameHost.h"
#include "core/frame/Settings.h"
#include "core/input/EventHandler.h"
#include "core/inspector/InspectorInstrumentation.h"
#include "core/inspector/InstanceCounters.h"
#include "core/loader/EmptyClients.h"
#include "core/loader/FrameLoaderClient.h"
//#include "core/page/FocusController.h"
#include "core/page/Page.h"
#include "wtf/PassOwnPtr.h"
#include "wtf/RefCountedLeakCounter.h"

#include "blinkit/app/app_impl.h"

namespace blink {

using namespace HTMLNames;

namespace {

int64_t generateFrameID()
{
    // Initialize to the current time to reduce the likelihood of generating
    // identifiers that overlap with those from past/future browser sessions.
    static int64_t next = static_cast<int64_t>(currentTime() * 1000000.0);
    return ++next;
}

#ifndef NDEBUG
WTF::RefCountedLeakCounter& frameCounter()
{
    DEFINE_STATIC_LOCAL(WTF::RefCountedLeakCounter, staticFrameCounter, ("Frame"));
    return staticFrameCounter;
}
#endif

} // namespace

Frame::~Frame()
{
    InstanceCounters::decrementCounter(InstanceCounters::FrameCounter);
#ifndef NDEBUG
    frameCounter().decrement();
#endif
}

DEFINE_TRACE(Frame)
{
#ifndef BLINKIT_CRAWLER_ONLY
    visitor->trace(m_host);
#endif
    visitor->trace(m_client);
}

void Frame::detach(FrameDetachType type)
{
    ASSERT(m_client);
    m_client->setOpener(0);
    domWindow()->resetLocation();
    // After this, we must no longer talk to the client since this clears
    // its owning reference back to our owning LocalFrame.
    m_client->detached(type);
    m_client = nullptr;
#ifndef BLINKIT_CRAWLER_ONLY
    m_host = nullptr;
#endif
}

Page* Frame::page() const
{
    ASSERT(!m_client->IsCrawler());
#ifndef BLINKIT_CRAWLER_ONLY
    if (m_host)
        return &m_host->page();
#endif
    return nullptr;
}

FrameHost* Frame::host() const
{
#ifdef BLINKIT_CRAWLER_ONLY
    return nullptr;
#else
    return m_host;
#endif
}

bool Frame::isMainFrame() const
{
    return true;
}

bool Frame::isLocalRoot() const
{
    return true;
}

#ifndef BLINKIT_CRAWLER_ONLY
static ChromeClient& emptyChromeClient()
{
    DEFINE_STATIC_LOCAL(OwnPtrWillBePersistent<EmptyChromeClient>, client, (EmptyChromeClient::create()));
    return *client;
}

ChromeClient& Frame::chromeClient() const
{
    if (Page* page = this->page())
        return page->chromeClient();
    return emptyChromeClient();
}
#endif

bool Frame::canNavigate(const Frame& targetFrame)
{
    if (&targetFrame == this)
        return true;

    printNavigationErrorMessage(targetFrame, "The frame attempting navigation is neither same-origin with the target, nor is it the target's parent or opener.");
    return false;
}

Settings* Frame::settings() const
{
#ifndef BLINKIT_CRAWLER_ONLY
    if (m_host)
        return &m_host->settings();
#endif
    assert(m_client->IsCrawler());
    return &(BlinKit::AppImpl::Get().CrawlerSettings());
}

Frame::Frame(FrameClient* client, FrameHost* host)
    : m_client(client)
#ifndef BLINKIT_CRAWLER_ONLY
    , m_host(host)
#endif
    , m_frameID(generateFrameID())
    , m_isLoading(false)
{
    InstanceCounters::incrementCounter(InstanceCounters::FrameCounter);

    ASSERT(client->IsCrawler() || nullptr != page());

#ifndef NDEBUG
    frameCounter().increment();
#endif

#ifndef BLINKIT_CRAWLER_ONLY
    if (!client->IsCrawler())
        page()->setMainFrame(this);
#endif
}

bool Frame::IsCrawlerFrame(void) const
{
    return m_client->IsCrawler();
}

} // namespace blink
