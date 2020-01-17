/*
 * Copyright (C) 2008 Nikolas Zimmermann <zimmermann@kde.org>
 * Copyright (C) 2013 Google Inc. All rights reserved.
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
 *
 */
#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_SCRIPT_SCRIPT_ELEMENT_BASE_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_SCRIPT_SCRIPT_ELEMENT_BASE_H_

#include "third_party/blink/renderer/core/core_export.h"
#include "third_party/blink/renderer/core/dom/element.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/heap/heap.h"
#include "third_party/blink/renderer/platform/wtf/text/atomic_string.h"
#include "third_party/blink/renderer/platform/wtf/text/text_position.h"
#include "third_party/blink/renderer/platform/wtf/text/wtf_string.h"

namespace blink {

class HTMLScriptElementOrSVGScriptElement;
class ScriptLoader;

ScriptLoader* ScriptLoaderFromElement(Element*);

class CORE_EXPORT ScriptElementBase : public GarbageCollectedMixin {
 public:
  virtual ~ScriptElementBase(void);

  ScriptLoader* Loader(void) const { return m_loader.get(); }

  bool AsyncAttributeValue(void) const { return false; } // Seems no need to support `async` scripts.
  String CharsetAttributeValue(void) const;
#if 0 // BKTODO:
  bool DeferAttributeValue(void) const { return false; }
  String EventAttributeValue(void) const;
  virtual String ForAttributeValue() const = 0;
  virtual String IntegrityAttributeValue() const = 0;
#endif
  String LanguageAttributeValue(void) const;
#if 0 // BKTODO:
  virtual bool NomoduleAttributeValue() const = 0;
  virtual String SourceAttributeValue() const = 0;
#endif
  String TypeAttributeValue(void) const;
#if 0 // BKTODO:
  virtual String ReferrerPolicyAttributeValue() const = 0;
#endif

  String TextFromChildren(void) const;
  bool HasSourceAttribute(void) const;
  bool IsConnected(void) const;
#if 0 // BKTODO:
  virtual bool HasChildren() const = 0;
  virtual const AtomicString& GetNonceForElement() const = 0;
  virtual bool ElementHasDuplicateAttributes() const = 0;
#endif

  Document& GetDocument(void) const { return GetElement().GetDocument(); }
#if 0 // BKTODO:
  virtual void SetScriptElementForBinding(
      HTMLScriptElementOrSVGScriptElement&) = 0;

  virtual void DispatchLoadEvent() = 0;
  virtual void DispatchErrorEvent() = 0;
#endif

 protected:
  ScriptElementBase(bool parserInserted, bool alreadyStarted);

  Node::InsertionNotificationRequest InsertedIntoImpl(ContainerNode &insertionPoint);
  void DidNotifySubtreeInsertionsToDocumentImpl(void);
  void ChildrenChangedImpl(const ContainerNode::ChildrenChange &change);
  bool ParseAttributeImpl(const Element::AttributeModificationParams &params);
private:
  virtual const Element& GetElement(void) const = 0;

  std::unique_ptr<ScriptLoader> m_loader;
};

}  // namespace blink

#endif
