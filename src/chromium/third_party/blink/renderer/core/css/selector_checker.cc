// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: selector_checker.cc
// Description: SelectorChecker Class
//      Author: Ziming Li
//     Created: 2020-03-30
// -------------------------------------------------
// Copyright (C) 2020 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 2004-2005 Allan Sandfeld Jensen (kde@carewolf.com)
 * Copyright (C) 2006, 2007 Nicholas Shanks (webkit@nickshanks.com)
 * Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012, 2013 Apple Inc.
 * All rights reserved.
 * Copyright (C) 2007 Alexey Proskuryakov <ap@webkit.org>
 * Copyright (C) 2007, 2008 Eric Seidel <eric@webkit.org>
 * Copyright (C) 2008, 2009 Torch Mobile Inc. All rights reserved.
 * (http://www.torchmobile.com/)
 * Copyright (c) 2011, Code Aurora Forum. All rights reserved.
 * Copyright (C) Research In Motion Limited 2011. All rights reserved.
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

#include "third_party/blink/renderer/core/css/selector_checker.h"

#include "base/auto_reset.h"
#include "third_party/blink/renderer/core/css/css_selector_list.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/core/dom/element_traversal.h"
#include "third_party/blink/renderer/core/dom/nth_index_cache.h"
#include "third_party/blink/renderer/core/dom/text.h"
#include "third_party/blink/renderer/core/html/html_document.h"
#include "third_party/blink/renderer/core/html/parser/html_parser_idioms.h"
#include "third_party/blink/renderer/platform/runtime_enabled_features.h"
#ifdef BLINKIT_CRAWLER_ONLY
#   include "third_party/blink/renderer/core/frame/use_counter.h"
#else
#   include "third_party/blink/renderer/core/css/part_names.h"
#   include "third_party/blink/renderer/core/css/style_engine.h"
#   include "third_party/blink/renderer/core/dom/flat_tree_traversal.h"
#   include "third_party/blink/renderer/core/dom/node_computed_style.h"
#   include "third_party/blink/renderer/core/dom/shadow_root.h"
#   include "third_party/blink/renderer/core/dom/v0_insertion_point.h"
#   include "third_party/blink/renderer/core/editing/frame_selection.h"
#   include "third_party/blink/renderer/core/frame/local_frame.h"
#if 0 // BKTODO:
#   include "third_party/blink/renderer/core/html/forms/html_form_control_element.h"
#   include "third_party/blink/renderer/core/html/forms/html_input_element.h"
#   include "third_party/blink/renderer/core/html/forms/html_option_element.h"
#   include "third_party/blink/renderer/core/html/forms/html_select_element.h"
#   include "third_party/blink/renderer/core/html/html_frame_element_base.h"
#   include "third_party/blink/renderer/core/html/html_slot_element.h"
#   include "third_party/blink/renderer/core/html/media/html_video_element.h"
#   include "third_party/blink/renderer/core/html/track/vtt/vtt_element.h"
#endif
#   include "third_party/blink/renderer/core/html_names.h"
// BKTODO: #include "third_party/blink/renderer/core/page/focus_controller.h"
#   include "third_party/blink/renderer/core/page/page.h"
#   include "third_party/blink/renderer/core/probe/core_probes.h"
#   include "third_party/blink/renderer/core/scroll/scrollable_area.h"
// BKTODO: #include "third_party/blink/renderer/core/scroll/scrollbar_theme.h"
#   include "third_party/blink/renderer/core/style/computed_style.h"
#endif

namespace blink {

using namespace html_names;

#ifndef BLINKIT_CRAWLER_ONLY
static bool IsFrameFocused(const Element& element) {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return element.GetDocument().GetFrame() && element.GetDocument()
                                                 .GetFrame()
                                                 ->Selection()
                                                 .FrameIsFocusedAndActive();
#endif
}
#endif

static bool MatchesSpatialNavigationFocusPseudoClass(const Element& element) {
#ifdef BLINKIT_CRAWLER_ONLY
  return false;
#else
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return IsHTMLOptionElement(element) &&
         ToHTMLOptionElement(element).SpatialNavigationFocused() &&
         IsFrameFocused(element);
#endif
#endif
}

static bool MatchesListBoxPseudoClass(const Element& element) {
#ifdef BLINKIT_CRAWLER_ONLY
  return false; // BKTODO: Implementation for <select> tag.
#else
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return IsHTMLSelectElement(element) &&
         !ToHTMLSelectElement(element).UsesMenuList();
#endif
#endif
}

static bool MatchesTagName(const Element& element,
                           const QualifiedName& tag_q_name) {
  if (tag_q_name == AnyQName())
    return true;
  const AtomicString& local_name = tag_q_name.LocalName();
  if (local_name != CSSSelector::UniversalSelectorAtom() &&
      local_name != element.localName()) {
    if (element.IsHTMLElement() || !element.GetDocument().IsHTMLDocument())
      return false;
    // Non-html elements in html documents are normalized to their camel-cased
    // version during parsing if applicable. Yet, type selectors are lower-cased
    // for selectors in html documents. Compare the upper case converted names
    // instead to allow matching SVG elements like foreignObject.
    if (element.TagQName().LocalNameUpper() != tag_q_name.LocalNameUpper())
      return false;
  }
  const AtomicString& namespace_uri = tag_q_name.NamespaceURI();
  return namespace_uri == g_star_atom ||
         namespace_uri == element.namespaceURI();
}

static Element* ParentElement(
    const SelectorChecker::SelectorCheckingContext& context) {
#ifdef BLINKIT_CRAWLER_ONLY
  ASSERT(nullptr == context.element->ContainingShadowRoot());
#else
  // - If context.scope is a shadow root, we should walk up to its shadow host.
  // - If context.scope is some element in some shadow tree and querySelector
  //   initialized the context, e.g. shadowRoot.querySelector(':host *'),
  //   (a) context.element has the same treescope as context.scope, need to walk
  //       up to its shadow host.
  //   (b) Otherwise, should not walk up from a shadow root to a shadow host.
  if (context.scope &&
      (context.scope == context.element->ContainingShadowRoot() ||
       context.scope->GetTreeScope() == context.element->GetTreeScope()))
    return context.element->ParentOrShadowHostElement();
#endif
  return context.element->parentElement();
}

#ifndef BLINKIT_CRAWLER_ONLY
// If context has scope, return slot that matches the scope, otherwise return
// the assigned slot for scope-less matching of ::slotted pseudo element.
static const HTMLSlotElement* FindSlotElementInScope(
    const SelectorChecker::SelectorCheckingContext& context) {
  ASSERT(false); // BKTODO:
#if 0
  if (!context.scope)
    return context.element->AssignedSlot();

  for (const HTMLSlotElement* slot = context.element->AssignedSlot(); slot;
       slot = slot->AssignedSlot()) {
    if (slot->GetTreeScope() == context.scope->GetTreeScope())
      return slot;
  }
#endif
  return nullptr;
}
#endif

static bool ScopeContainsLastMatchedElement(
    const SelectorChecker::SelectorCheckingContext& context) {
  // If this context isn't scoped, skip checking.
  if (!context.scope)
    return true;

  if (context.scope->GetTreeScope() == context.element->GetTreeScope())
    return true;

  // Because Blink treats a shadow host's TreeScope as a separate one from its
  // descendent shadow roots, if the last matched element is a shadow host, the
  // condition above isn't met, even though it should be.
  return context.element == context.scope->OwnerShadowHost() &&
         (!context.previous_element ||
          context.previous_element->IsInDescendantTreeOf(context.element));
}

static inline bool NextSelectorExceedsScope(
    const SelectorChecker::SelectorCheckingContext& context) {
  if (context.scope && context.scope->IsInShadowTree())
    return context.element == context.scope->OwnerShadowHost();

  return false;
}

#ifndef BLINKIT_CRAWLER_ONLY
static bool ShouldMatchHoverOrActive(
    const SelectorChecker::SelectorCheckingContext& context) {
  // If we're in quirks mode, then :hover and :active should never match anchors
  // with no href and *:hover and *:active should not match anything. This is
  // specified in https://quirks.spec.whatwg.org/#the-:active-and-:hover-quirk
  if (!context.element->GetDocument().InQuirksMode())
    return true;
  if (context.is_sub_selector)
    return true;
  if (context.element->IsLink())
    return true;
  const CSSSelector* selector = context.selector;
  while (selector->Relation() == CSSSelector::kSubSelector &&
         selector->TagHistory()) {
    selector = selector->TagHistory();
    if (selector->Match() != CSSSelector::kPseudoClass)
      return true;
    if (selector->GetPseudoType() != CSSSelector::kPseudoHover &&
        selector->GetPseudoType() != CSSSelector::kPseudoActive)
      return true;
  }
  return false;
}
#endif

static bool IsFirstChild(Element& element) {
  return !ElementTraversal::PreviousSibling(element);
}

static bool IsLastChild(Element& element) {
  return !ElementTraversal::NextSibling(element);
}

static bool IsFirstOfType(Element& element, const QualifiedName& type) {
  return !ElementTraversal::PreviousSibling(element, HasTagName(type));
}

static bool IsLastOfType(Element& element, const QualifiedName& type) {
  return !ElementTraversal::NextSibling(element, HasTagName(type));
}

// Recursive check of selectors and combinators
// It can return 4 different values:
// * SelectorMatches          - the selector matches the element e
// * SelectorFailsLocally     - the selector fails for the element e
// * SelectorFailsAllSiblings - the selector fails for e and any sibling of e
// * SelectorFailsCompletely  - the selector fails for e and any sibling or
//   ancestor of e
SelectorChecker::MatchStatus SelectorChecker::MatchSelector(
    const SelectorCheckingContext& context,
    MatchResult& result) const {
  MatchResult sub_result;
  if (!CheckOne(context, sub_result))
    return kSelectorFailsLocally;

  if (sub_result.dynamic_pseudo != kPseudoIdNone)
    result.dynamic_pseudo = sub_result.dynamic_pseudo;

  if (context.selector->IsLastInTagHistory()) {
    if (ScopeContainsLastMatchedElement(context)) {
      result.specificity += sub_result.specificity;
      return kSelectorMatches;
    }
    return kSelectorFailsLocally;
  }

  MatchStatus match;
  if (context.selector->Relation() != CSSSelector::kSubSelector) {
    if (NextSelectorExceedsScope(context))
      return kSelectorFailsCompletely;

    if (context.pseudo_id != kPseudoIdNone &&
        context.pseudo_id != result.dynamic_pseudo)
      return kSelectorFailsCompletely;

    base::AutoReset<PseudoId> dynamic_pseudo_scope(&result.dynamic_pseudo,
                                                   kPseudoIdNone);
    match = MatchForRelation(context, result);
  } else {
    match = MatchForSubSelector(context, result);
  }
  if (match == kSelectorMatches)
    result.specificity += sub_result.specificity;
  return match;
}

static inline SelectorChecker::SelectorCheckingContext
PrepareNextContextForRelation(
    const SelectorChecker::SelectorCheckingContext& context) {
  SelectorChecker::SelectorCheckingContext next_context(context);
  DCHECK(context.selector->TagHistory());
  next_context.selector = context.selector->TagHistory();
  return next_context;
}

SelectorChecker::MatchStatus SelectorChecker::MatchForSubSelector(
    const SelectorCheckingContext& context,
    MatchResult& result) const {
  SelectorCheckingContext next_context = PrepareNextContextForRelation(context);

  PseudoId dynamic_pseudo = result.dynamic_pseudo;
  next_context.has_scrollbar_pseudo =
      dynamic_pseudo != kPseudoIdNone &&
      (scrollbar_ || dynamic_pseudo == kPseudoIdScrollbarCorner ||
       dynamic_pseudo == kPseudoIdResizer);

  // Only match pseudo classes following scrollbar pseudo elements while
  // actually computing style for scrollbar pseudo elements. This is to
  // avoid incorrectly setting affected-by flags on actual elements for
  // cases like: div::-webkit-scrollbar-thumb:hover { }
  if (context.in_rightmost_compound && dynamic_pseudo != kPseudoIdNone &&
      dynamic_pseudo != kPseudoIdSelection &&
      !next_context.has_scrollbar_pseudo) {
    return kSelectorFailsCompletely;
  }

  next_context.has_selection_pseudo = dynamic_pseudo == kPseudoIdSelection;
  next_context.is_sub_selector = true;
  return MatchSelector(next_context, result);
}

#ifndef BLINKIT_CRAWLER_ONLY
static inline bool IsV0ShadowRoot(const Node* node) {
  ASSERT(false); // BKTODO:
  return false;
#if 0
  return node && node->IsShadowRoot() &&
         ToShadowRoot(node)->GetType() == ShadowRootType::V0;
#endif
}
#endif

SelectorChecker::MatchStatus SelectorChecker::MatchForPseudoShadow(
    const SelectorCheckingContext& context,
    const ContainerNode* node,
    MatchResult& result) const {
#ifdef BLINKIT_CRAWLER_ONLY
  return kSelectorFailsCompletely;
#else
  if (!IsV0ShadowRoot(node))
    return kSelectorFailsCompletely;
  if (!context.previous_element)
    return kSelectorFailsCompletely;
  return MatchSelector(context, result);
#endif
}

static inline Element* ParentOrV0ShadowHostElement(const Element& element) {
#ifndef BLINKIT_CRAWLER_ONLY
  if (element.parentNode() && element.parentNode()->IsShadowRoot()) {
    ASSERT(false); // BKTODO:
#if 0
    if (ToShadowRoot(element.parentNode())->GetType() != ShadowRootType::V0)
      return nullptr;
#endif
  }
#endif
  return element.ParentOrShadowHostElement();
}

SelectorChecker::MatchStatus SelectorChecker::MatchForRelation(
    const SelectorCheckingContext& context,
    MatchResult& result) const {
  SelectorCheckingContext next_context = PrepareNextContextForRelation(context);

  CSSSelector::RelationType relation = context.selector->Relation();

  // Disable :visited matching when we see the first link or try to match
  // anything else than an ancestors.
  //
  // FIXME(emilio): This is_sub_selector check is wrong if we allow sub
  // selectors with combinators somewhere.
  if (!context.is_sub_selector &&
      (context.element->IsLink() || (relation != CSSSelector::kDescendant &&
                                     relation != CSSSelector::kChild)))
    next_context.visited_match_type = kVisitedMatchDisabled;

  next_context.in_rightmost_compound = false;
  next_context.is_sub_selector = false;
  next_context.previous_element = context.element;
  next_context.pseudo_id = kPseudoIdNone;

  switch (relation) {
    case CSSSelector::kShadowDeepAsDescendant:
      FALLTHROUGH;
    case CSSSelector::kDescendant:
      if (next_context.selector->GetPseudoType() == CSSSelector::kPseudoScope) {
        if (next_context.selector->IsLastInTagHistory()) {
          if (context.scope->IsDocumentFragment())
            return kSelectorMatches;
        }
      }

      if (context.selector->RelationIsAffectedByPseudoContent()) {
        for (Element* element = context.element; element;
             element = element->parentElement()) {
          if (MatchForPseudoContent(next_context, *element, result) ==
              kSelectorMatches)
            return kSelectorMatches;
        }
        return kSelectorFailsCompletely;
      }

#ifdef BLINKIT_CRAWLER_ONLY
      ASSERT(CSSSelector::kPseudoShadow != next_context.selector->GetPseudoType());
#else
      if (next_context.selector->GetPseudoType() == CSSSelector::kPseudoShadow)
        return MatchForPseudoShadow(
            next_context, context.element->ContainingShadowRoot(), result);
#endif

      for (next_context.element = ParentElement(next_context);
           next_context.element;
           next_context.element = ParentElement(next_context)) {
        MatchStatus match = MatchSelector(next_context, result);
        if (match == kSelectorMatches || match == kSelectorFailsCompletely)
          return match;
        if (NextSelectorExceedsScope(next_context))
          return kSelectorFailsCompletely;
        if (next_context.element->IsLink())
          next_context.visited_match_type = kVisitedMatchDisabled;
      }
      return kSelectorFailsCompletely;
    case CSSSelector::kChild: {
      if (next_context.selector->GetPseudoType() == CSSSelector::kPseudoScope) {
        if (next_context.selector->IsLastInTagHistory()) {
          if (context.element->parentNode() == context.scope &&
              context.scope->IsDocumentFragment())
            return kSelectorMatches;
        }
      }

      if (context.selector->RelationIsAffectedByPseudoContent())
        return MatchForPseudoContent(next_context, *context.element, result);

      if (next_context.selector->GetPseudoType() == CSSSelector::kPseudoShadow)
        return MatchForPseudoShadow(next_context, context.element->parentNode(),
                                    result);

      next_context.element = ParentElement(next_context);
      if (!next_context.element)
        return kSelectorFailsCompletely;
      return MatchSelector(next_context, result);
    }
    case CSSSelector::kDirectAdjacent:
      // Shadow roots can't have sibling elements
      if (next_context.selector->GetPseudoType() == CSSSelector::kPseudoShadow)
        return kSelectorFailsCompletely;

      if (mode_ == kResolvingStyle) {
        if (ContainerNode* parent =
                context.element->ParentElementOrShadowRoot())
          parent->SetChildrenAffectedByDirectAdjacentRules();
      }
      next_context.element =
          ElementTraversal::PreviousSibling(*context.element);
      if (!next_context.element)
        return kSelectorFailsAllSiblings;
      return MatchSelector(next_context, result);

    case CSSSelector::kIndirectAdjacent:
      // Shadow roots can't have sibling elements
      if (next_context.selector->GetPseudoType() == CSSSelector::kPseudoShadow)
        return kSelectorFailsCompletely;

      if (mode_ == kResolvingStyle) {
        if (ContainerNode* parent =
                context.element->ParentElementOrShadowRoot())
          parent->SetChildrenAffectedByIndirectAdjacentRules();
      }
      next_context.element =
          ElementTraversal::PreviousSibling(*context.element);
      for (; next_context.element;
           next_context.element =
               ElementTraversal::PreviousSibling(*next_context.element)) {
        MatchStatus match = MatchSelector(next_context, result);
        if (match == kSelectorMatches || match == kSelectorFailsAllSiblings ||
            match == kSelectorFailsCompletely)
          return match;
      }
      return kSelectorFailsAllSiblings;

    case CSSSelector::kShadowPseudo: {
      DCHECK(mode_ == kQueryingRules ||
             context.selector->GetPseudoType() != CSSSelector::kPseudoShadow);
      if (context.selector->GetPseudoType() == CSSSelector::kPseudoShadow) {
        UseCounter::Count(context.element->GetDocument(),
                          WebFeature::kPseudoShadowInStaticProfile);
      }
      // If we're in the same tree-scope as the scoping element, then following
      // a shadow descendant combinator would escape that and thus the scope.
      if (context.scope && context.scope->OwnerShadowHost() &&
          context.scope->OwnerShadowHost()->GetTreeScope() ==
              context.element->GetTreeScope())
        return kSelectorFailsCompletely;

      Element* shadow_host = context.element->OwnerShadowHost();
      if (!shadow_host)
        return kSelectorFailsCompletely;
      next_context.element = shadow_host;
      return MatchSelector(next_context, result);
    }

    case CSSSelector::kShadowDeep: {
      DCHECK(mode_ == kQueryingRules);
      UseCounter::Count(context.element->GetDocument(),
                        WebFeature::kDeepCombinatorInStaticProfile);
#ifndef BLINKIT_CRAWLER_ONLY
      if (ShadowRoot* root = context.element->ContainingShadowRoot()) {
        ASSERT(false); // BKTODO:
#if 0
        if (root->IsUserAgent())
          return kSelectorFailsCompletely;
#endif
      }
#endif

      if (context.selector->RelationIsAffectedByPseudoContent()) {
        // TODO(kochi): closed mode tree should be handled as well for
        // ::content.
        for (Element* element = context.element; element;
             element = element->ParentOrShadowHostElement()) {
          if (MatchForPseudoContent(next_context, *element, result) ==
              kSelectorMatches) {
            if (context.element->IsInShadowTree()) {
              UseCounter::Count(context.element->GetDocument(),
                                WebFeature::kCSSDeepCombinatorAndShadow);
            }
            return kSelectorMatches;
          }
        }
        return kSelectorFailsCompletely;
      }

      for (next_context.element = ParentOrV0ShadowHostElement(*context.element);
           next_context.element;
           next_context.element =
               ParentOrV0ShadowHostElement(*next_context.element)) {
        MatchStatus match = MatchSelector(next_context, result);
        if (match == kSelectorMatches && context.element->IsInShadowTree()) {
          UseCounter::Count(context.element->GetDocument(),
                            WebFeature::kCSSDeepCombinatorAndShadow);
        }
        if (match == kSelectorMatches || match == kSelectorFailsCompletely)
          return match;
        if (NextSelectorExceedsScope(next_context))
          return kSelectorFailsCompletely;
      }
      return kSelectorFailsCompletely;
    }

    case CSSSelector::kShadowSlot: {
#ifdef BLINKIT_CRAWLER_ONLY
      return kSelectorFailsCompletely;
#else
      ASSERT(false); // BKTODO:
#if 0
      if (ToHTMLSlotElementIfSupportsAssignmentOrNull(*context.element))
        return kSelectorFailsCompletely;
      const HTMLSlotElement* slot = FindSlotElementInScope(context);
      if (!slot)
        return kSelectorFailsCompletely;

      next_context.element = const_cast<HTMLSlotElement*>(slot);
#endif
      return MatchSelector(next_context, result);
#endif
    }

    case CSSSelector::kShadowPart:
      // We ascend through ancestor shadow host elements until we reach the host
      // in the TreeScope associated with the style rule. We then match against
      // that host.
      if (RuntimeEnabledFeatures::CSSPartPseudoElementEnabled()) {
        while (next_context.element) {
          next_context.element = next_context.element->OwnerShadowHost();
          if (!next_context.element)
            return kSelectorFailsCompletely;

          if (next_context.element->GetTreeScope() ==
              context.scope->GetTreeScope())
            return MatchSelector(next_context, result);
        }
      }
      return kSelectorFailsCompletely;
      break;
    case CSSSelector::kSubSelector:
      break;
  }
  NOTREACHED();
  return kSelectorFailsCompletely;
}

SelectorChecker::MatchStatus SelectorChecker::MatchForPseudoContent(
    const SelectorCheckingContext& context,
    const Element& element,
    MatchResult& result) const {
#ifndef BLINKIT_CRAWLER_ONLY
  std::vector<V0InsertionPoint *> insertion_points;
  CollectDestinationInsertionPoints(element, insertion_points);
  SelectorCheckingContext next_context(context);
  for (const auto& insertion_point : insertion_points) {
    next_context.element = insertion_point;
    if (Match(next_context, result))
      return kSelectorMatches;
  }
#endif
  return kSelectorFailsLocally;
}

static bool AttributeValueMatches(const Attribute& attribute_item,
                                  CSSSelector::MatchType match,
                                  const AtomicString& selector_value,
                                  TextCaseSensitivity case_sensitivity) {
  // TODO(esprehn): How do we get here with a null value?
  const AtomicString& value = attribute_item.Value();
  if (value.IsNull())
    return false;

  switch (match) {
    case CSSSelector::kAttributeExact:
      if (case_sensitivity == kTextCaseSensitive)
        return selector_value == value;
      return EqualIgnoringASCIICase(selector_value, value);
    case CSSSelector::kAttributeSet:
      return true;
    case CSSSelector::kAttributeList: {
      // Ignore empty selectors or selectors containing HTML spaces
      if (selector_value.IsEmpty() ||
          selector_value.Find(&IsHTMLSpace<UChar>) != kNotFound)
        return false;

      unsigned start_search_at = 0;
      while (true) {
        wtf_size_t found_pos =
            value.Find(selector_value, start_search_at, case_sensitivity);
        if (found_pos == kNotFound)
          return false;
        if (!found_pos || IsHTMLSpace<UChar>(value[found_pos - 1])) {
          unsigned end_str = found_pos + selector_value.length();
          if (end_str == value.length() || IsHTMLSpace<UChar>(value[end_str]))
            break;  // We found a match.
        }

        // No match. Keep looking.
        start_search_at = found_pos + 1;
      }
      return true;
    }
    case CSSSelector::kAttributeContain:
      if (selector_value.IsEmpty())
        return false;
      return value.Contains(selector_value, case_sensitivity);
    case CSSSelector::kAttributeBegin:
      if (selector_value.IsEmpty())
        return false;
      return value.StartsWith(selector_value, case_sensitivity);
    case CSSSelector::kAttributeEnd:
      if (selector_value.IsEmpty())
        return false;
      return value.EndsWith(selector_value, case_sensitivity);
    case CSSSelector::kAttributeHyphen:
      if (value.length() < selector_value.length())
        return false;
      if (!value.StartsWith(selector_value, case_sensitivity))
        return false;
      // It they start the same, check for exact match or following '-':
      if (value.length() != selector_value.length() &&
          value[selector_value.length()] != '-')
        return false;
      return true;
    default:
      NOTREACHED();
      return false;
  }
}

static bool AnyAttributeMatches(Element& element,
                                CSSSelector::MatchType match,
                                const CSSSelector& selector) {
  const QualifiedName& selector_attr = selector.Attribute();
  // Should not be possible from the CSS grammar.
  DCHECK_NE(selector_attr.LocalName(), CSSSelector::UniversalSelectorAtom());

  // Synchronize the attribute in case it is lazy-computed.
  // Currently all lazy properties have a null namespace, so only pass
  // localName().
  element.SynchronizeAttribute(selector_attr.LocalName());

  const AtomicString& selector_value = selector.Value();
  TextCaseSensitivity case_sensitivity =
      (selector.AttributeMatch() == CSSSelector::kCaseInsensitive)
          ? kTextCaseASCIIInsensitive
          : kTextCaseSensitive;

  AttributeCollection attributes = element.AttributesWithoutUpdate();
  for (const auto& attribute_item : attributes) {
    if (!attribute_item.Matches(selector_attr)) {
      if (element.IsHTMLElement() || !element.GetDocument().IsHTMLDocument())
        continue;
      // Non-html attributes in html documents are normalized to their camel-
      // cased version during parsing if applicable. Yet, attribute selectors
      // are lower-cased for selectors in html documents. Compare the selector
      // and the attribute local name insensitively to e.g. allow matching SVG
      // attributes like viewBox.
      if (!attribute_item.MatchesCaseInsensitive(selector_attr))
        continue;
    }

    if (AttributeValueMatches(attribute_item, match, selector_value,
                              case_sensitivity))
      return true;

    if (case_sensitivity == kTextCaseASCIIInsensitive) {
      if (selector_attr.NamespaceURI() != g_star_atom)
        return false;
      continue;
    }

    // Legacy dictates that values of some attributes should be compared in
    // a case-insensitive manner regardless of whether the case insensitive
    // flag is set or not.
    bool legacy_case_insensitive =
        element.GetDocument().IsHTMLDocument() &&
        !HTMLDocument::IsCaseSensitiveAttribute(selector_attr);

    // If case-insensitive, re-check, and count if result differs.
    // See http://code.google.com/p/chromium/issues/detail?id=327060
    if (legacy_case_insensitive &&
        AttributeValueMatches(attribute_item, match, selector_value,
                              kTextCaseASCIIInsensitive)) {
      UseCounter::Count(element.GetDocument(),
                        WebFeature::kCaseInsensitiveAttrSelectorMatch);
      return true;
    }
    if (selector_attr.NamespaceURI() != g_star_atom)
      return false;
  }

  return false;
}

bool SelectorChecker::CheckOne(const SelectorCheckingContext& context,
                               MatchResult& result) const {
  DCHECK(context.element);
  Element& element = *context.element;
  DCHECK(context.selector);
  const CSSSelector& selector = *context.selector;

  // Only :host and :host-context() should match the host:
  // http://drafts.csswg.org/css-scoping/#host-element
  if (context.scope && context.scope->OwnerShadowHost() == element &&
      (!selector.IsHostPseudoClass() &&
       selector.GetPseudoType() != CSSSelector::kPseudoScope &&
       !context.treat_shadow_host_as_normal_scope &&
       selector.Match() != CSSSelector::kPseudoElement))
    return false;

  switch (selector.Match()) {
    case CSSSelector::kTag:
      return MatchesTagName(element, selector.TagQName());
    case CSSSelector::kClass:
      return element.HasClass() &&
             element.ClassNames().Contains(selector.Value());
    case CSSSelector::kId:
      return element.HasID() &&
             element.IdForStyleResolution() == selector.Value();

    // Attribute selectors
    case CSSSelector::kAttributeExact:
    case CSSSelector::kAttributeSet:
    case CSSSelector::kAttributeHyphen:
    case CSSSelector::kAttributeList:
    case CSSSelector::kAttributeContain:
    case CSSSelector::kAttributeBegin:
    case CSSSelector::kAttributeEnd:
      return AnyAttributeMatches(element, selector.Match(), selector);

    case CSSSelector::kPseudoClass:
      return CheckPseudoClass(context, result);
    case CSSSelector::kPseudoElement:
      return CheckPseudoElement(context, result);

    default:
      NOTREACHED();
      return false;
  }
}

bool SelectorChecker::CheckPseudoNot(const SelectorCheckingContext& context,
                                     MatchResult& result) const {
  const CSSSelector& selector = *context.selector;

  SelectorCheckingContext sub_context(context);
  sub_context.is_sub_selector = true;
  DCHECK(selector.SelectorList());
  for (sub_context.selector = selector.SelectorList()->First();
       sub_context.selector;
       sub_context.selector = sub_context.selector->TagHistory()) {
    // :not cannot nest. I don't really know why this is a
    // restriction in CSS3, but it is, so let's honor it.
    // the parser enforces that this never occurs
    DCHECK_NE(sub_context.selector->GetPseudoType(), CSSSelector::kPseudoNot);
    // We select between :visited and :link when applying. We don't know which
    // one applied (or not) yet.
    if (sub_context.selector->GetPseudoType() == CSSSelector::kPseudoVisited ||
        (sub_context.selector->GetPseudoType() == CSSSelector::kPseudoLink &&
         sub_context.visited_match_type == kVisitedMatchEnabled))
      return true;
    if (!CheckOne(sub_context, result))
      return true;
  }
  return false;
}

bool SelectorChecker::CheckPseudoClass(const SelectorCheckingContext& context,
                                       MatchResult& result) const {
  Element& element = *context.element;
  const CSSSelector& selector = *context.selector;
  bool force_pseudo_state = false;

  if (context.has_scrollbar_pseudo) {
    // CSS scrollbars match a specific subset of pseudo classes, and they have
    // specialized rules for each
    // (since there are no elements involved).
    return CheckScrollbarPseudoClass(context, result);
  }

  switch (selector.GetPseudoType()) {
    case CSSSelector::kPseudoNot:
      return CheckPseudoNot(context, result);
    case CSSSelector::kPseudoEmpty: {
      bool result = true;
      for (Node* n = element.firstChild(); n; n = n->nextSibling()) {
        if (n->IsElementNode()) {
          result = false;
          break;
        }
        if (n->IsTextNode()) {
          Text* text_node = ToText(n);
          if (!text_node->data().IsEmpty()) {
            result = false;
            break;
          }
        }
      }
      if (mode_ == kResolvingStyle)
        element.SetStyleAffectedByEmpty();
      return result;
    }
    case CSSSelector::kPseudoFirstChild:
      if (mode_ == kResolvingStyle) {
        if (ContainerNode* parent = element.ParentElementOrDocumentFragment())
          parent->SetChildrenAffectedByFirstChildRules();
        element.SetAffectedByFirstChildRules();
      }
      return IsFirstChild(element);
    case CSSSelector::kPseudoFirstOfType:
      if (mode_ == kResolvingStyle) {
        if (ContainerNode* parent = element.ParentElementOrDocumentFragment())
          parent->SetChildrenAffectedByForwardPositionalRules();
      }
      return IsFirstOfType(element, element.TagQName());
    case CSSSelector::kPseudoLastChild: {
      ContainerNode* parent = element.ParentElementOrDocumentFragment();
      if (mode_ == kResolvingStyle) {
        if (parent)
          parent->SetChildrenAffectedByLastChildRules();
        element.SetAffectedByLastChildRules();
      }
      if (mode_ != kQueryingRules && parent &&
          !parent->IsFinishedParsingChildren())
        return false;
      return IsLastChild(element);
    }
    case CSSSelector::kPseudoLastOfType: {
      ContainerNode* parent = element.ParentElementOrDocumentFragment();
      if (mode_ == kResolvingStyle) {
        if (parent)
          parent->SetChildrenAffectedByBackwardPositionalRules();
      }
      if (mode_ != kQueryingRules && parent &&
          !parent->IsFinishedParsingChildren())
        return false;
      return IsLastOfType(element, element.TagQName());
    }
    case CSSSelector::kPseudoOnlyChild: {
      ContainerNode* parent = element.ParentElementOrDocumentFragment();
      if (mode_ == kResolvingStyle) {
        if (parent) {
          parent->SetChildrenAffectedByFirstChildRules();
          parent->SetChildrenAffectedByLastChildRules();
        }
        element.SetAffectedByFirstChildRules();
        element.SetAffectedByLastChildRules();
      }
      if (mode_ != kQueryingRules && parent &&
          !parent->IsFinishedParsingChildren())
        return false;
      return IsFirstChild(element) && IsLastChild(element);
    }
    case CSSSelector::kPseudoOnlyOfType: {
      // FIXME: This selector is very slow.
      ContainerNode* parent = element.ParentElementOrDocumentFragment();
      if (mode_ == kResolvingStyle && parent) {
        parent->SetChildrenAffectedByForwardPositionalRules();
        parent->SetChildrenAffectedByBackwardPositionalRules();
      }
      if (mode_ != kQueryingRules && parent &&
          !parent->IsFinishedParsingChildren())
        return false;
      return IsFirstOfType(element, element.TagQName()) &&
             IsLastOfType(element, element.TagQName());
    }
    case CSSSelector::kPseudoPlaceholderShown:
#ifndef BLINKIT_CRAWLER_ONLY
      ASSERT(false); // BKTODO:
#if 0
      if (auto* text_control = ToTextControlOrNull(element))
        return text_control->IsPlaceholderVisible();
#endif
#endif
      break;
    case CSSSelector::kPseudoNthChild:
      if (mode_ == kResolvingStyle) {
        if (ContainerNode* parent = element.ParentElementOrDocumentFragment())
          parent->SetChildrenAffectedByForwardPositionalRules();
      }
      return selector.MatchNth(NthIndexCache::NthChildIndex(element));
    case CSSSelector::kPseudoNthOfType:
      if (mode_ == kResolvingStyle) {
        if (ContainerNode* parent = element.ParentElementOrDocumentFragment())
          parent->SetChildrenAffectedByForwardPositionalRules();
      }
      return selector.MatchNth(NthIndexCache::NthOfTypeIndex(element));
    case CSSSelector::kPseudoNthLastChild: {
      ContainerNode* parent = element.ParentElementOrDocumentFragment();
      if (mode_ == kResolvingStyle && parent)
        parent->SetChildrenAffectedByBackwardPositionalRules();
      if (mode_ != kQueryingRules && parent &&
          !parent->IsFinishedParsingChildren())
        return false;
      return selector.MatchNth(NthIndexCache::NthLastChildIndex(element));
    }
    case CSSSelector::kPseudoNthLastOfType: {
      ContainerNode* parent = element.ParentElementOrDocumentFragment();
      if (mode_ == kResolvingStyle && parent)
        parent->SetChildrenAffectedByBackwardPositionalRules();
      if (mode_ != kQueryingRules && parent &&
          !parent->IsFinishedParsingChildren())
        return false;
      return selector.MatchNth(NthIndexCache::NthLastOfTypeIndex(element));
    }
    case CSSSelector::kPseudoTarget:
#ifdef BLINKIT_CRAWLER_ONLY
      return false;
#else
      ASSERT(false); // BKTODO:
      return false;
#if 0
      return element == element.GetDocument().CssTarget();
#endif
#endif
    case CSSSelector::kPseudoAny: {
      SelectorCheckingContext sub_context(context);
      sub_context.is_sub_selector = true;
      DCHECK(selector.SelectorList());
      for (sub_context.selector = selector.SelectorList()->First();
           sub_context.selector; sub_context.selector = CSSSelectorList::Next(
                                     *sub_context.selector)) {
        if (Match(sub_context))
          return true;
      }
    } break;
    case CSSSelector::kPseudoAutofill:
#ifdef BLINKIT_CRAWLER_ONLY
      return false;
#else
      ASSERT(false); // BKTODO:
      return false;
#if 0
      return element.IsFormControlElement() &&
             ToHTMLFormControlElement(element).IsAutofilled();
#endif
#endif
    case CSSSelector::kPseudoAutofillPreviewed:
#ifdef BLINKIT_CRAWLER_ONLY
      return false;
#else
      ASSERT(false); // BKTODO:
      return false;
#if 0
      return element.IsFormControlElement() &&
             ToHTMLFormControlElement(element).GetAutofillState() ==
                 WebAutofillState::kPreviewed;
#endif
#endif
    case CSSSelector::kPseudoAutofillSelected:
#ifdef BLINKIT_CRAWLER_ONLY
      return false;
#else
      ASSERT(false); // BKTODO:
      return false;
#if 0
      return element.IsFormControlElement() &&
             ToHTMLFormControlElement(element).GetAutofillState() ==
                 WebAutofillState::kAutofilled;
#endif
#endif
    case CSSSelector::kPseudoAnyLink:
    case CSSSelector::kPseudoWebkitAnyLink:
    case CSSSelector::kPseudoLink:
      return element.IsLink();
    case CSSSelector::kPseudoVisited:
      return element.IsLink() &&
             context.visited_match_type == kVisitedMatchEnabled;
    case CSSSelector::kPseudoDrag:
#ifdef BLINKIT_CRAWLER_ONLY
      return false;
#else
      ASSERT(false); // BKTODO:
      return false;
#if 0
      if (mode_ == kResolvingStyle) {
        if (context.in_rightmost_compound)
          element_style_->SetAffectedByDrag();
        else
          element.SetChildrenOrSiblingsAffectedByDrag();
      }
      return element.IsDragged();
#endif
#endif
    case CSSSelector::kPseudoFocus:
      if (mode_ == kResolvingStyle && !context.in_rightmost_compound)
        element.SetChildrenOrSiblingsAffectedByFocus();
      return MatchesFocusPseudoClass(element);
    case CSSSelector::kPseudoFocusVisible:
      if (mode_ == kResolvingStyle && !context.in_rightmost_compound)
        element.SetChildrenOrSiblingsAffectedByFocusVisible();
      return MatchesFocusVisiblePseudoClass(element);
    case CSSSelector::kPseudoFocusWithin:
#ifdef BLINKIT_CRAWLER_ONLY
      return false;
#else
      ASSERT(false); // BKTODO:
      return false;
#if 0
      if (mode_ == kResolvingStyle) {
        if (context.in_rightmost_compound)
          element_style_->SetAffectedByFocusWithin();
        else
          element.SetChildrenOrSiblingsAffectedByFocusWithin();
      }
      if (force_pseudo_state)
        return true;
      return element.HasFocusWithin();
#endif
#endif
    case CSSSelector::kPseudoHover:
#ifdef BLINKIT_CRAWLER_ONLY
      return false;
#else
      ASSERT(false); // BKTODO:
      return false;
#if 0
      if (mode_ == kResolvingStyle) {
        if (context.in_rightmost_compound)
          element_style_->SetAffectedByHover();
        else
          element.SetChildrenOrSiblingsAffectedByHover();
      }
      if (!ShouldMatchHoverOrActive(context))
        return false;
      if (force_pseudo_state)
        return true;
      return element.IsHovered();
#endif
#endif
    case CSSSelector::kPseudoActive:
#ifdef BLINKIT_CRAWLER_ONLY
      return false;
#else
      if (mode_ == kResolvingStyle) {
        ASSERT(false); // BKTODO:
#if 0
        if (context.in_rightmost_compound)
          element_style_->SetAffectedByActive();
        else
          element.SetChildrenOrSiblingsAffectedByActive();
#endif
      }
      if (!ShouldMatchHoverOrActive(context))
        return false;
      if (force_pseudo_state)
        return true;
      return element.IsActive();
#endif
    case CSSSelector::kPseudoEnabled:
      return element.MatchesEnabledPseudoClass();
    case CSSSelector::kPseudoFullPageMedia:
      return false;
    case CSSSelector::kPseudoDefault:
      return element.MatchesDefaultPseudoClass();
    case CSSSelector::kPseudoDisabled:
      return element.IsDisabledFormControl();
    case CSSSelector::kPseudoReadOnly:
      return element.MatchesReadOnlyPseudoClass();
    case CSSSelector::kPseudoReadWrite:
      return element.MatchesReadWritePseudoClass();
    case CSSSelector::kPseudoOptional:
      return element.IsOptionalFormControl();
    case CSSSelector::kPseudoRequired:
      return element.IsRequiredFormControl();
    case CSSSelector::kPseudoValid:
#ifdef BLINKIT_CRAWLER_ONLY
      return false;
#else
      ASSERT(false); // BKTODO:
#if 0
      if (mode_ == kResolvingStyle)
        element.GetDocument().SetContainsValidityStyleRules();
#endif
      return element.MatchesValidityPseudoClasses() && element.IsValidElement();
#endif
    case CSSSelector::kPseudoInvalid:
#ifdef BLINKIT_CRAWLER_ONLY
      return false;
#else
      ASSERT(false); // BKTODO:
#if 0
      if (mode_ == kResolvingStyle)
        element.GetDocument().SetContainsValidityStyleRules();
#endif
      return element.MatchesValidityPseudoClasses() &&
             !element.IsValidElement();
#endif
    case CSSSelector::kPseudoChecked: {
#ifdef BLINKIT_CRAWLER_ONLY
      return false; // BKTODO: Process form element classes.
#else
      ASSERT(false); // BKTODO:
      return false;
#if 0
      if (auto* input_element = ToHTMLInputElementOrNull(element)) {
        // Even though WinIE allows checked and indeterminate to
        // co-exist, the CSS selector spec says that you can't be
        // both checked and indeterminate. We will behave like WinIE
        // behind the scenes and just obey the CSS spec here in the
        // test for matching the pseudo.
        if (input_element->ShouldAppearChecked() &&
            !input_element->ShouldAppearIndeterminate())
          return true;
      } else if (IsHTMLOptionElement(element) &&
                 ToHTMLOptionElement(element).Selected()) {
        return true;
      }
      break;
#endif
#endif
    }
    case CSSSelector::kPseudoIndeterminate:
      return element.ShouldAppearIndeterminate();
    case CSSSelector::kPseudoRoot:
      return element == element.GetDocument().documentElement();
    case CSSSelector::kPseudoLang: {
      AtomicString value = element.ComputeInheritedLanguage();
      const AtomicString& argument = selector.Argument();
      if (value.IsEmpty() ||
          !value.StartsWith(argument, kTextCaseASCIIInsensitive))
        break;
      if (value.length() != argument.length() &&
          value[argument.length()] != '-')
        break;
      return true;
    }
    case CSSSelector::kPseudoFullscreen:
    case CSSSelector::kPseudoFullScreen:
    case CSSSelector::kPseudoFullScreenAncestor:
    case CSSSelector::kPseudoVideoPersistent:
    case CSSSelector::kPseudoVideoPersistentAncestor:
      return false;
    case CSSSelector::kPseudoInRange:
#ifdef BLINKIT_CRAWLER_ONLY
      return false;
#else
      ASSERT(false); // BKTODO:
      return false;
#if 0
      if (mode_ == kResolvingStyle)
        element.GetDocument().SetContainsValidityStyleRules();
      return element.IsInRange();
#endif
#endif
    case CSSSelector::kPseudoOutOfRange:
#ifdef BLINKIT_CRAWLER_ONLY
      return false;
#else
      ASSERT(false); // BKTODO:
      return false;
#if 0
      if (mode_ == kResolvingStyle)
        element.GetDocument().SetContainsValidityStyleRules();
      return element.IsOutOfRange();
#endif
#endif
    case CSSSelector::kPseudoFutureCue:
    case CSSSelector::kPseudoPastCue:
      return false;
    case CSSSelector::kPseudoScope:
      if (!context.scope)
        return false;
      if (context.scope == &element.GetDocument())
        return element == element.GetDocument().documentElement();
#ifndef BLINKIT_CRAWLER_ONLY
      if (context.scope->IsShadowRoot())
        return element == ToShadowRoot(context.scope)->host();
#endif
      return context.scope == &element;
    case CSSSelector::kPseudoUnresolved:
#ifdef BLINKIT_CRAWLER_ONLY
      return false;
#else
      ASSERT(false); // BKTODO:
      return false;
#if 0
      return !element.IsDefined() && element.IsUnresolvedV0CustomElement();
#endif
#endif
    case CSSSelector::kPseudoDefined:
#ifdef BLINKIT_CRAWLER_ONLY
      return false;
#else
      ASSERT(false); // BKTODO:
      return false;
#if 0
      return element.IsDefined() || element.IsUpgradedV0CustomElement();
#endif
#endif
    case CSSSelector::kPseudoHost:
    case CSSSelector::kPseudoHostContext:
      return CheckPseudoHost(context, result);
    case CSSSelector::kPseudoSpatialNavigationFocus:
      DCHECK(is_ua_rule_);
      return MatchesSpatialNavigationFocusPseudoClass(element);
    case CSSSelector::kPseudoIsHtml:
      DCHECK(is_ua_rule_);
      return element.GetDocument().IsHTMLDocument();
    case CSSSelector::kPseudoListBox:
      DCHECK(is_ua_rule_);
      return MatchesListBoxPseudoClass(element);
    case CSSSelector::kPseudoHostHasAppearance:
#ifndef BLINKIT_CRAWLER_ONLY
      DCHECK(is_ua_rule_);
      if (ShadowRoot* root = element.ContainingShadowRoot()) {
        ASSERT(false); // BKTODO:
#if 0
        if (!root->IsUserAgent())
          return false;
#endif
        const ComputedStyle* style = root->host().GetComputedStyle();
        return style && style->HasAppearance();
      }
#endif
      return false;
    case CSSSelector::kPseudoWindowInactive:
#ifdef BLINKIT_CRAWLER_ONLY
      return false;
#else
      if (!context.has_selection_pseudo)
        return false;
      ASSERT(false); // BKTODO:
      return false;
#if 0
      return !element.GetDocument().GetPage()->GetFocusController().IsActive();
#endif
#endif
    case CSSSelector::kPseudoHorizontal:
    case CSSSelector::kPseudoVertical:
    case CSSSelector::kPseudoDecrement:
    case CSSSelector::kPseudoIncrement:
    case CSSSelector::kPseudoStart:
    case CSSSelector::kPseudoEnd:
    case CSSSelector::kPseudoDoubleButton:
    case CSSSelector::kPseudoSingleButton:
    case CSSSelector::kPseudoNoButton:
    case CSSSelector::kPseudoCornerPresent:
      return false;
    case CSSSelector::kPseudoUnknown:
    case CSSSelector::kPseudoMatches:
    case CSSSelector::kPseudoIS:
    default:
      NOTREACHED();
      break;
  }
  return false;
}

bool SelectorChecker::CheckPseudoElement(const SelectorCheckingContext& context,
                                         MatchResult& result) const {
  const CSSSelector& selector = *context.selector;
  Element& element = *context.element;

  switch (selector.GetPseudoType()) {
    case CSSSelector::kPseudoCue: {
      SelectorCheckingContext sub_context(context);
      sub_context.is_sub_selector = true;
      sub_context.scope = nullptr;
      sub_context.treat_shadow_host_as_normal_scope = false;

      for (sub_context.selector = selector.SelectorList()->First();
           sub_context.selector; sub_context.selector = CSSSelectorList::Next(
                                     *sub_context.selector)) {
        if (Match(sub_context))
          return true;
      }
      return false;
    }
    case CSSSelector::kPseudoPart:
      if (!RuntimeEnabledFeatures::CSSPartPseudoElementEnabled())
        return false;
#ifdef BLINKIT_CRAWLER_ONLY
      ASSERT(false); // BKTODO:
      return false;
#else
      DCHECK(part_names_);
      return part_names_->Contains(selector.Argument());
#endif
    case CSSSelector::kPseudoPlaceholder:
#ifndef BLINKIT_CRAWLER_ONLY
      if (ShadowRoot* root = element.ContainingShadowRoot()) {
        ASSERT(false); // BKTODO:
#if 0
        return root->IsUserAgent() &&
               element.ShadowPseudoId() == "-webkit-input-placeholder";
#endif
      }
#endif
      return false;
    case CSSSelector::kPseudoWebKitCustomElement: {
#ifndef BLINKIT_CRAWLER_ONLY
      ASSERT(false); // BKTODO:
#if 0
      if (ShadowRoot* root = element.ContainingShadowRoot())
        return root->IsUserAgent() &&
               element.ShadowPseudoId() == selector.Value();
#endif
#endif
      return false;
    }
    case CSSSelector::kPseudoBlinkInternalElement:
#ifndef BLINKIT_CRAWLER_ONLY
      DCHECK(is_ua_rule_);
      ASSERT(false); // BKTODO:
#if 0
      if (ShadowRoot* root = element.ContainingShadowRoot())
        return root->IsUserAgent() &&
               element.ShadowPseudoId() == selector.Value();
#endif
#endif
      return false;
    case CSSSelector::kPseudoSlotted: {
      SelectorCheckingContext sub_context(context);
      sub_context.is_sub_selector = true;
      sub_context.scope = nullptr;
      sub_context.treat_shadow_host_as_normal_scope = false;

      // ::slotted() only allows one compound selector.
      DCHECK(selector.SelectorList()->First());
      DCHECK(!CSSSelectorList::Next(*selector.SelectorList()->First()));
      sub_context.selector = selector.SelectorList()->First();
      MatchResult sub_result;
      if (!Match(sub_context, sub_result))
        return false;
      result.specificity += sub_context.selector->Specificity() +
                            sub_result.specificity +
                            CSSSelector::kTagSpecificity;
      return true;
    }
    case CSSSelector::kPseudoContent:
#ifdef BLINKIT_CRAWLER_ONLY
      return false;
#else
      return element.IsInShadowTree() && element.IsV0InsertionPoint();
#endif
    case CSSSelector::kPseudoShadow:
      return element.IsInShadowTree() && context.previous_element;
    default:
      DCHECK_NE(mode_, kQueryingRules);
      result.dynamic_pseudo =
          CSSSelector::GetPseudoId(selector.GetPseudoType());
      DCHECK_NE(result.dynamic_pseudo, kPseudoIdNone);
      return true;
  }
}

bool SelectorChecker::CheckPseudoHost(const SelectorCheckingContext& context,
                                      MatchResult& result) const {
#ifndef BLINKIT_CRAWLER_ONLY
  const CSSSelector& selector = *context.selector;
  Element& element = *context.element;

  // :host only matches a shadow host when :host is in a shadow tree of the
  // shadow host.
  if (!context.scope)
    return false;
  const ContainerNode* shadow_host = context.scope->OwnerShadowHost();
  if (!shadow_host || shadow_host != element)
    return false;
  DCHECK(IsShadowHost(element));
  DCHECK(element.GetShadowRoot());
  ASSERT(false); // BKTODO:
#if 0
  bool is_v1_shadow = element.GetShadowRoot()->IsV1();

  // For the case with no parameters, i.e. just :host.
  if (!selector.SelectorList()) {
    if (is_v1_shadow)
      result.specificity += CSSSelector::kClassLikeSpecificity;
    return true;
  }

  SelectorCheckingContext sub_context(context);
  sub_context.is_sub_selector = true;

  bool matched = false;
  unsigned max_specificity = 0;

  // If one of simple selectors matches an element, returns SelectorMatches.
  // Just "OR".
  for (sub_context.selector = selector.SelectorList()->First();
       sub_context.selector;
       sub_context.selector = CSSSelectorList::Next(*sub_context.selector)) {
    sub_context.treat_shadow_host_as_normal_scope = true;
    sub_context.scope = context.scope;
    // Use FlatTreeTraversal to traverse a composed ancestor list of a given
    // element.
    Element* next_element = &element;
    SelectorCheckingContext host_context(sub_context);
    do {
      MatchResult sub_result;
      host_context.element = next_element;
      if (Match(host_context, sub_result)) {
        matched = true;
        // Consider div:host(div:host(div:host(div:host...))).
        max_specificity =
            std::max(max_specificity, host_context.selector->Specificity() +
                                          sub_result.specificity);
        break;
      }
      host_context.treat_shadow_host_as_normal_scope = false;
      host_context.scope = nullptr;

      if (selector.GetPseudoType() == CSSSelector::kPseudoHost)
        break;

      host_context.in_rightmost_compound = false;
      next_element = FlatTreeTraversal::ParentElement(*next_element);
    } while (next_element);
  }
  if (matched) {
    result.specificity += max_specificity;
    if (is_v1_shadow)
      result.specificity += CSSSelector::kClassLikeSpecificity;
    return true;
  }
#endif
#endif

  // FIXME: this was a fallthrough condition.
  return false;
}

bool SelectorChecker::CheckScrollbarPseudoClass(
    const SelectorCheckingContext& context,
    MatchResult& result) const {
  const CSSSelector& selector = *context.selector;

  if (selector.GetPseudoType() == CSSSelector::kPseudoNot)
    return CheckPseudoNot(context, result);

#ifdef BLINKIT_CRAWLER_ONLY
  return false;
#else
  ASSERT(false); // BKTODO:
#if 0
  // FIXME: This is a temporary hack for resizers and scrollbar corners.
  // Eventually :window-inactive should become a real
  // pseudo class and just apply to everything.
  if (selector.GetPseudoType() == CSSSelector::kPseudoWindowInactive)
    return !context.element->GetDocument()
                .GetPage()
                ->GetFocusController()
                .IsActive();
#endif

  if (!scrollbar_)
    return false;

  switch (selector.GetPseudoType()) {
    case CSSSelector::kPseudoEnabled:
      return scrollbar_->Enabled();
    case CSSSelector::kPseudoDisabled:
      return !scrollbar_->Enabled();
    case CSSSelector::kPseudoHover: {
      ScrollbarPart hovered_part = scrollbar_->HoveredPart();
      if (scrollbar_part_ == kScrollbarBGPart)
        return hovered_part != kNoPart;
      if (scrollbar_part_ == kTrackBGPart)
        return hovered_part == kBackTrackPart ||
               hovered_part == kForwardTrackPart || hovered_part == kThumbPart;
      return scrollbar_part_ == hovered_part;
    }
    case CSSSelector::kPseudoActive: {
      ScrollbarPart pressed_part = scrollbar_->PressedPart();
      if (scrollbar_part_ == kScrollbarBGPart)
        return pressed_part != kNoPart;
      if (scrollbar_part_ == kTrackBGPart)
        return pressed_part == kBackTrackPart ||
               pressed_part == kForwardTrackPart || pressed_part == kThumbPart;
      return scrollbar_part_ == pressed_part;
    }
    case CSSSelector::kPseudoHorizontal:
      return scrollbar_->Orientation() == kHorizontalScrollbar;
    case CSSSelector::kPseudoVertical:
      return scrollbar_->Orientation() == kVerticalScrollbar;
    case CSSSelector::kPseudoDecrement:
      return scrollbar_part_ == kBackButtonStartPart ||
             scrollbar_part_ == kBackButtonEndPart ||
             scrollbar_part_ == kBackTrackPart;
    case CSSSelector::kPseudoIncrement:
      return scrollbar_part_ == kForwardButtonStartPart ||
             scrollbar_part_ == kForwardButtonEndPart ||
             scrollbar_part_ == kForwardTrackPart;
    case CSSSelector::kPseudoStart:
      return scrollbar_part_ == kBackButtonStartPart ||
             scrollbar_part_ == kForwardButtonStartPart ||
             scrollbar_part_ == kBackTrackPart;
    case CSSSelector::kPseudoEnd:
      return scrollbar_part_ == kBackButtonEndPart ||
             scrollbar_part_ == kForwardButtonEndPart ||
             scrollbar_part_ == kForwardTrackPart;
    case CSSSelector::kPseudoDoubleButton: {
      ASSERT(false); // BKTODO:
#if 0
      WebScrollbarButtonsPlacement buttons_placement =
          scrollbar_->GetTheme().ButtonsPlacement();
      if (scrollbar_part_ == kBackButtonStartPart ||
          scrollbar_part_ == kForwardButtonStartPart ||
          scrollbar_part_ == kBackTrackPart)
        return buttons_placement == kWebScrollbarButtonsPlacementDoubleStart ||
               buttons_placement == kWebScrollbarButtonsPlacementDoubleBoth;
      if (scrollbar_part_ == kBackButtonEndPart ||
          scrollbar_part_ == kForwardButtonEndPart ||
          scrollbar_part_ == kForwardTrackPart)
        return buttons_placement == kWebScrollbarButtonsPlacementDoubleEnd ||
               buttons_placement == kWebScrollbarButtonsPlacementDoubleBoth;
#endif
      return false;
    }
    case CSSSelector::kPseudoSingleButton: {
      ASSERT(false); // BKTODO:
#if 0
      WebScrollbarButtonsPlacement buttons_placement =
          scrollbar_->GetTheme().ButtonsPlacement();
      if (scrollbar_part_ == kBackButtonStartPart ||
          scrollbar_part_ == kForwardButtonEndPart ||
          scrollbar_part_ == kBackTrackPart ||
          scrollbar_part_ == kForwardTrackPart)
        return buttons_placement == kWebScrollbarButtonsPlacementSingle;
#endif
      return false;
    }
    case CSSSelector::kPseudoNoButton: {
      ASSERT(false); // BKTODO:
#if 0
      WebScrollbarButtonsPlacement buttons_placement =
          scrollbar_->GetTheme().ButtonsPlacement();
      if (scrollbar_part_ == kBackTrackPart)
        return buttons_placement == kWebScrollbarButtonsPlacementNone ||
               buttons_placement == kWebScrollbarButtonsPlacementDoubleEnd;
      if (scrollbar_part_ == kForwardTrackPart)
        return buttons_placement == kWebScrollbarButtonsPlacementNone ||
               buttons_placement == kWebScrollbarButtonsPlacementDoubleStart;
#endif
      return false;
    }
    case CSSSelector::kPseudoCornerPresent:
      return scrollbar_->GetScrollableArea() &&
             scrollbar_->GetScrollableArea()->IsScrollCornerVisible();
    default:
      return false;
  }
#endif
}

bool SelectorChecker::MatchesFocusPseudoClass(const Element& element) {
#ifdef BLINKIT_CRAWLER_ONLY
  return false;
#else
  return element.IsFocused() && IsFrameFocused(element);
#endif
}

bool SelectorChecker::MatchesFocusVisiblePseudoClass(const Element& element) {
#ifdef BLINKIT_CRAWLER_ONLY
  return false;
#else
  const Document& document = element.GetDocument();
  ASSERT(false); // BKTODO:
  return false;
#if 0
  bool always_show_focus_ring = element.MayTriggerVirtualKeyboard();
  bool last_focus_from_mouse =
      document.GetFrame() &&
      document.GetFrame()->Selection().FrameIsFocusedAndActive() &&
      document.LastFocusType() == kWebFocusTypeMouse;
  bool had_keyboard_event = document.HadKeyboardEvent();

  return element.IsFocused() && (!last_focus_from_mouse || had_keyboard_event ||
                                 always_show_focus_ring);
#endif
#endif
}

}  // namespace blink
