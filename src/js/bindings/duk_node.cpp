// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_node.cpp
// Description: Bindings for Node
//      Author: Ziming Li
//     Created: 2019-06-01
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "duk_node.h"

namespace BlinKit {

void DukNode::RegisterToPrototypeEntry(PrototypeEntry &entry)
{
    DukEventTarget::RegisterToPrototypeEntry(entry);
    // BKTODO:
}

} // namespace BlinKit
