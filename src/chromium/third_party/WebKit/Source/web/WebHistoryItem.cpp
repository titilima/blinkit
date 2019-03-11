// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: WebHistoryItem.cpp
// Description: Placeholder for WebHistoryItem
//      Author: Ziming Li
//     Created: 2019-03-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "public/web/WebHistoryItem.h"

#include "core/loader/HistoryItem.h"

namespace blink {

void WebHistoryItem::reset()
{
    m_private.reset();
}

WebHistoryItem::WebHistoryItem(const PassRefPtrWillBeRawPtr<HistoryItem>& item)
    : m_private(item)
{
}

WebHistoryItem& WebHistoryItem::operator=(const PassRefPtrWillBeRawPtr<HistoryItem>& item)
{
    m_private = item;
    return *this;
}

WebHistoryItem::operator PassRefPtrWillBeRawPtr<HistoryItem>() const
{
    return m_private.get();
}

} // namespace blink
