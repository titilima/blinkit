// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: meta_data.h
// Description: Meta Data for Prototype
//      Author: Ziming Li
//     Created: 2019-05-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_META_DATA_H
#define BLINKIT_JS_META_DATA_H

#pragma once

namespace BlinKit {

struct MetaData {
    enum {
        Window,
        Console
    };
    int typeId;

    MetaData *parentData;
};

} // namespace BlinKit

#endif // BLINKIT_JS_META_DATA_H
