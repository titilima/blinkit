// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: script_streamer.h
// Description: ScriptStreamer Class
//      Author: Ziming Li
//     Created: 2018-10-03
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#ifndef BLINKIT_BLINKIT_SCRIPT_STREAMER_H
#define BLINKIT_BLINKIT_SCRIPT_STREAMER_H

#pragma once

#include "blinkit/blink/renderer/core/dom/PendingScript.h"
#include "blinkit/blink/renderer/platform/heap/Handle.h"
#include "blinkit/blink/renderer/wtf/RefCounted.h"

namespace blink {

class Settings;

class ScriptStreamer final : public RefCountedWillBeRefCountedGarbageCollected<ScriptStreamer>
{
public:
    static PassRefPtrWillBeRawPtr<ScriptStreamer> create(ScriptResource *resource, PendingScript::Type scriptType, WebTaskRunner *loadingTaskRunner)
    {
        return adoptRefWillBeNoop(new ScriptStreamer(resource, scriptType, loadingTaskRunner));
    }

    ~ScriptStreamer(void);

    static void startStreaming(PendingScript &, PendingScript::Type, Settings *, WebTaskRunner *);

    bool isFinished(void) const { return m_loadingFinished; }

    void cancel(void);

    bool streamingSuppressed(void) const { return false; }

    void addClient(ScriptResourceClient *client)
    {
        assert(nullptr == m_client);
        m_client = client;
        NotifyFinishedToClient();
    }

    void removeClient(ScriptResourceClient *client)
    {
        assert(m_client == client);
        m_client = nullptr;
    }

    void notifyAppendData(ScriptResource *);
    void notifyFinished(Resource *);
private:
    ScriptStreamer(ScriptResource *, PendingScript::Type, WebTaskRunner *);

    void NotifyFinishedToClient(void);

    RawPtrWillBeMember<ScriptResource> m_resource;
    ScriptResourceClient *m_client = nullptr;
    bool m_loadingFinished = false;
    bool m_haveEnoughDataForStreaming = false;
    PendingScript::Type m_scriptType;
    std::shared_ptr<WebTaskRunner> m_loadingTaskRunner;
};

} // namespace blink

#endif // BLINKIT_BLINKIT_SCRIPT_STREAMER_H
