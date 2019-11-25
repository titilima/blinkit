// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: preload_request.h
// Description: PreloadRequest Class
//      Author: Ziming Li
//     Created: 2019-10-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef BLINKIT_BLINK_PRELOAD_REQUEST_H
#define BLINKIT_BLINK_PRELOAD_REQUEST_H

#pragma once

#include <vector>

namespace blink {

class Document;
class Resource;

class PreloadRequest
{
public:
    enum RequestType {
        kRequestTypePreload,
        kRequestTypePreconnect,
        kRequestTypeLinkRelPreload
    };

    Resource* Start(Document *document);
};

typedef std::vector<std::unique_ptr<PreloadRequest>> PreloadRequestStream;

}  // namespace blink

#endif // BLINKIT_BLINK_PRELOAD_REQUEST_H
