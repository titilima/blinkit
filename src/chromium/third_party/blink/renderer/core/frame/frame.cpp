// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: frame.cpp
// Description: Frame Class
//      Author: Ziming Li
//     Created: 2019-09-12
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
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011 Apple Inc. All
 * rights reserved.
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

#include "frame.h"

#include "third_party/blink/renderer/core/frame/dom_window.h"
#include "third_party/blink/renderer/core/frame/frame_client.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/loader/empty_clients.h"
#   include "third_party/blink/renderer/core/page/page.h"
#endif

namespace blink {

#ifdef BLINKIT_CRAWLER_ONLY
Frame::Frame(FrameClient *client) : m_client(client) {}
#else
Frame::Frame(FrameClient *client, Page *page)
    : m_client(client)
    , m_page(page)
{
    if (m_page)
        m_page->SetMainFrame(this);
}
#endif

Frame::~Frame(void) = default;

void Frame::Detach(FrameDetachType type)
{
    // Detach() can be re-entered, so this can't simply DCHECK(IsAttached()).
    ASSERT(!IsDetached());
    m_lifecycle.AdvanceTo(FrameLifecycle::kDetaching);

    DetachImpl(type);
    // Due to re-entrancy, |this| could have completed detaching already.
    // TODO(dcheng): This DCHECK is not always true. See https://crbug.com/838348.
    ASSERT(IsDetached() == (nullptr == m_client));
    if (nullptr == m_client)
        return;

#if 0 // BKTODO:
    detach_stack_ = base::debug::StackTrace();
    m_client->SetOpener(nullptr);
#endif
    // After this, we must no longer talk to the client since this clears
    // its owning reference back to our owning LocalFrame.
    m_client->Detached(type);
    m_client = nullptr;
    // Mark the frame as detached once |client_| is null, as most of the frame has
    // been torn down at this point.
    // TODO(dcheng): Once https://crbug.com/820782 is fixed, Frame::Client() will
    // also assert that it is only accessed when the frame is not detached.
    m_lifecycle.AdvanceTo(FrameLifecycle::kDetached);
#ifndef BLINKIT_CRAWLER_ONLY
    m_page = nullptr;
#endif
}

void Frame::Trace(Visitor *visitor)
{
    visitor->Trace(m_domWindow);
}

#ifndef BLINKIT_CRAWLER_ONLY
ChromeClient& Frame::GetChromeClient(void) const
{
    if (Page *page = GetPage())
        return page->GetChromeClient();

    static EmptyChromeClient s_emptyClient;
    return s_emptyClient;
}

Page* Frame::GetPage(void) const
{
    return m_page;
}
#endif // BLINKIT_CRAWLER_ONLY

} // namespace blink
