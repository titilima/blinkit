// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: raster_task.cpp
// Description: RasterTask Class
//      Author: Ziming Li
//     Created: 2021-12-24
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./raster_task.h"

#include "blinkit/app/app_impl.h"
#include "blinkit/ui/compositor/blink/display_item_list.h"
#include "blinkit/ui/compositor/blink/layer.h"
#include "blinkit/ui/compositor/compositor.h"
#include "blinkit/ui/compositor/layers/layer_client.h"
#include "blinkit/ui/compositor/paint_ui_task.h"

namespace BlinKit {

class RasterTask::Entry final : public DisplayItemList
{
public:
    Entry(Layer *layer) : m_id(layer->id()) {}

    int Id(void) const { return m_id; }
private:
    const int m_id;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RasterTask::RasterTask(const IntSize &viewportSize) : m_viewportSize(viewportSize)
{
}

RasterTask::~RasterTask(void) = default;

void RasterTask::AddDirtyLayer(Layer *layer)
{
    if (!layer->drawsContent())
        return;

    if (!Invalidate(layer))
        return;

    RecordPainting(layer);
}

bool RasterTask::Invalidate(Layer *layer)
{
    IntRect dirtyRect = layer->TakeDirtyRect();
    if (dirtyRect.isEmpty())
    {
        dirtyRect.setSize(layer->bounds());
        if (dirtyRect.isEmpty())
            return false;
    }

    dirtyRect.setLocation(roundedIntPoint(layer->position()));
    m_dirtyRect.unite(dirtyRect);
    return true;
}

void RasterTask::RecordPainting(Layer *layer)
{
    m_entries.emplace_back(std::make_unique<Entry>(layer));
    layer->Client()->PaintContents(m_entries.back().get());
}

void RasterTask::Run(Compositor &compositor)
{
    ASSERT(!HasNothingToDo());

    for (const auto &e : m_entries)
        compositor.UpdateLayer(e->Id(), *e);

    ASSERT(m_paintTask);
    m_paintTask->PerformComposition(compositor, m_dirtyRect);

    ASSERT(m_taskRunner);
    m_taskRunner->postTask(BLINK_FROM_HERE, m_paintTask.release());
}

void RasterTask::SavePaintTask(std::unique_ptr<PaintUITask> &paintTask)
{
    ASSERT(!m_paintTask);
    ASSERT(paintTask);
    m_paintTask.swap(paintTask);

    m_taskRunner = AppImpl::Get().taskRunner();
}

} // namespace BlinKit
