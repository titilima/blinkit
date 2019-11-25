// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: wtf.cpp
// Description: Placeholders for WTF Part
//      Author: Ziming Li
//     Created: 2019-09-28
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "wtf.h"

#include "third_party/blink/renderer/platform/wtf/threading.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"
#include "third_party/blink/renderer/platform/wtf/text/string_statics.h"
#include "third_party/blink/renderer/platform/wtf/wtf_thread_data.h"

namespace WTF {

static bool g_initialized = false;
static void (*g_callOnMainThreadFunction)(MainThreadFunction, void *) = nullptr;
static ThreadIdentifier g_mainThreadIdentifier;

namespace internal {

void CallOnMainThread(MainThreadFunction *function, void *context)
{
    (*g_callOnMainThreadFunction)(function, context);
}

}  // namespace internal

bool IsMainThread(void)
{
    return CurrentThread() == g_mainThreadIdentifier;
}

void Initialize(void (*callOnMainThreadFunction)(MainThreadFunction, void *))
{
    // WTF, and Blink in general, cannot handle being re-initialized.
    // Make that explicit here.
    CHECK(!g_initialized);
    g_initialized = true;
    InitializeCurrentThread();
    g_mainThreadIdentifier = CurrentThread();

    WTFThreadData::Initialize();

#if 0 // BKTODO:
    InitializeDates();

    // Force initialization of static DoubleToStringConverter converter variable
    // inside EcmaScriptConverter function while we are in single thread mode.
    double_conversion::DoubleToStringConverter::EcmaScriptConverter();
#endif

    g_callOnMainThreadFunction = callOnMainThreadFunction;
#if 0 // BKTODO:
    internal::InitializeMainThreadStackEstimate();
#endif
    AtomicString::Init();
    StringStatics::Init();
}

} // namespace WTF
