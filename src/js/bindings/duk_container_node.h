// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: duk_container_node.h
// Description: Bindings for ContainerNode
//      Author: Ziming Li
//     Created: 2019-06-17
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_DUK_CONTAINER_NODE_H
#define BLINKIT_JS_DUK_CONTAINER_NODE_H

#pragma once

#include "core/dom/ContainerNode.h"

#include "duk_node.h"

namespace BlinKit {

class DukContainerNode : public DukNode
{
protected:
    DukContainerNode(void) = default;

    static void RegisterToPrototypeEntry(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_JS_DUK_CONTAINER_NODE_H
