// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: NetworkHintsInterface.h
// Description: NetworkHintsInterface Classes
//      Author: Ziming Li
//     Created: 2019-05-14
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NetworkHintsInterface_h
#define NetworkHintsInterface_h

#include "platform/network/NetworkHints.h"

namespace blink {

class NetworkHintsInterface {
public:
    virtual void preconnectHost(const KURL&, const CrossOriginAttributeValue) const = 0;
};

class NetworkHintsInterfaceImpl : public NetworkHintsInterface {
    void preconnectHost(const KURL& host, const CrossOriginAttributeValue crossOrigin) const override
    {
        preconnect(host, crossOrigin);
    }
};

} // namespace blink

#endif
