// -------------------------------------------------
// BlinKit - net Library
// -------------------------------------------------
//   File Name: cookie_options.h
// Description: CookieOptions Class
//      Author: Ziming Li
//     Created: 2021-08-05
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Brought to you by number 42.

#ifndef NET_COOKIES_COOKIE_OPTIONS_H_
#define NET_COOKIES_COOKIE_OPTIONS_H_

#include "base/time/time.h"
#include "net/base/net_export.h"
#include "third_party/zed/include/zed/net/url.hpp"

namespace net {

class NET_EXPORT CookieOptions {
 public:
  // Creates a CookieOptions object which:
  //
  // * Excludes HttpOnly cookies
  // * Excludes First-Party-Only cookies
  // * Does not enforce prefix restrictions (e.g. "$Secure-*")
  //
  // These settings can be altered by calling:
  //
  // * |set_{include,exclude}_httponly()|
  // * |set_include_first_party_only_cookies()|
  // * |set_enforce_prefixes()|
  CookieOptions();

  void set_exclude_httponly() { exclude_httponly_ = true; }
  void set_include_httponly() { exclude_httponly_ = false; }
  bool exclude_httponly() const { return exclude_httponly_; }

  // Default is to exclude 'first-party-only' cookies.
  void set_include_first_party_only_cookies() {
    include_first_party_only_cookies_ = true;
  }
  bool include_first_party_only_cookies() const {
    return include_first_party_only_cookies_;
  }

  // TODO(jww): Remove once we decide whether to ship modifying 'secure' cookies
  // only from secure schemes. https://crbug.com/546820
  void set_enforce_strict_secure() { enforce_strict_secure_ = true; }
  bool enforce_strict_secure() const { return enforce_strict_secure_; }

  // |server_time| indicates what the server sending us the Cookie thought the
  // current time was when the cookie was produced.  This is used to adjust for
  // clock skew between server and host.
  void set_server_time(const base::Time& server_time) {
    server_time_ = server_time;
  }
  bool has_server_time() const { return !server_time_.is_null(); }
  base::Time server_time() const { return server_time_; }

 private:
  bool exclude_httponly_;
  bool include_first_party_only_cookies_;
  bool enforce_strict_secure_;
  base::Time server_time_;
};

}  // namespace net

#endif  // NET_COOKIES_COOKIE_OPTIONS_H_
