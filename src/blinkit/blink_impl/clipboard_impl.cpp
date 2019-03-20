// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: clipboard_impl.cpp
// Description: ClipboardImpl Class
//      Author: Ziming Li
//     Created: 2018-07-06
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "clipboard_impl.h"

using namespace blink;

namespace BlinKit {

bool ClipboardImpl::isFormatAvailable(Format format, Buffer buffer)
{
    assert(false); // Not reached!
    return false;
}

WebVector<WebString> ClipboardImpl::readAvailableTypes(Buffer buffer, bool *containsFilenames)
{
    WebVector<WebString> ret(1U);
    ret[0] = WebString("text/plain"); // Currently, support only plain text.
    return ret;
}

WebString ClipboardImpl::readCustomData(Buffer buffer, const WebString &type)
{
    assert(false); // Not reached!
    return WebString();
}

WebString ClipboardImpl::readHTML(Buffer buffer, WebURL *pageURL, unsigned *fragmentStart, unsigned *fragmentEnd)
{
    assert(false); // Not reached!
    return WebString();
}

WebData ClipboardImpl::readImage(Buffer buffer)
{
    assert(false); // Not reached!
    return WebData();
}

WebString ClipboardImpl::readPlainText(Buffer buffer)
{
    assert(false); // Not reached!
    return WebString();
}

uint64_t ClipboardImpl::sequenceNumber(Buffer buffer)
{
    assert(false); // Not reached!
    return 0;
}

void ClipboardImpl::writeDataObject(const WebDragData &data)
{
    assert(false); // Not reached!
}

void ClipboardImpl::writeHTML(const WebString &htmlText, const WebURL &sourceURL, const WebString &plainText, bool writeSmartPaste)
{
    assert(false); // Not reached!
}

void ClipboardImpl::writeImage(const WebImage &image, const WebURL &url, const WebString &title)
{
    assert(false); // Not reached!
}

void ClipboardImpl::writePlainText(const WebString &plainText)
{
    assert(false); // Not reached!
}

} // namespace BlinKit
