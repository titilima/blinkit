// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: kurl.h
// Description: URL Helpers
//      Author: Ziming Li
//     Created: 2020-06-09
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_KURL_H
#define BLINKIT_BLINK_KURL_H

#pragma once

#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"
#include "url/gurl.h"

namespace blink {

const GURL& BlankURL(void);

bool EqualIgnoringFragmentIdentifier(const GURL &a, const GURL &b);

} // namespace blink

#endif // BLINKIT_BLINK_KURL_H
