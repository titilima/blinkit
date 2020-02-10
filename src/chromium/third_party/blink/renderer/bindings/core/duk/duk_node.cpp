// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_node.cpp
// Description: DukNode Class
//      Author: Ziming Li
//     Created: 2020-02-07
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_node.h"

namespace BlinKit {

void DukNode::FillPrototypeEntry(PrototypeEntry &entry)
{
    DukEventTarget::FillPrototypeEntry(entry);
}

} // namespace BlinKit
