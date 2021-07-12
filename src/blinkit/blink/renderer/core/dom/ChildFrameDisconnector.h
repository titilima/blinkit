#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: ChildFrameDisconnector.h
// Description: Placeholder for Frame Stuff
//      Author: Ziming Li
//     Created: 2021-07-12
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CHILD_FRAME_DISCONNECTOR_H
#define BLINKIT_BLINKIT_CHILD_FRAME_DISCONNECTOR_H

#include "blinkit/blink/renderer/wtf/Allocator.h"

namespace blink {

class Node;

class ChildFrameDisconnector
{
    STACK_ALLOCATED();
public:
    ChildFrameDisconnector(Node &) {}

    enum DisconnectPolicy {
        RootAndDescendants,
        DescendantsOnly
    };
    void disconnect(DisconnectPolicy = RootAndDescendants) {}
};

} // namespace blink

#endif // BLINKIT_BLINKIT_CHILD_FRAME_DISCONNECTOR_H
