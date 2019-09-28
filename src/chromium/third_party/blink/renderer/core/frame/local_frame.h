// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: local_frame.h
// Description: LocalFrame Class
//      Author: Ziming Li
//     Created: 2019-09-12
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1998, 1999 Torben Weis <weis@kde.org>
 *                     1999-2001 Lars Knoll <knoll@kde.org>
 *                     1999-2001 Antti Koivisto <koivisto@kde.org>
 *                     2000-2001 Simon Hausmann <hausmann@kde.org>
 *                     2000-2001 Dirk Mueller <mueller@kde.org>
 *                     2000 Stefan Schimanski <1Stein@gmx.de>
 * Copyright (C) 2004, 2005, 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights
 * reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2008 Eric Seidel <eric@webkit.org>
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

#ifndef BLINKIT_BLINK_LOCAL_FRAME_H
#define BLINKIT_BLINK_LOCAL_FRAME_H

#pragma once

#include "third_party/blink/renderer/core/frame/frame.h"
#include "third_party/blink/renderer/core/loader/frame_loader.h"

namespace blink {

class Document;
class LocalDOMWindow;
class LocalFrameClient;

class LocalFrame final : public Frame
{
public:
    static std::unique_ptr<LocalFrame> Create(LocalFrameClient &client, Page *page = nullptr);

    void Init(void) { m_loader.Init(); }

    LocalFrameClient* Client(void) const;
    FrameLoader& Loader(void) const { return m_loader; }
    LocalDOMWindow* DomWindow(void) const;
    Document* GetDocument(void) const;
private:
    LocalFrame(LocalFrameClient &client, Page *page);

    mutable FrameLoader m_loader;
};

} // namespace blink

#endif // BLINKIT_BLINK_LOCAL_FRAME_H
