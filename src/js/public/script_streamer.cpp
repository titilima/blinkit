// -------------------------------------------------
// BlinKit - js Library
// -------------------------------------------------
//   File Name: script_streamer.cpp
// Description: ScriptStreamer Class
//      Author: Ziming Li
//     Created: 2018-10-03
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "script_streamer.h"

#include "core/html/parser/TextResourceDecoder.h"
#include "platform/SharedBuffer.h"
#include "public/platform/Platform.h"

namespace blink {

const char* startedStreamingHistogramName(PendingScript::Type scriptType)
{
    switch (scriptType)
    {
        case PendingScript::ParsingBlocking:
            return "WebCore.Scripts.ParsingBlocking.StartedStreaming";
        case PendingScript::Deferred:
            return "WebCore.Scripts.Deferred.StartedStreaming";
        case PendingScript::Async:
            return "WebCore.Scripts.Async.StartedStreaming";
        default:
            assert(false); // Not reached!
    }
    return nullptr;
}

enum NotStreamingReason {
    AlreadyLoaded,
    NotHTTP,
    Reload,
    NotStreamingReasonEnd
};

const char* notStreamingReasonHistogramName(PendingScript::Type scriptType)
{
    switch (scriptType)
    {
        case PendingScript::ParsingBlocking:
            return "WebCore.Scripts.ParsingBlocking.NotStreamingReason";
        case PendingScript::Deferred:
            return "WebCore.Scripts.Deferred.NotStreamingReason";
        case PendingScript::Async:
            return "WebCore.Scripts.Async.NotStreamingReason";
        default:
            assert(false); // Not reached!
    }
    return nullptr;
}

ScriptStreamer::ScriptStreamer(ScriptResource *resource, PendingScript::Type scriptType, WebTaskRunner *loadingTaskRunner)
    : m_resource(resource)
    , m_scriptType(scriptType)
    , m_loadingTaskRunner(adoptPtr(loadingTaskRunner->clone()))
{
    // Nothing
}

ScriptStreamer::~ScriptStreamer(void)
{
    // Nothing
}

void ScriptStreamer::cancel(void)
{
    ASSERT(isMainThread());
    m_resource.clear();
}

void ScriptStreamer::notifyAppendData(ScriptResource *resource)
{
    ASSERT(isMainThread());
    ASSERT(m_resource == resource);
    // Currently nothing to do.
}

void ScriptStreamer::notifyFinished(Resource *resource)
{
    ASSERT(isMainThread());
    ASSERT(m_resource == resource);
    m_loadingFinished = true;
    NotifyFinishedToClient();
}

void ScriptStreamer::NotifyFinishedToClient(void)
{
    ASSERT(isMainThread());
    if (isFinished() && nullptr != m_client)
        m_client->notifyFinished(m_resource);
}

void ScriptStreamer::startStreaming(
    PendingScript &script, PendingScript::Type scriptType,
    Settings *settings,
    WebTaskRunner *loadingTaskRunner)
{
    ASSERT(isMainThread());
    ScriptResource *resource = script.resource();
    do {
        if (resource->isLoaded())
        {
            Platform::current()->histogramEnumeration(notStreamingReasonHistogramName(scriptType),
                AlreadyLoaded, NotStreamingReasonEnd);
            break;
        }
        if (!resource->url().protocolIsInHTTPFamily())
        {
            Platform::current()->histogramEnumeration(notStreamingReasonHistogramName(scriptType),
                NotHTTP, NotStreamingReasonEnd);
            break;
        }
        if (resource->isCacheValidator())
        {
            Platform::current()->histogramEnumeration(notStreamingReasonHistogramName(scriptType),
                Reload, NotStreamingReasonEnd);
            break;
        }

        script.setStreamer(ScriptStreamer::create(resource, scriptType, loadingTaskRunner));
        return;
    } while (false);

    Platform::current()->histogramEnumeration(startedStreamingHistogramName(scriptType), 0, 2);
}

} // namespace blink
