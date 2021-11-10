// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: blink_initializer.cpp
// Description: Blink Initializer
//      Author: Ziming Li
//     Created: 2019-09-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "blinkit/blink/public/web/blink.h"

#include "blinkit/blink/renderer/core/EventNames.h"
#include "blinkit/blink/renderer/core/EventTargetNames.h"
#include "blinkit/blink/renderer/core/EventTypeNames.h"
#include "blinkit/blink/renderer/core/HTMLNames.h"
#include "blinkit/blink/renderer/core/HTMLTokenizerNames.h"
#include "blinkit/blink/renderer/core/css/CSSPrimitiveValue.h"
#include "blinkit/blink/renderer/core/css/parser/CSSParserTokenRange.h"
#include "blinkit/blink/renderer/core/dom/Document.h"
#include "blinkit/blink/renderer/core/events/event_factory.h"
#include "blinkit/blink/renderer/platform/HTTPNames.h"
#include "blinkit/blink/renderer/wtf/MainThread.h"
#include "blinkit/blink/renderer/wtf/WTF.h"
#ifdef BLINKIT_UI_ENABLED
#   include "blinkit/blink/renderer/core/InputTypeNames.h"
#   include "blinkit/blink/renderer/core/MediaFeatureNames.h"
#   include "blinkit/blink/renderer/core/MediaTypeNames.h"
#   include "blinkit/blink/renderer/platform/FontFamilyNames.h"
#endif

namespace blink {

static void InitializeCommon(Platform *platform)
{
    constexpr unsigned kQualifiedNamesCount = HTMLNames::HTMLTagsCount + HTMLNames::HTMLAttrsCount;
    constexpr unsigned kCoreStaticStringsCount = EventNames::EventNamesCount + EventTargetNames::EventTargetNamesCount
        + EventTypeNames::EventTypeNamesCount + HTMLTokenizerNames::HTMLTokenizerNamesCount + HTTPNames::HTTPNamesCount
#ifdef BLINKIT_UI_ENABLED
        + InputTypeNames::InputTypeNamesCount + MediaFeatureNames::MediaFeatureNamesCount
        + MediaTypeNames::MediaTypeNamesCount + FontFamilyNames::FontFamilyNamesCount
#endif
        ;

    StringImpl::reserveStaticStringsCapacityForSize(kCoreStaticStringsCount + StringImpl::allStaticStrings().size());
    QualifiedName::initAndReserveCapacityForSize(kQualifiedNamesCount);
    AtomicString::reserveTableCapacity(kCoreStaticStringsCount);

    HTMLNames::init();

    EventNames::init();
    EventTargetNames::init();
    EventTypeNames::init();
    HTMLTokenizerNames::init();
    HTTPNames::init();
#ifdef BLINKIT_UI_ENABLED
    InputTypeNames::init();
    MediaFeatureNames::init();
    MediaTypeNames::init();
    FontFamilyNames::init();
#endif

#ifdef BLINKIT_UI_ENABLED
    CSSPrimitiveValue::initUnitTable();
#endif
    CSSParserTokenRange::initStaticEOFToken();

    StringImpl::freezeStaticStrings();

    Document::registerEventFactory(EventFactory::create());
}

static double CurrentTimeFunction(void)
{
    return Platform::current()->currentTimeSeconds();
}

static double MonotonicallyIncreasingTimeFunction()
{
    return Platform::current()->monotonicallyIncreasingTimeSeconds();
}

void Initialize(Platform *platform)
{
    Platform::initialize(platform);
    WTF::initialize(CurrentTimeFunction, MonotonicallyIncreasingTimeFunction, nullptr, nullptr);
    WTF::initializeMainThread(nullptr);
    InitializeCommon(platform);
}

} // namespace blink
