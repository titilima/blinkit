// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ResourcePreloader.cpp
// Description: ResourcePreloader Class
//      Author: Ziming Li
//     Created: 2021-07-25
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/html/parser/ResourcePreloader.h"
// BKTODO: #include "core/loader/NetworkHintsInterface.h"

namespace blink {

void ResourcePreloader::takeAndPreload(PreloadRequestStream& r)
{
    ASSERT(false); // BKTODO:
#if 0
    PreloadRequestStream requests;
    NetworkHintsInterfaceImpl networkHintsInterface;
    requests.swap(r);

    for (PreloadRequestStream::iterator it = requests.begin(); it != requests.end(); ++it)
        preload(it->release(), networkHintsInterface);
#endif
}

}
