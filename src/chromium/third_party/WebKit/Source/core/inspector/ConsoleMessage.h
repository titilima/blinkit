// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: ConsoleMessage.h
// Description: ConsoleMessage Class
//      Author: Ziming Li
//     Created: 2018-08-31
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ConsoleMessage_h
#define ConsoleMessage_h

#include "core/CoreExport.h"
#include "core/frame/ConsoleTypes.h"
#include "core/inspector/ConsoleAPITypes.h"
#include "platform/heap/Handle.h"
#include "wtf/Forward.h"
#include "wtf/PassRefPtr.h"
#include "wtf/RefCounted.h"
#include "wtf/text/WTFString.h"

namespace blink {

class LocalDOMWindow;

class CORE_EXPORT ConsoleMessage final: public RefCountedWillBeGarbageCollectedFinalized<ConsoleMessage> {
public:
    static PassRefPtrWillBeRawPtr<ConsoleMessage> create(MessageSource source, MessageLevel level, const String& message, const String& url = String(), unsigned lineNumber = 0, unsigned columnNumber = 0)
    {
        return adoptRefWillBeNoop(new ConsoleMessage(source, level, message, url, lineNumber, columnNumber));
    }
    ~ConsoleMessage();

    MessageType type() const;
    void setType(MessageType);
    int scriptId() const;
    void setScriptId(int);
    const String& url() const;
    void setURL(const String&);
    unsigned lineNumber() const;
    void setLineNumber(unsigned);
    unsigned long requestIdentifier() const;
    void setRequestIdentifier(unsigned long);
    double timestamp() const;
    void setTimestamp(double);
    unsigned assignMessageId();
    unsigned messageId() const { return m_messageId; }
    unsigned relatedMessageId() const { return m_relatedMessageId; }
    void setRelatedMessageId(unsigned relatedMessageId) { m_relatedMessageId = relatedMessageId; }

    MessageSource source() const;
    MessageLevel level() const;
    const String& message() const;
    unsigned columnNumber() const;

    unsigned argumentCount();

    void collectCallStack();

    DECLARE_TRACE();

private:
    ConsoleMessage(MessageSource, MessageLevel, const String& message, const String& url = String(), unsigned lineNumber = 0, unsigned columnNumber = 0);

    MessageSource m_source;
    MessageLevel m_level;
    MessageType m_type;
    String m_message;
    int m_scriptId;
    String m_url;
    unsigned m_lineNumber;
    unsigned m_columnNumber;
    unsigned long m_requestIdentifier;
    double m_timestamp;
    unsigned m_messageId;
    unsigned m_relatedMessageId;
};

} // namespace blink

#endif // ConsoleMessage_h
