// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_element.cpp
// Description: DukElement Class
//      Author: Ziming Li
//     Created: 2020-02-11
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_element.h"

using namespace blink;

namespace BlinKit {

const char DukElement::ProtoName[] = "HTMLElement";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Impl {

} // namespace Impl

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DukElement::FillPrototypeEntryForCrawler(PrototypeEntry &entry)
{
    DukContainerNode::FillPrototypeEntry(entry);
}

void DukElement::RegisterPrototypeForCrawler(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntryForCrawler);
}

} // namespace BlinKit
