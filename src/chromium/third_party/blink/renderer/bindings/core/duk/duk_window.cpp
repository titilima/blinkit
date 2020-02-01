// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: duk_window.cpp
// Description: DukWindow Class
//      Author: Ziming Li
//     Created: 2020-01-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#include "duk_window.h"

namespace BlinKit {

const char DukWindow::ProtoName[] = "Window";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Impl {

static duk_ret_t AToB(duk_context *ctx)
{
    duk_dup(ctx, 0);
    duk_base64_decode(ctx, -1);
    duk_buffer_to_string(ctx, -1);
    return 1;
}

static duk_ret_t BToA(duk_context *ctx)
{
    duk_dup(ctx, 0);
    duk_base64_encode(ctx, -1);
    duk_buffer_to_string(ctx, -1);
    return 1;
}

} // namespace Impl

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DukWindow::FillPrototypeEntryForCrawler(PrototypeEntry &entry)
{
    static const PrototypeEntry::Method Methods[] = {
        { "atob",             Impl::AToB,                1           },
        { "btoa",             Impl::BToA,                1           },
    };

    DukEventTarget::FillPrototypeEntry(entry);
    entry.Add(Methods, std::size(Methods));
}

void DukWindow::RegisterPrototypeForCrawler(PrototypeHelper &helper)
{
    helper.Register(ProtoName, FillPrototypeEntryForCrawler);
}

} // namespace BlinKit
