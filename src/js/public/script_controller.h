// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: script_controller.h
// Description: ScriptController Class
//      Author: Ziming Li
//     Created: 2018-07-12
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_JS_SCRIPT_CONTROLLER_H
#define BLINKIT_JS_SCRIPT_CONTROLLER_H

#pragma once

#include "platform/heap/Handle.h"

namespace BlinKit {
class DukContext;
}

namespace blink {

class LocalFrame;
class SecurityOrigin;

enum ReasonForCallingCanExecuteScripts {
    AboutToExecuteScript,
    NotAboutToExecuteScript
};

class ScriptController final {
    WTF_MAKE_NONCOPYABLE(ScriptController);
public:
    ~ScriptController(void);

    void enableEval(void);
    void disableEval(const String &errorMessage);

    bool canExecuteScripts(ReasonForCallingCanExecuteScripts);

    void updateSecurityOrigin(SecurityOrigin *);
private:
    explicit ScriptController(LocalFrame *);

    std::unique_ptr<BlinKit::DukContext> m_context;
};

} // namespace blink

#endif // BLINKIT_JS_SCRIPT_CONTROLLER_H
