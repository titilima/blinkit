#pragma once
// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: script_controller.h
// Description: Dispatcher for ScriptController Class
//      Author: Ziming Li
//     Created: 2022-03-02
// -------------------------------------------------
// Copyright (C) 2022 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_SCRIPT_CONTROLLER_H
#define BLINKIT_SCRIPT_CONTROLLER_H

namespace blink {

class LocalFrame;
class ScriptSourceCode;

enum ReasonForCallingCanExecuteScripts {
    AboutToExecuteScript,
    NotAboutToExecuteScript
};

}

#include "./qjs/script_controller_impl.h"

#endif // BLINKIT_SCRIPT_CONTROLLER_H
