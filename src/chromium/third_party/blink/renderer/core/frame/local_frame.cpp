// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: local_frame.cpp
// Description: LocalFrame Class
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

#include "local_frame.h"

#include "third_party/blink/renderer/core/frame/local_dom_window.h"
#include "third_party/blink/renderer/core/frame/local_frame_client.h"

namespace blink {

LocalFrame::LocalFrame(LocalFrameClient &client, Page *page) : Frame(client, page), m_loader(this)
{
}

std::unique_ptr<LocalFrame> LocalFrame::Create(LocalFrameClient &client, Page *page)
{
    return std::unique_ptr<LocalFrame>(new LocalFrame(client, page));
}

LocalDOMWindow* LocalFrame::DomWindow(void) const
{
    DOMWindow *domWindow = Frame::DomWindow();
    return ToLocalDOMWindow(domWindow);
}

Document* LocalFrame::GetDocument(void) const
{
    LocalDOMWindow *domWindow = DomWindow();
    return nullptr != domWindow ? domWindow->document() : nullptr;
}

} // namespace blink
