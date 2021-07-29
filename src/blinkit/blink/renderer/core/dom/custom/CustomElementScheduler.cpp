// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: CustomElementScheduler.cpp
// Description: CustomElementScheduler Class
//      Author: Ziming Li
//     Created: 2021-07-29
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer
 *    in the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of Google Inc. nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "core/dom/custom/CustomElementScheduler.h"

#include "core/dom/Document.h"
#include "core/dom/Element.h"
#include "core/dom/custom/CustomElementCallbackInvocation.h"
#include "core/dom/custom/CustomElementLifecycleCallbacks.h"
#if 0 // BKTODO:
#include "core/dom/custom/CustomElementMicrotaskDispatcher.h"
#include "core/dom/custom/CustomElementMicrotaskImportStep.h"
#include "core/dom/custom/CustomElementMicrotaskResolutionStep.h"
#endif
#include "core/dom/custom/CustomElementMicrotaskRunQueue.h"
#include "core/dom/custom/CustomElementProcessingStack.h"
#include "core/dom/custom/CustomElementRegistrationContext.h"
// BKTODO: #include "core/dom/custom/CustomElementSyncMicrotaskQueue.h"
#include "core/html/imports/HTMLImportChild.h"
#include "core/html/imports/HTMLImportsController.h"

namespace blink {

DEFINE_EMPTY_DESTRUCTOR_WILL_BE_REMOVED(CustomElementScheduler)

// FIXME: Consider moving the element's callback queue to ElementRareData.
typedef WillBeHeapHashMap<RawPtrWillBeMember<Element>, OwnPtrWillBeMember<CustomElementCallbackQueue>> ElementCallbackQueueMap;

static ElementCallbackQueueMap& callbackQueues()
{
    DEFINE_STATIC_LOCAL(OwnPtrWillBePersistent<ElementCallbackQueueMap>, map, (adoptPtrWillBeNoop(new ElementCallbackQueueMap())));
    return *map;
}

static CustomElementCallbackQueue& ensureCallbackQueue(PassRefPtrWillBeRawPtr<Element> element)
{
    ASSERT(false); // BKTODO:
    exit(0);
#if 0
    ElementCallbackQueueMap::ValueType* it = callbackQueues().add(element.get(), nullptr).storedValue;
    if (!it->value)
        it->value = CustomElementCallbackQueue::create(element);
    return *it->value.get();
#endif
}

// Finds or creates the callback queue for element.
static CustomElementCallbackQueue& scheduleCallbackQueue(PassRefPtrWillBeRawPtr<Element> passElement)
{
    RefPtrWillBeRawPtr<Element> element(passElement);

    CustomElementCallbackQueue& callbackQueue = ensureCallbackQueue(element);
    if (callbackQueue.inCreatedCallback()) {
        // Don't move it. Authors use the createdCallback like a
        // constructor. By not moving it, the createdCallback
        // completes before any other callbacks are entered for this
        // element.
        return callbackQueue;
    }

    if (CustomElementProcessingStack::inCallbackDeliveryScope()) {
        // The processing stack is active.
        CustomElementProcessingStack::instance().enqueue(&callbackQueue);
        return callbackQueue;
    }

    ASSERT(false); // BKTODO: CustomElementMicrotaskDispatcher::instance().enqueue(&callbackQueue);
    return callbackQueue;
}

void CustomElementScheduler::scheduleCallback(PassRefPtrWillBeRawPtr<CustomElementLifecycleCallbacks> callbacks, PassRefPtrWillBeRawPtr<Element> element, CustomElementLifecycleCallbacks::CallbackType type)
{
    ASSERT(type != CustomElementLifecycleCallbacks::AttributeChangedCallback);

    if (!callbacks->hasCallback(type))
        return;

    CustomElementCallbackQueue& queue = scheduleCallbackQueue(element);
    queue.append(CustomElementCallbackInvocation::createInvocation(callbacks, type));
}

void CustomElementScheduler::scheduleAttributeChangedCallback(PassRefPtrWillBeRawPtr<CustomElementLifecycleCallbacks> callbacks, PassRefPtrWillBeRawPtr<Element> element, const AtomicString& name, const AtomicString& oldValue, const AtomicString& newValue)
{
    if (!callbacks->hasCallback(CustomElementLifecycleCallbacks::AttributeChangedCallback))
        return;

    CustomElementCallbackQueue& queue = scheduleCallbackQueue(element);
    queue.append(CustomElementCallbackInvocation::createAttributeChangedInvocation(callbacks, name, oldValue, newValue));
}

void CustomElementScheduler::resolveOrScheduleResolution(PassRefPtrWillBeRawPtr<CustomElementRegistrationContext> context, PassRefPtrWillBeRawPtr<Element> element, const CustomElementDescriptor& descriptor)
{
    if (CustomElementProcessingStack::inCallbackDeliveryScope()) {
        context->resolve(element.get(), descriptor);
        return;
    }

    Document& document = element->document();
    ASSERT(false); // BKTODO:
#if 0
    OwnPtrWillBeRawPtr<CustomElementMicrotaskResolutionStep> step = CustomElementMicrotaskResolutionStep::create(context, element, descriptor);
    enqueueMicrotaskStep(document, step.release());
#endif
}

CustomElementMicrotaskImportStep* CustomElementScheduler::scheduleImport(HTMLImportChild* import)
{
    ASSERT(!import->hasFinishedLoading());
    ASSERT(import->parent());

    ASSERT(false); // BKTODO:
    return nullptr;
#if 0
    // Ownership of the new step is transferred to the parent
    // processing step, or the base queue.
    OwnPtrWillBeRawPtr<CustomElementMicrotaskImportStep> step = CustomElementMicrotaskImportStep::create(import);
    CustomElementMicrotaskImportStep* rawStep = step.get();
    enqueueMicrotaskStep(*(import->parent()->document()), step.release(), import->isSync());
    return rawStep;
#endif
}

void CustomElementScheduler::enqueueMicrotaskStep(Document& document, PassOwnPtrWillBeRawPtr<CustomElementMicrotaskStep> step, bool importIsSync)
{
    Document& master = document.importsController() ? *(document.importsController()->master()) : document;
    master.customElementMicrotaskRunQueue()->enqueue(document.importLoader(), step, importIsSync);
}


void CustomElementScheduler::callbackDispatcherDidFinish()
{
    ASSERT(false); // BKTODO:
#if 0
    if (CustomElementMicrotaskDispatcher::instance().elementQueueIsEmpty())
        callbackQueues().clear();
#endif
}

void CustomElementScheduler::microtaskDispatcherDidFinish()
{
    ASSERT(!CustomElementProcessingStack::inCallbackDeliveryScope());
    callbackQueues().clear();
}

} // namespace blink
