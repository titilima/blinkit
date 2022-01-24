// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: DragClientImpl.cpp
// Description: DragClientImpl Class
//      Author: Ziming Li
//     Created: 2021-08-09
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
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

#include "./DragClientImpl.h"

#include "blinkit/blink/public/platform/WebCommon.h"
#include "blinkit/blink/renderer/core/clipboard/DataObject.h"
#include "blinkit/blink/renderer/core/clipboard/DataTransfer.h"
#include "blinkit/blink/renderer/core/frame/LocalFrame.h"
#include "blinkit/blink/renderer/platform/DragImage.h"
#if 0 // BKTODO:
#include "public/platform/WebDragData.h"
#include "public/platform/WebImage.h"
#include "public/platform/WebPoint.h"
#include "public/web/WebDragOperation.h"
#include "public/web/WebViewClient.h"
#include "web/WebViewImpl.h"
#endif
#include "blinkit/blink/renderer/wtf/Assertions.h"
#include "blinkit/blink/renderer/wtf/RefPtr.h"

namespace blink {

DragDestinationAction DragClientImpl::actionMaskForDrag(DragData*)
{
    ASSERT(false); // BKTODO:
#if 0
    if (m_webView->client() && m_webView->client()->acceptsLoadDrops())
        return DragDestinationActionAny;
#endif

    return static_cast<DragDestinationAction>(
        DragDestinationActionDHTML | DragDestinationActionEdit);
}

void DragClientImpl::startDrag(DragImage* dragImage, const IntPoint& dragImageOrigin, const IntPoint& eventPos, DataTransfer* dataTransfer, LocalFrame* frame, bool isLinkDrag)
{
    // Add a ref to the frame just in case a load occurs mid-drag.
    RefPtrWillBeRawPtr<LocalFrame> frameProtector(frame);

    ASSERT(false); // BKTODO:
#if 0
    WebDragData dragData = dataTransfer->dataObject()->toWebDragData();
    WebDragOperationsMask dragOperationMask = static_cast<WebDragOperationsMask>(dataTransfer->sourceOperation());
    WebImage image;
    IntSize offsetSize(eventPos - dragImageOrigin);
    WebPoint offsetPoint(offsetSize.width(), offsetSize.height());

    if (dragImage) {
        float resolutionScale = dragImage->resolutionScale();
        float deviceScaleFactor =
            m_webView->client()->screenInfo().deviceScaleFactor;
        if (deviceScaleFactor != resolutionScale) {
            ASSERT(resolutionScale > 0);
            float scale = deviceScaleFactor / resolutionScale;
            dragImage->scale(scale, scale);
        }
        image = dragImage->bitmap();
    }

    m_webView->startDragging(frame, dragData, dragOperationMask, image, offsetPoint);
#endif
}

} // namespace blink
