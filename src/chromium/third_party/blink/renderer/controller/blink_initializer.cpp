// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: blink_initializer.cpp
// Description: Blink Initializer
//      Author: Ziming Li
//     Created: 2019-09-11
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#include "third_party/blink/public/web/blink.h"

#include "third_party/blink/renderer/core/css/parser/css_parser_token_range.h"
#include "third_party/blink/renderer/core/event_type_names.h"
#include "third_party/blink/renderer/core/html_names.h"
#include "third_party/blink/renderer/core/html_tokenizer_names.h"
#include "third_party/blink/renderer/core/xlink_names.h"
#include "third_party/blink/renderer/core/xml_names.h"
#include "third_party/blink/renderer/core/xmlns_names.h"
#include "third_party/blink/renderer/platform/network/http_names.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string_table.h"
#ifndef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/media_type_names.h"
#   include "third_party/blink/renderer/platform/font_family_names.h"
#   include "third_party/blink/renderer/platform/loader/fetch/fetch_initiator_type_names.h"
#endif

namespace blink {

static void InitializeCommon(Platform *platform)
{
    const unsigned kQualifiedNamesCount = html_names::kTagsCount + html_names::kAttrsCount
        + xlink_names::kAttrsCount + xml_names::kAttrsCount + xmlns_names::kAttrsCount;

    const unsigned kCoreStaticStringsCount = event_type_names::kNamesCount + html_tokenizer_names::kNamesCount + http_names::kNamesCount
#ifndef BLINKIT_CRAWLER_ONLY
        + media_type_names::kNamesCount + FontFamilyNames::FontFamilyNamesCount + fetch_initiator_type_names::kNamesCount
#endif
        ;

    StringImpl::ReserveStaticStringsCapacityForSize(kCoreStaticStringsCount + StringImpl::AllStaticStrings().size());
    QualifiedName::InitAndReserveCapacityForSize(kQualifiedNamesCount);
    AtomicStringTable::Instance().ReserveCapacity(kCoreStaticStringsCount);

    // BKTODO:
    html_names::Init();
    xlink_names::Init();
    xml_names::Init();
    xmlns_names::Init();

    event_type_names::Init();
    html_tokenizer_names::Init();
    http_names::Init();
#ifndef BLINKIT_CRAWLER_ONLY
    media_type_names::Init();
    FontFamilyNames::init();
    fetch_initiator_type_names::Init();
#endif

    CSSParserTokenRange::InitStaticEOFToken();
}

void Initialize(Platform *platform)
{
    Platform::Initialize(platform);
    InitializeCommon(platform);
}

} // namespace blink
