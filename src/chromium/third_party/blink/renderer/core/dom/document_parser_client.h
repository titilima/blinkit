// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: document_parser_client.h
// Description: DocumentParserClient Class
//      Author: Ziming Li
//     Created: 2019-10-17
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_DOM_DOCUMENT_PARSER_CLIENT_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_DOM_DOCUMENT_PARSER_CLIENT_H_

#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class DocumentParserClient : public GarbageCollectedMixin {
 public:
  // This callback is called when all data pushed to parser has been consumed.
  virtual void NotifyParserStopped() = 0;

 protected:
  DocumentParserClient() = default;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_DOM_DOCUMENT_PARSER_CLIENT_H_
