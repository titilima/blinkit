#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: prototype_traits.h
// Description: Protytype Traits
//      Author: Ziming Li
//     Created: 2022-03-14
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_PROTOTYPE_TRAITS_H
#define BLINKIT_PROTOTYPE_TRAITS_H

namespace blink {
class Document;
class Location;
}

namespace BlinKit {
namespace qjs {

enum Prototypes {
    PROTO_Window = 0,
    PROTO_Document,
    PROTO_Element,

    PROTO_Location,

    PROTO_MAX
};

template <class T>
struct PrototypeTrait {
};

template <>
struct PrototypeTrait<blink::Document> {
    constexpr static int PROTO = PROTO_Document;
};

template <>
struct PrototypeTrait<blink::Location> {
    constexpr static int PROTO = PROTO_Location;
};

} // namespace qjs
} // namespace BlinKit

#endif // BLINKIT_PROTOTYPE_TRAITS_H
