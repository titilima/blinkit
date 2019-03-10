// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: exception_messages.cpp
// Description: ExceptionMessages Class
//      Author: Ziming Li
//     Created: 2018-09-01
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "exception_messages.h"

namespace blink {

String ExceptionMessages::argumentNullOrIncorrectType(int argumentIndex, const String &expectedType)
{
    assert(false); // BKTODO:
    return String();
}

String ExceptionMessages::failedToConstruct(const char *type, const String &detail)
{
    assert(false); // BKTODO:
    return String();
}

String ExceptionMessages::failedToExecute(const char *method, const char *type, const String &detail)
{
    assert(false); // BKTODO:
    return String();
}

String ExceptionMessages::notAFiniteNumber(double value, const char *name)
{
    assert(false); // BKTODO:
    return String();
}

String ExceptionMessages::readOnly(const char *detail)
{
    assert(false); // BKTODO:
    return String();
}

} // namespace blink
