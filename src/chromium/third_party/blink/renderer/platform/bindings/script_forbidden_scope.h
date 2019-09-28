// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_forbidden_scope.h
// Description: ScriptForbiddenScope Class
//      Author: Ziming Li
//     Created: 2019-09-28
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_SCRIPT_FORBIDDEN_SCOPE_H
#define BLINKIT_BLINK_SCRIPT_FORBIDDEN_SCOPE_H

#pragma once

namespace blink {

class ScriptForbiddenScope final
{
public:
    ScriptForbiddenScope(void) { Enter(); }
    ~ScriptForbiddenScope(void) { Exit(); }

    static void Enter(void);
    static void Exit(void);
    static bool IsScriptForbidden(void);
};

} // namespace blink

#endif // BLINKIT_BLINK_SCRIPT_FORBIDDEN_SCOPE_H
