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

#include "blinkit/blink/public/platform/WebClipboard.h"

namespace BlinKit {

class WinClipboard final : public blink::WebClipboard
{
private:
    String readPlainText(Buffer buffer) override;
    void writePlainText(const String &plainText) override;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_WIN_CLIPBOARD_H
