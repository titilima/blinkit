// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: script_streamer.cpp
// Description: ScriptStreamer Class
//      Author: Ziming Li
//     Created: 2018-10-03
// -------------------------------------------------
// Copyright (C) 2018 MingYang Software Technology.
// -------------------------------------------------

#include "./script_streamer.h"

#include "blinkit/blink/renderer/core/html/parser/TextResourceDecoder.h"
#include "blinkit/blink/renderer/platform/SharedBuffer.h"

namespace blink {

ScriptStreamer::ScriptStreamer(ScriptResource *resource, PendingScript::Type scriptType, WebTaskRunner *loadingTaskRunner)
    : m_resource(resource)
    , m_scriptType(scriptType)
    // BKTODO: , m_loadingTaskRunner(adoptPtr(loadingTaskRunner->clone()))
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
    ASSERT(false); // BKTODO:
#if 0
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
#endif
}

} // namespace blink
