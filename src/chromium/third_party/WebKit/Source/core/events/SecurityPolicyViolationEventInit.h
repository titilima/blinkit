// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: SecurityPolicyViolationEventInit.h
// Description: SecurityPolicyViolationEventInit Class
//      Author: Ziming Li
//     Created: 2018-07-27
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_SECURITY_POLICY_VIOLATION_EVENT_INIT_H
#define BLINKIT_BLINK_SECURITY_POLICY_VIOLATION_EVENT_INIT_H

#pragma once

#include <optional>
#include "core/events/EventInit.h"
#include "wtf/text/WTFString.h"

namespace blink {

class SecurityPolicyViolationEventInit : public EventInit
{
public:
    bool hasDocumentURI(void) const { return !m_documentURI.isNull(); }
    const String& documentURI(void) const { return m_documentURI; }
    void setDocumentURI(String value) { m_documentURI = value; }

    bool hasReferrer(void) const { return !m_referrer.isNull(); }
    const String& referrer(void) const { return m_referrer; }
    void setReferrer(String value) { m_referrer = value; }

    bool hasBlockedURI(void) const { return !m_blockedURI.isNull(); }
    const String& blockedURI(void) const { return m_blockedURI; }
    void setBlockedURI(String value) { m_blockedURI = value; }

    bool hasViolatedDirective(void) const { return !m_violatedDirective.isNull(); }
    const String& violatedDirective(void) const { return m_violatedDirective; }
    void setViolatedDirective(String value) { m_violatedDirective = value; }

    bool hasEffectiveDirective(void) const { return !m_effectiveDirective.isNull(); }
    const String& effectiveDirective(void) const { return m_effectiveDirective; }
    void setEffectiveDirective(String value) { m_effectiveDirective = value; }

    bool hasOriginalPolicy(void) const { return !m_originalPolicy.isNull(); }
    const String& originalPolicy() const { return m_originalPolicy; }
    void setOriginalPolicy(String value) { m_originalPolicy = value; }

    bool hasSourceFile(void) const { return !m_sourceFile.isNull(); }
    const String& sourceFile(void) const { return m_sourceFile; }
    void setSourceFile(String value) { m_sourceFile = value; }
    bool hasLineNumber(void) const { return m_lineNumber.has_value(); }
    int lineNumber(void) const { return m_lineNumber.value(); }
    void setLineNumber(int value) { m_lineNumber = value; }
    bool hasColumnNumber(void) const { return m_columnNumber.has_value(); }
    int columnNumber(void) const { return m_columnNumber.value(); }
    void setColumnNumber(int value) { m_columnNumber = value; }

    bool hasStatusCode(void) const { return m_statusCode.has_value(); }
    int statusCode(void) const { return m_statusCode.value(); }
    void setStatusCode(int value) { m_statusCode = value; }
private:
    String m_documentURI, m_blockedURI;
    String m_referrer;
    String m_violatedDirective, m_effectiveDirective;
    String m_originalPolicy;

    String m_sourceFile;
    std::optional<int> m_lineNumber, m_columnNumber;

    std::optional<int> m_statusCode;
};

} // namespace blink

#endif // BLINKIT_BLINK_SECURITY_POLICY_VIOLATION_EVENT_INIT_H
