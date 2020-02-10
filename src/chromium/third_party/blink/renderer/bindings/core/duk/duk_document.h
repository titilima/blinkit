// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_document.h
// Description: DukDocument Class
//      Author: Ziming Li
//     Created: 2020-02-07
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_DOCUMENT_H
#define BLINKIT_BLINK_DUK_DOCUMENT_H

#pragma once

#include "third_party/blink/renderer/bindings/core/duk/duk_container_node.h"
#include "third_party/blink/renderer/core/dom/document.h"

namespace BlinKit {

class DukDocument final : public DukContainerNode
{
public:
    static const char ProtoName[];
    static void RegisterPrototypeForCrawler(PrototypeHelper &helper);
private:
    static void FillPrototypeEntryForCrawler(PrototypeEntry &entry);
};

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_DOCUMENT_H
