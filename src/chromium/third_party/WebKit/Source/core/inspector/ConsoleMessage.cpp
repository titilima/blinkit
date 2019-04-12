// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ConsoleMessage.cpp
// Description: ConsoleMessage Class
//      Author: Ziming Li
//     Created: 2019-02-22
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "core/inspector/ConsoleMessage.h"

#include "wtf/CurrentTime.h"
#include "wtf/PassOwnPtr.h"

namespace blink {

unsigned nextMessageId()
{
    struct MessageId {
        MessageId() : value(0) { }
        unsigned value;
    };

    DEFINE_THREAD_SAFE_STATIC_LOCAL(WTF::ThreadSpecific<MessageId>, messageId, new WTF::ThreadSpecific<MessageId>);
    return ++messageId->value;
}

ConsoleMessage::ConsoleMessage(MessageSource source,
    MessageLevel level,
    const String& message,
    const String& url,
    unsigned lineNumber,
    unsigned columnNumber)
    : m_source(source)
    , m_level(level)
    , m_type(LogMessageType)
    , m_message(message)
    , m_scriptId(0)
    , m_url(url)
    , m_lineNumber(lineNumber)
    , m_columnNumber(columnNumber)
    , m_requestIdentifier(0)
    , m_timestamp(WTF::currentTime())
    , m_messageId(0)
    , m_relatedMessageId(0)
{
}

ConsoleMessage::~ConsoleMessage()
{
}

MessageType ConsoleMessage::type() const
{
    return m_type;
}

void ConsoleMessage::setType(MessageType type)
{
    m_type = type;
}

int ConsoleMessage::scriptId() const
{
    return m_scriptId;
}

void ConsoleMessage::setScriptId(int scriptId)
{
    m_scriptId = scriptId;
}

const String& ConsoleMessage::url() const
{
    return m_url;
}

void ConsoleMessage::setURL(const String& url)
{
    m_url = url;
}

unsigned ConsoleMessage::lineNumber() const
{
    return m_lineNumber;
}

void ConsoleMessage::setLineNumber(unsigned lineNumber)
{
    m_lineNumber = lineNumber;
}

unsigned long ConsoleMessage::requestIdentifier() const
{
    return m_requestIdentifier;
}

void ConsoleMessage::setRequestIdentifier(unsigned long requestIdentifier)
{
    m_requestIdentifier = requestIdentifier;
}

double ConsoleMessage::timestamp() const
{
    return m_timestamp;
}

void ConsoleMessage::setTimestamp(double timestamp)
{
    m_timestamp = timestamp;
}

unsigned ConsoleMessage::assignMessageId()
{
    if (!m_messageId)
        m_messageId = nextMessageId();
    return m_messageId;
}

MessageSource ConsoleMessage::source() const
{
    return m_source;
}

MessageLevel ConsoleMessage::level() const
{
    return m_level;
}

const String& ConsoleMessage::message() const
{
    return m_message;
}

unsigned ConsoleMessage::columnNumber() const
{
    return m_columnNumber;
}

unsigned ConsoleMessage::argumentCount()
{
    assert(false); // Not reached!
    return 0;
}

void ConsoleMessage::collectCallStack()
{
    if (m_type == EndGroupMessageType)
        return;

    assert(false); // Not reached!
}

DEFINE_TRACE(ConsoleMessage)
{
}

} // namespace blink
