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

#include "sdk/include/BlinKit.h"
#include "core/fetch/AccessControlStatus.h"
#include "platform/heap/Handle.h"

namespace BlinKit {
class DukContext;
}

namespace blink {

class HTMLDocument;
class KURL;
class LocalFrame;
class ScriptSourceCode;
class SecurityOrigin;

enum ReasonForCallingCanExecuteScripts {
    AboutToExecuteScript,
    NotAboutToExecuteScript
};

class ScriptController final {
    WTF_MAKE_NONCOPYABLE(ScriptController);
public:
    static PassOwnPtrWillBeRawPtr<ScriptController> create(LocalFrame *frame)
    {
        return adoptPtrWillBeNoop(new ScriptController(frame));
    }
    ~ScriptController(void);

    int CreateCrawlerObject(const char *script, size_t length);
    int CallFunction(const char *name, BlinKit::BkCallerContext::Callback callback, void *userData);
    int CallCrawler(const char *method, BlinKit::BkCallerContext::Callback callback, void *userData);
    int RegisterFunction(const char *name, BlinKit::BkFunction *functionImpl);

    void executeScriptInMainWorld(const ScriptSourceCode &, AccessControlStatus = NotSharableCrossOrigin, double *compilationFinishTime = nullptr);

    bool executeScriptIfJavaScriptURL(const KURL &);

    bool shouldBypassMainWorldCSP(void) { return true; }

    void enableEval(void);
    void disableEval(const String &errorMessage);

    static bool canAccessFromCurrentOrigin(LocalFrame *frame) { return nullptr != frame; }

    bool canExecuteScripts(ReasonForCallingCanExecuteScripts);

    void clearWindowProxy(void);
    void updateDocument(void);

    void namedItemAdded(HTMLDocument *, const AtomicString &);
    void namedItemRemoved(HTMLDocument *, const AtomicString &);

    void updateSecurityOrigin(SecurityOrigin *);
    void clearScriptObjects(void);

    void clearForClose(void);
private:
    explicit ScriptController(LocalFrame *);

    BlinKit::DukContext& EnsureContext(void);

    LocalFrame &m_frame;
    std::unique_ptr<BlinKit::DukContext> m_context;
};

} // namespace blink

#endif // BLINKIT_JS_SCRIPT_CONTROLLER_H
