// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: win_clipboard.h
// Description: WinClipboard Class
//      Author: Ziming Li
//     Created: 2018-07-06
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_WIN_CLIPBOARD_H
#define BLINKIT_BLINKIT_WIN_CLIPBOARD_H

#pragma once

#include "clipboard_impl.h"

namespace BlinKit {

class WinClipboard final : public ClipboardImpl
{
private:
    uint64_t sequenceNumber(Buffer buffer) override;
    blink::WebString readPlainText(Buffer buffer) override;
    void writePlainText(const blink::WebString &plainText) override;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_CLIPBOARD_H
