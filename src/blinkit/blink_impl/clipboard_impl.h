// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: clipboard_impl.h
// Description: ClipboardImpl Class
//      Author: Ziming Li
//     Created: 2018-07-06
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_CLIPBOARD_IMPL_H
#define BLINKIT_BLINKIT_CLIPBOARD_IMPL_H

#pragma once

#include "public/platform/WebClipboard.h"

namespace BlinKit {

class ClipboardImpl : public blink::WebClipboard
{
protected:
    // blink::WebClipboard
    uint64_t sequenceNumber(Buffer buffer) override;
    bool isFormatAvailable(Format format, Buffer buffer) override;
    blink::WebVector<blink::WebString> readAvailableTypes(Buffer buffer, bool *containsFilenames) override;
    blink::WebString readPlainText(Buffer buffer) override;
    blink::WebString readHTML(Buffer buffer, blink::WebURL *pageURL, unsigned *fragmentStart, unsigned *fragmentEnd) override;
    blink::WebData readImage(Buffer buffer) override;
    blink::WebString readCustomData(Buffer buffer, const blink::WebString &type) override;
    void writePlainText(const blink::WebString &plainText) override;
    void writeHTML(const blink::WebString &htmlText, const blink::WebURL &sourceURL, const blink::WebString &plainText, bool writeSmartPaste) override;
    void writeImage(const blink::WebImage &image, const blink::WebURL &url, const blink::WebString &title) override;
    void writeDataObject(const blink::WebDragData &data) override;
};

} // namespace BlinKit

#endif // BLINKIT_BLINKIT_CLIPBOARD_IMPL_H
