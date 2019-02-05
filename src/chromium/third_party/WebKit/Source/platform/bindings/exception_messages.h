// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: exception_messages.h
// Description: ExceptionMessages Class
//      Author: Ziming Li
//     Created: 2018-09-01
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_EXCEPTION_MESSAGES_H
#define BLINKIT_BLINK_EXCEPTION_MESSAGES_H

#pragma once

#include "wtf/text/StringBuilder.h"
#include "wtf/text/WTFString.h"

namespace blink {

class ExceptionMessages
{
public:
    static String argumentNullOrIncorrectType(int argumentIndex, const String &expectedType);

    static String failedToConstruct(const char *type, const String &detail);
    static String failedToExecute(const char *method, const char *type, const String &detail);

    template <typename NumType>
    static String formatNumber(NumType number)
    {
        return formatFiniteNumber(number);
    }

    template <typename NumberType>
    static String indexExceedsMaximumBound(const char *name, NumberType given, NumberType bound)
    {
        bool eq = given == bound;
        StringBuilder result;
        result.appendLiteral("The ");
        result.append(name);
        result.appendLiteral(" provided (");
        result.append(formatNumber(given));
        result.appendLiteral(") is greater than ");
        result.append(eq ? "or equal to " : "");
        result.appendLiteral("the maximum bound (");
        result.append(formatNumber(bound));
        result.appendLiteral(").");
        return result.toString();
    }

    template <typename NumberType>
    static String indexExceedsMinimumBound(const char *name, NumberType given, NumberType bound)
    {
        bool eq = given == bound;
        StringBuilder result;
        result.appendLiteral("The ");
        result.append(name);
        result.appendLiteral(" provided (");
        result.append(formatNumber(given));
        result.appendLiteral(") is less than ");
        result.append(eq ? "or equal to " : "");
        result.appendLiteral("the minimum bound (");
        result.append(formatNumber(bound));
        result.appendLiteral(").");
        return result.toString();
    }

    static String notAFiniteNumber(double value, const char *name = "value provided");

    static String readOnly(const char *detail = nullptr);
private:
    template <typename NumType>
    static String formatFiniteNumber(NumType number)
    {
        if (number > 1e20 || number < -1e20)
            return String::format("%e", 1.0*number);
        return String::number(number);
    }
};

} // namespace blink

#endif // BLINKIT_BLINK_EXCEPTION_MESSAGES_H
