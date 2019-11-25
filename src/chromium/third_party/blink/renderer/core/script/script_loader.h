// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_loader.h
// Description: ScriptLoader Class
//      Author: Ziming Li
//     Created: 2019-10-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 2008 Nikolas Zimmermann <zimmermann@kde.org>
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

#ifndef BLINKIT_BLINK_SCRIPT_LOADER_H
#define BLINKIT_BLINK_SCRIPT_LOADER_H

#pragma once

#include "third_party/blink/renderer/core/script/pending_script.h"
#include "third_party/blink/renderer/core/script/script.h"
#include "third_party/blink/renderer/core/script/script_scheduling_type.h"
#include "third_party/blink/renderer/platform/bindings/name_client.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

namespace blink {

class ScriptLoader final : public GarbageCollectedFinalized<ScriptLoader>
                         // BKTODO:, public PendingScriptClient
                         , public NameClient
{
public:
    ~ScriptLoader(void); // BKTODO: override;

    enum LegacyTypeSupport {
        kDisallowLegacyTypeInTypeAttribute,
        kAllowLegacyTypeInTypeAttribute
    };
    static bool IsValidScriptTypeAndLanguage(const String &typeAttributeValue, const String &languageAttributeValue,
        LegacyTypeSupport supportLegacyTypes, ScriptType &outScriptType);

    static bool BlockForNoModule(ScriptType scriptType, bool nomodule);

    std::shared_ptr<PendingScript> TakePendingScript(ScriptSchedulingType schedulingType);
};

}  // namespace blink

#endif // BLINKIT_BLINK_SCRIPT_LOADER_H
