// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: execution_context.h
// Description: ExecutionContext Class
//      Author: Ziming Li
//     Created: 2019-10-18
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_EXECUTION_CONTEXT_H
#define BLINKIT_BLINK_EXECUTION_CONTEXT_H

#pragma once

#include "third_party/blink/public/platform/task_type.h"
#include "third_party/blink/renderer/core/dom/context_lifecycle_notifier.h"
#include "third_party/blink/renderer/core/dom/context_lifecycle_observer.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

class GURL;

namespace base {
class SingleThreadTaskRunner;
}

namespace blink {

class LocalDOMWindow;
class ResourceFetcher;

enum ReasonForCallingCanExecuteScripts {
    kAboutToExecuteScript,
    kNotAboutToExecuteScript
};

enum class SecureContextMode { kInsecureContext, kSecureContext };

class ExecutionContext : public ContextLifecycleNotifier
{
public:
    virtual bool IsDocument(void) const { return false; }

    virtual const GURL& Url(void) const = 0;
    virtual const GURL& BaseURL(void) const = 0;
    virtual GURL CompleteURL(const String &url) const = 0;
    virtual LocalDOMWindow* ExecutingWindow(void) const { return nullptr; }

    virtual ResourceFetcher* Fetcher(void) const = 0;

    virtual bool CanExecuteScripts(ReasonForCallingCanExecuteScripts reason) { return false; }

    // Returns a referrer to be used in the "Determine request's Referrer"
    // algorithm defined in the Referrer Policy spec.
    // https://w3c.github.io/webappsec-referrer-policy/#determine-requests-referrer
    virtual String OutgoingReferrer(void) const;

    virtual std::shared_ptr<base::SingleThreadTaskRunner> GetTaskRunner(TaskType) = 0;
};

} // namespace blink

#endif // BLINKIT_BLINK_EXECUTION_CONTEXT_H
