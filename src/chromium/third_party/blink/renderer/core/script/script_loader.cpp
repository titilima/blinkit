// -------------------------------------------------
// BlinKit - blink Library
// -------------------------------------------------
//   File Name: script_loader.cpp
// Description: ScriptLoader Class
//      Author: Ziming Li
//     Created: 2019-10-20
// -------------------------------------------------
// Copyright (C) 2019 MingYang Software Technology.
// -------------------------------------------------

/*
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 *           (C) 2001 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006, 2007, 2008 Apple Inc. All rights
 * reserved.
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
 */

#include "script_loader.h"

namespace blink {

ScriptLoader::~ScriptLoader(void) = default;

bool ScriptLoader::BlockForNoModule(ScriptType scriptType, bool nomodule)
{
    return nomodule && ScriptType::kClassic == scriptType;
}

bool ScriptLoader::IsValidScriptTypeAndLanguage(
    const String &typeAttributeValue, const String &languageAttributeValue,
    LegacyTypeSupport supportLegacyTypes,
    ScriptType &outScriptType)
{
    ASSERT(false); // BKTODO:
    return false;
}

std::shared_ptr<PendingScript> ScriptLoader::TakePendingScript(ScriptSchedulingType schedulingType)
{
    ASSERT(false); // BKTODO:
    return nullptr;
}

}  // namespace blink
