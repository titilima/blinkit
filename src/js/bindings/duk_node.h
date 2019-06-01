// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_node.h
// Description: Bindings for Node
//      Author: Ziming Li
//     Created: 2019-06-01
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_NODE_H
#define BLINKIT_JS_DUK_NODE_H

#pragma once

#include "duk_event_target.h"

namespace BlinKit {

class DukNode : public DukEventTarget
{
protected:
    static void RegisterToPrototypeEntry(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_NODE_H
