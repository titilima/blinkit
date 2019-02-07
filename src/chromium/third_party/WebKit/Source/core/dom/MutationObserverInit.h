// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: MutationObserverInit.h
// Description: MutationObserverInit Class
//      Author: Ziming Li
//     Created: 2018-07-23
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINK_MUTATION_OBSERVER_INIT_H
#define BLINKIT_BLINK_MUTATION_OBSERVER_INIT_H

#pragma once

#include <optional>
#include "wtf/Vector.h"
#include "wtf/text/WTFString.h"

namespace blink {

class MutationObserverInit
{
public:
    bool childList(void) const { return m_childList; }

    bool hasAttributes(void) const { return m_attributes.has_value(); }
    bool attributes(void) const {
        assert(m_attributes.has_value());
        return m_attributes.value();
    }

    bool hasCharacterData(void) const { return m_characterData.has_value(); }
    bool characterData(void) const {
        assert(m_characterData.has_value());
        return m_characterData.value();
    }

    bool subtree(void) const { return m_subtree; }

    bool hasAttributeOldValue(void) const { return m_attributeOldValue.has_value(); }
    bool attributeOldValue(void) const {
        assert(m_attributeOldValue.has_value());
        return m_attributeOldValue.value();
    }

    bool hasCharacterDataOldValue(void) const { return m_characterDataOldValue.has_value(); }
    bool characterDataOldValue(void) const {
        assert(m_characterDataOldValue.has_value());
        return m_characterDataOldValue.value();
    }

    bool hasAttributeFilter(void) const { return m_attributeFilter.has_value(); }
    const auto& attributeFilter(void) const {
        assert(m_attributeFilter.has_value());
        return m_attributeFilter.value();
    }
private:
    bool m_childList = false;
    std::optional<bool> m_attributes;
    std::optional<bool> m_characterData;
    bool m_subtree = false;
    std::optional<bool> m_attributeOldValue;
    std::optional<bool> m_characterDataOldValue;
    std::optional<Vector<String>> m_attributeFilter;
};

} // namespace blink

#endif // BLINKIT_BLINK_MUTATION_OBSERVER_INIT_H
