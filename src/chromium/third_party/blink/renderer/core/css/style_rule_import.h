// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: style_rule_import.h
// Description: StyleRuleImport Class
//      Author: Ziming Li
//     Created: 2020-08-06
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * (C) 2002-2003 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2002, 2006, 2008, 2012 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_CSS_STYLE_RULE_IMPORT_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_CSS_STYLE_RULE_IMPORT_H_

#include "third_party/blink/renderer/core/css/style_rule.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/loader/fetch/resource_client.h"

namespace blink {

class MediaQuerySet;
class StyleSheetContents;

class StyleRuleImport : public StyleRuleBase {
 public:
  static std::shared_ptr<StyleRuleImport> Create(const String& href,
                                                 scoped_refptr<MediaQuerySet>);

  ~StyleRuleImport();

  StyleSheetContents* ParentStyleSheet() const { return parent_style_sheet_; }
  void SetParentStyleSheet(StyleSheetContents* sheet) {
    DCHECK(sheet);
    parent_style_sheet_ = sheet;
  }
  void ClearParentStyleSheet() { parent_style_sheet_ = nullptr; }

  String Href() const { return str_href_; }
  StyleSheetContents* GetStyleSheet() const { return style_sheet_.get(); }

  bool IsLoading() const;
  MediaQuerySet* MediaQueries() { return media_queries_.get(); }

  void RequestStyleSheet();

  void TraceAfterDispatch(blink::Visitor*);

 private:
  // FIXME: inherit from ResourceClient directly to eliminate back
  // pointer, as there are no space savings in this.
  // NOTE: We put the ResourceClient in a member instead of inheriting
  // from it to avoid adding a vptr to StyleRuleImport.
  class ImportedStyleSheetClient final
      : public GarbageCollectedFinalized<ImportedStyleSheetClient>,
        public ResourceClient {
    USING_GARBAGE_COLLECTED_MIXIN(ImportedStyleSheetClient);

   public:
    ImportedStyleSheetClient(const std::shared_ptr<StyleRuleImport> &owner_rule)
        : owner_rule_(owner_rule) {}
    ~ImportedStyleSheetClient() override = default;
    void NotifyFinished(Resource* resource) override {
      owner_rule_->NotifyFinished(resource);
    }
    void Dispose() { ClearResource(); }

    String DebugName() const override { return "ImportedStyleSheetClient"; }

   private:
    std::shared_ptr<StyleRuleImport> owner_rule_;
  };

  void NotifyFinished(Resource*);

  StyleRuleImport(const String& href, scoped_refptr<MediaQuerySet>);

  void Dispose();

  Member<StyleSheetContents> parent_style_sheet_;

  Member<ImportedStyleSheetClient> style_sheet_client_;
  String str_href_;
  scoped_refptr<MediaQuerySet> media_queries_;
  std::shared_ptr<StyleSheetContents> style_sheet_;
  bool loading_;
};

DEFINE_STYLE_RULE_TYPE_CASTS(Import);

}  // namespace blink

#endif
