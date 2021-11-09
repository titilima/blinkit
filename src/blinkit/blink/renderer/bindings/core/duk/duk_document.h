// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_document.h
// Description: DukDocument Classes
//      Author: Ziming Li
//     Created: 2020-02-07
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_DUK_DOCUMENT_H
#define BLINKIT_BLINK_DUK_DOCUMENT_H

#pragma once

#include "blinkit/blink/renderer/bindings/core/duk/duk_container_node.h"
#include "blinkit/blink/renderer/core/dom/Document.h"

namespace BlinKit {

class DukDocument : public DukContainerNode
{
public:
    static const char ProtoName[];
protected:
    static void FillPrototypeEntry(PrototypeEntry &entry);
};

#ifdef BLINKIT_CRAWLER_ENABLED
class DukCrawlerDocument final : public DukDocument
{
public:
    static void RegisterPrototype(PrototypeHelper &helper);
private:
    static void FillPrototypeEntry(PrototypeEntry &entry);
};
#endif

#ifdef BLINKIT_UI_ENABLED
class DukUIDocument final : public DukDocument
{
public:
    static void RegisterPrototype(PrototypeHelper &helper);
};
#endif

} // namespace BlinKit

#endif // BLINKIT_BLINK_DUK_DOCUMENT_H
