// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: shadow_root.cc
// Description: ShadowRoot Class
//      Author: Ziming Li
//     Created: 2020-11-01
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
 */

#include "third_party/blink/renderer/core/dom/shadow_root.h"

#include "third_party/blink/public/platform/platform.h"
#include "third_party/blink/renderer/core/css/resolver/style_resolver.h"
#include "third_party/blink/renderer/core/css/style_change_reason.h"
#include "third_party/blink/renderer/core/css/style_engine.h"
#include "third_party/blink/renderer/core/css/style_sheet_list.h"
#include "third_party/blink/renderer/core/dom/element_traversal.h"
#include "third_party/blink/renderer/core/dom/events/event_dispatch_forbidden_scope.h"
#include "third_party/blink/renderer/core/dom/shadow_root_v0.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/dom/slot_assignment.h"
#include "third_party/blink/renderer/core/dom/slot_assignment_engine.h"
#endif
#include "third_party/blink/renderer/core/dom/text.h"
#include "third_party/blink/renderer/core/dom/v0_insertion_point.h"
// BKTODO: #include "third_party/blink/renderer/core/dom/whitespace_attacher.h"
#include "third_party/blink/renderer/core/editing/serializers/serialization.h"
#if 0 // BKTODO:
#include "third_party/blink/renderer/core/html/html_content_element.h"
#include "third_party/blink/renderer/core/html/html_shadow_element.h"
#include "third_party/blink/renderer/core/html/html_slot_element.h"
#endif
#include "third_party/blink/renderer/core/layout/layout_object.h"
// BKTODO: #include "third_party/blink/renderer/core/trustedtypes/trusted_types_util.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"

namespace blink {

void ShadowRoot::Distribute() {
  if (!IsV1())
    V0().Distribute();
}

struct SameSizeAsShadowRoot : public DocumentFragment, public TreeScope {
  char empty_class_fields_due_to_gc_mixin_marker[1];
  Member<void*> member[3];
  unsigned counters_and_flags[1];
};

static_assert(sizeof(ShadowRoot) == sizeof(SameSizeAsShadowRoot),
              "ShadowRoot should stay small");

ShadowRoot::ShadowRoot(Document& document, ShadowRootType type)
    : DocumentFragment(nullptr, kCreateShadowRoot),
      TreeScope(*this, document),
      style_sheet_list_(nullptr),
      child_shadow_root_count_(0),
      type_(static_cast<unsigned short>(type)),
      registered_with_parent_shadow_root_(false),
      delegates_focus_(false),
      slotting_(static_cast<unsigned short>(ShadowRootSlotting::kAuto)),
      needs_distribution_recalc_(false),
      unused_(0) {
  if (IsV0())
    shadow_root_v0_ = new ShadowRootV0(*this);
}

ShadowRoot::~ShadowRoot() = default;

#if 0 // BKTODO:
SlotAssignment& ShadowRoot::EnsureSlotAssignment() {
  if (!slot_assignment_)
    slot_assignment_ = SlotAssignment::Create(*this);
  return *slot_assignment_;
}

HTMLSlotElement* ShadowRoot::AssignedSlotFor(const Node& node) {
  if (!slot_assignment_)
    return nullptr;
  return slot_assignment_->FindSlot(node);
}

void ShadowRoot::DidAddSlot(HTMLSlotElement& slot) {
  DCHECK(IsV1());
  EnsureSlotAssignment().DidAddSlot(slot);
}

void ShadowRoot::DidChangeHostChildSlotName(const AtomicString& old_value,
                                            const AtomicString& new_value) {
  if (!slot_assignment_)
    return;
  slot_assignment_->DidChangeHostChildSlotName(old_value, new_value);
}
#endif

Node* ShadowRoot::Clone(Document&, CloneChildrenFlag) const {
  NOTREACHED(); // ShadowRoot nodes are not clonable.
  return nullptr;
}

void ShadowRoot::SetSlotting(ShadowRootSlotting slotting) {
  slotting_ = static_cast<unsigned short>(slotting);
}

String ShadowRoot::InnerHTMLAsString() const {
  return CreateMarkup(this, kChildrenOnly);
}

void ShadowRoot::innerHTML(StringOrTrustedHTML& result) const {
  ASSERT(false); // BKTODO: result.SetString(InnerHTMLAsString());
}

void ShadowRoot::SetInnerHTMLFromString(const String& markup,
                                        ExceptionState& exception_state) {
  ASSERT(false); // BKTODO:
#if 0
  if (DocumentFragment* fragment = CreateFragmentForInnerOuterHTML(
          markup, &host(), kAllowScriptingContent, "innerHTML",
          exception_state))
    ReplaceChildrenWithFragment(this, fragment, exception_state);
#endif
}

void ShadowRoot::setInnerHTML(const StringOrTrustedHTML& stringOrHtml,
                              ExceptionState& exception_state) {
  ASSERT(false); // BKTODO:
#if 0
  String html =
      GetStringFromTrustedHTML(stringOrHtml, &GetDocument(), exception_state);
  if (!exception_state.HadException()) {
    SetInnerHTMLFromString(html, exception_state);
  }
#endif
}

void ShadowRoot::RecalcStyle(StyleRecalcChange change) {
  ASSERT(false); // BKTODO:
#if 0
  // ShadowRoot doesn't support custom callbacks.
  DCHECK(!HasCustomStyleCallbacks());

  if (GetStyleChangeType() >= kSubtreeStyleChange) {
    if (change < kForce)
      change = kForce;
    if (NeedsAttach() || change == kReattach)
      SetNeedsReattachLayoutTree();
  }
  // There's no style to update so just calling RecalcStyle means we're updated.
  if (change != kReattach)
    ClearNeedsStyleRecalc();

  if (change >= kUpdatePseudoElements || ChildNeedsStyleRecalc())
    RecalcDescendantStyles(change);
#endif
  ClearChildNeedsStyleRecalc();
}

void ShadowRoot::RebuildLayoutTree(WhitespaceAttacher& whitespace_attacher) {
  ClearNeedsReattachLayoutTree();
  ASSERT(false); // BKTODO: RebuildChildrenLayoutTrees(whitespace_attacher);
  ClearChildNeedsReattachLayoutTree();
}

void ShadowRoot::AttachLayoutTree(AttachContext& context) {
  Node::AttachContext children_context(context);
  DocumentFragment::AttachLayoutTree(children_context);
}

void ShadowRoot::DetachLayoutTree(const AttachContext& context) {
  Node::AttachContext children_context(context);
  children_context.clear_invalidation = true;
  GetDocument()
      .GetStyleEngine()
      .GetPendingNodeInvalidations()
      .ClearInvalidation(*this);
  DocumentFragment::DetachLayoutTree(children_context);
}

Node::InsertionNotificationRequest ShadowRoot::InsertedInto(
    ContainerNode& insertion_point) {
  DocumentFragment::InsertedInto(insertion_point);

  if (!insertion_point.isConnected())
    return kInsertionDone;

  ASSERT(false); // BKTODO: GetDocument().GetSlotAssignmentEngine().Connected(*this);

  // FIXME: When parsing <video controls>, InsertedInto() is called many times
  // without invoking RemovedFrom().  For now, we check
  // registered_with_parent_shadow_root. We would like to
  // DCHECK(!registered_with_parent_shadow_root) here.
  // https://bugs.webkit.org/show_bug.cig?id=101316
  if (registered_with_parent_shadow_root_)
    return kInsertionDone;

  if (ShadowRoot* root = host().ContainingShadowRoot()) {
    root->AddChildShadowRoot();
    registered_with_parent_shadow_root_ = true;
  }

  return kInsertionDone;
}

void ShadowRoot::RemovedFrom(ContainerNode& insertion_point) {
  if (insertion_point.isConnected()) {
    ASSERT(false); // BKTODO:
#if 0
    if (NeedsSlotAssignmentRecalc())
      GetDocument().GetSlotAssignmentEngine().Disconnected(*this);
    GetDocument().GetStyleEngine().ShadowRootRemovedFromDocument(this);
    if (registered_with_parent_shadow_root_) {
      ShadowRoot* root = host().ContainingShadowRoot();
      if (!root)
        root = insertion_point.ContainingShadowRoot();
      if (root)
        root->RemoveChildShadowRoot();
      registered_with_parent_shadow_root_ = false;
    }
    if (NeedsStyleInvalidation()) {
      GetDocument()
          .GetStyleEngine()
          .GetPendingNodeInvalidations()
          .ClearInvalidation(*this);
    }
#endif
  }

  DocumentFragment::RemovedFrom(insertion_point);
}

void ShadowRoot::SetNeedsAssignmentRecalc() {
  DCHECK(IsV1());
  ASSERT(false); // BKTODO:
#if 0
  if (!slot_assignment_)
    return;
  return slot_assignment_->SetNeedsAssignmentRecalc();
#endif
}

bool ShadowRoot::NeedsSlotAssignmentRecalc() const {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return slot_assignment_ && slot_assignment_->NeedsAssignmentRecalc();
#endif
}

void ShadowRoot::ChildrenChanged(const ChildrenChange& change) {
  ContainerNode::ChildrenChanged(change);

  ASSERT(false); // BKTODO:
#if 0
  if (change.IsChildElementChange()) {
    CheckForSiblingStyleChanges(
        change.type == kElementRemoved ? kSiblingElementRemoved
                                       : kSiblingElementInserted,
        ToElement(change.sibling_changed), change.sibling_before_change,
        change.sibling_after_change);
  }
#endif
}

StyleSheetList& ShadowRoot::StyleSheets() {
  if (!style_sheet_list_)
    SetStyleSheets(StyleSheetList::Create(this));
  return *style_sheet_list_;
}

void ShadowRoot::SetNeedsDistributionRecalcWillBeSetNeedsAssignmentRecalc() {
  if (IsV1())
    SetNeedsAssignmentRecalc();
  else
    SetNeedsDistributionRecalc();
}

void ShadowRoot::SetNeedsDistributionRecalc() {
  DCHECK(!IsV1());
  if (needs_distribution_recalc_)
    return;
  needs_distribution_recalc_ = true;
  host().MarkAncestorsWithChildNeedsDistributionRecalc();
  if (!IsV1())
    V0().ClearDistribution();
}

std::ostream& operator<<(std::ostream& ostream, const ShadowRootType& type) {
  switch (type) {
    case ShadowRootType::kUserAgent:
      ostream << "UserAgent";
      break;
    case ShadowRootType::V0:
      ostream << "V0";
      break;
    case ShadowRootType::kOpen:
      ostream << "Open";
      break;
    case ShadowRootType::kClosed:
      ostream << "Closed";
      break;
  }
  return ostream;
}

}  // namespace blink