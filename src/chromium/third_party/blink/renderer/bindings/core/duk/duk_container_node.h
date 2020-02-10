// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_container_node.h
// Description: DukContainerNode Class
//      Author: Ziming Li
//     Created: 2020-02-07
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_CONTAINER_NODE_H
#define BLINKIT_BLINK_DUK_CONTAINER_NODE_H

#pragma once

#include "third_party/blink/renderer/bindings/core/duk/duk_node.h"

namespace BlinKit {

class DukContainerNode : public DukNode
{
protected:
    static void FillPrototypeEntry(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_CONTAINER_NODE_H
