// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: UseCounter.h
// Description: UseCounter Class
//      Author: Ziming Li
//     Created: 2019-02-05
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_USE_COUNTER_H
#define BLINKIT_BLINKIT_USE_COUNTER_H

namespace blink {

class Document;
class ExecutionContext;
class Frame;

class UseCounter
{
public:
    enum Feature {
        AnimationConstructorKeyframeListEffectNoTiming,
        AnimationConstructorKeyframeListEffectObjectTiming,
        AnimationFinishEvent,
        AspectRatioFlexItem,
        AttrCloneNode,
        AttrGetValue,
        AttrSetValue,
        AttrSetValueWithElement,
        AutocapitalizeAttribute,
        AutoFocusAttribute,
        BorderImageWithBorderStyleNone,
        CaseInsensitiveAttrSelectorMatch,
        ClientHintsContentDPR,
        ClipPathOfPositionedElement,
        CSSAnimationsStackedNeutralKeyframe,
        CSSDeepCombinator,
        CSSSelectorPseudoScrollbar,
        CSSSelectorPseudoScrollbarButton,
        CSSSelectorPseudoScrollbarThumb,
        CSSSelectorPseudoScrollbarTrack,
        CSSSelectorPseudoScrollbarTrackPiece,
        CSSSelectorPseudoShadow,
        DataElement,
        DataListElement,
        DemotedFormElement,
        DOMFocusInOutEvent,
        DOMStringList_Contains_Method_IndexedDB,
        DOMStringList_Contains_Method_Location,
        DOMStringList_Item_AttributeGetter_IndexedDB,
        DOMStringList_Item_AttributeGetter_Location,
        DOMWindowOpen,
        DOMWindowOpenFeatures,
        DocumentAll,
        DocumentBeforeUnloadFired,
        DocumentBeforeUnloadRegistered,
        DocumentUnloadFired,
        DocumentUnloadRegistered,
        EditingAppleTabSpanClass,
        ElementCreateShadowRootMultiple,
        ElementCreateShadowRootMultipleWithUserAgentShadowRoot,
        ExecCommandOnInputOrTextarea,
        FlexboxIntrinsicSizeAlgorithmIsDifferent,
        FlexboxPercentageMarginVertical,
        FlexboxPercentagePaddingVertical,
        FocusInOutEvent,
        FormAssociationByParser,
        FormAttribute,
        FormElement,
        FormNameAccessForImageElement,
        FormNameAccessForNonDescendantImageElement,
        FormNameAccessForPastNamesMap,
        HTMLDocumentCreateAttributeNameNotLowercase,
        HTMLElementDeprecatedWidth,
        HTMLImports,
        HTMLImportsAsyncAttribute,
        HTMLImportsHasStyleSheets,
        HTMLLabelElementFormContentAttribute,
        InnerTextWithShadowTree,
        InputTypeSearch,
        LangAttribute,
        LangAttributeDoesNotMatchToUILocale,
        LangAttributeOnBody,
        LangAttributeOnHTML,
        LineClamp,
        MenuItemCloseTag,
        MenuItemElement,
        MixedContentFormPresent,
        NonCSSStyleSheetType,
        NonHTMLElementSetAttributeNodeFromHTMLDocumentNameNotLowercase,
        PlaceholderAttribute,
        PrefixedDevicePixelRatioMediaFeature,
        PrefixedMaxDevicePixelRatioMediaFeature,
        PrefixedMinDevicePixelRatioMediaFeature,
        PrefixedTransform3dMediaFeature,
        PrefixedTransitionEndEvent,
        PseudoBeforeAfterForInputElement,
        Reflection,
        RequiredAttribute,
        ScriptElementWithInvalidTypeHasSrc,
        SelectionCollapseNull,
        SelectionSetBaseAndExtentNull,
        SelectionToStringWithShadowTree,
        SrcsetWDescriptor,
        SrcsetXDescriptor,
        StyleSheetListAnonymousNamedGetter,
        SubFrameBeforeUnloadRegistered,
        SyntheticKeyframesInCompositedCSSAnimation,
        TextInputFired,
        TimeElement,
        UnprefixedTransitionEndEvent,
        WindowFindWithShadowTree,
    };
    static void count(const Frame *, Feature) {}
    static void count(const Document &, Feature) {}
    static void count(const ExecutionContext *, Feature) {}

    static void countDeprecation(const Document &, Feature) {}
};

} // namespace blink

#endif // BLINKIT_BLINKIT_USE_COUNTER_H