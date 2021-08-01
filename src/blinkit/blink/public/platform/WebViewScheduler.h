// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: WebViewScheduler.h
// Description: WebViewScheduler Class
//      Author: Ziming Li
//     Created: 2021-07-17
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef WebViewScheduler_h
#define WebViewScheduler_h

#include "WebCommon.h"
// BKTODO: #include "public/platform/WebPassOwnPtr.h"

namespace blink {

class WebFrameScheduler;

class BLINK_PLATFORM_EXPORT WebViewScheduler {
public:
    virtual ~WebViewScheduler() { }

    // The scheduler may throttle tasks associated with background pages.
    virtual void setPageInBackground(bool) = 0;

#if 0 // BKTODO:
    // Creaters a new WebFrameScheduler, the caller is responsible for deleting it.
    virtual WebPassOwnPtr<WebFrameScheduler> createFrameScheduler() = 0;
#endif
};

} // namespace blink

#endif // WebViewScheduler
