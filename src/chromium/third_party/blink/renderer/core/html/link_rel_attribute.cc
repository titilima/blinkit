// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: link_rel_attribute.CC
// Description: LinkRelAttribute Class
//      Author: Ziming Li
//     Created: 2020-12-31
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
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
 *
 */

#include "third_party/blink/renderer/core/html/link_rel_attribute.h"

#include "third_party/blink/renderer/platform/runtime_enabled_features.h"

namespace blink {

LinkRelAttribute::LinkRelAttribute()
    : // BKTODO: icon_type_(kInvalidIcon),
      is_style_sheet_(false),
      is_alternate_(false),
      is_dns_prefetch_(false),
      is_preconnect_(false),
      is_link_prefetch_(false),
      is_link_preload_(false),
      is_link_prerender_(false),
      is_link_next_(false),
      is_import_(false),
      is_manifest_(false),
      is_module_preload_(false),
      is_service_worker_(false),
      is_canonical_(false) {}

LinkRelAttribute::LinkRelAttribute(const String& rel) : LinkRelAttribute() {
  if (rel.IsEmpty())
    return;
  String rel_copy = rel;
  rel_copy.Replace('\n', ' ');
  Vector<String> list;
  rel_copy.Split(' ', list);
  for (const String& link_type : list) {
    if (DeprecatedEqualIgnoringCase(link_type, "stylesheet")) {
      if (!is_import_)
        is_style_sheet_ = true;
    } else if (DeprecatedEqualIgnoringCase(link_type, "import")) {
      if (!is_style_sheet_)
        is_import_ = true;
    } else if (DeprecatedEqualIgnoringCase(link_type, "alternate")) {
      is_alternate_ = true;
    } else if (DeprecatedEqualIgnoringCase(link_type, "icon")) {
      ASSERT(false); // BKTODO:
#if 0
      // This also allows "shortcut icon" since we just ignore the non-standard
      // "shortcut" token.
      // FIXME: This doesn't really follow the spec that requires "shortcut
      // icon" to be the entire string
      // http://www.whatwg.org/specs/web-apps/current-work/multipage/links.html#rel-icon
      icon_type_ = kFavicon;
#endif
    } else if (DeprecatedEqualIgnoringCase(link_type, "prefetch")) {
      is_link_prefetch_ = true;
    } else if (DeprecatedEqualIgnoringCase(link_type, "dns-prefetch")) {
      is_dns_prefetch_ = true;
    } else if (DeprecatedEqualIgnoringCase(link_type, "preconnect")) {
      is_preconnect_ = true;
    } else if (DeprecatedEqualIgnoringCase(link_type, "preload")) {
      is_link_preload_ = true;
    } else if (DeprecatedEqualIgnoringCase(link_type, "prerender")) {
      is_link_prerender_ = true;
    } else if (DeprecatedEqualIgnoringCase(link_type, "next")) {
      is_link_next_ = true;
    } else if (DeprecatedEqualIgnoringCase(link_type, "apple-touch-icon")) {
      ASSERT(false); // BKTODO: icon_type_ = kTouchIcon;
    } else if (DeprecatedEqualIgnoringCase(link_type,
                                           "apple-touch-icon-precomposed")) {
      ASSERT(false); // BKTODO: icon_type_ = kTouchPrecomposedIcon;
    } else if (DeprecatedEqualIgnoringCase(link_type, "manifest")) {
      is_manifest_ = true;
    } else if (DeprecatedEqualIgnoringCase(link_type, "modulepreload")) {
      ASSERT(false); // BKTODO:
#if 0
      if (RuntimeEnabledFeatures::ModulePreloadEnabled())
        is_module_preload_ = true;
#endif
    } else if (DeprecatedEqualIgnoringCase(link_type, "serviceworker")) {
      is_service_worker_ = true;
    } else if (DeprecatedEqualIgnoringCase(link_type, "canonical")) {
      is_canonical_ = true;
    }
    // Adding or removing a value here requires you to update
    // RelList::supportedTokens()
  }
}

}  // namespace blink
