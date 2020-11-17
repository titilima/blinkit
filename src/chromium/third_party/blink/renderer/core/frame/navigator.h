// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: navigator.h
// Description: Navigator Class
//      Author: Ziming Li
//     Created: 2020-03-25
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
    Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_NAVIGATOR_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_NAVIGATOR_H_

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/frame/navigator_id.h"
#include "third_party/blink/renderer/core/frame/navigator_language.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/wtf/forward.h"

namespace blink {

class LocalFrame;

class CORE_EXPORT Navigator final : public ScriptWrappable,
                                    public NavigatorID,
                                    public NavigatorLanguage {
  DEFINE_WRAPPERTYPEINFO();
  USING_GARBAGE_COLLECTED_MIXIN(Navigator);

 public:
  static Navigator* Create(LocalFrame* frame) { return new Navigator(frame); }

  // NavigatorCookies
  bool cookieEnabled() const;

  bool webdriver() const { return true; }

  String productSub() const;
  String vendor() const;
  String vendorSub() const;

  String userAgent() const override;

  // NavigatorLanguage
  Vector<String> languages() override;

 private:
  explicit Navigator(LocalFrame*);

  LocalFrame *m_frame;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_FRAME_NAVIGATOR_H_
