// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: compositing_layer.cpp
// Description: CompositingLayer Class
//      Author: Ziming Li
//     Created: 2021-12-18
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./compositing_layer.h"

#include "blinkit/ui/compositor/blink/display_item_list.h"
#include "blinkit/ui/compositor/tile.h"
#include "third_party/skia/include/core/SkBitmap.h"
#include "third_party/skia/include/core/SkCanvas.h"

namespace BlinKit {

CompositingLayer::CompositingLayer(void) = default;
CompositingLayer::~CompositingLayer(void) = default;

void CompositingLayer::BlendTo(SkCanvas *canvas, const TileGrid::Range &dirtyTiles)
{
    if (!m_drawsContent || !m_bitmap)
        return;

    // BKTODO: Only draw in dirty tiles.
    canvas->drawBitmap(*m_bitmap, m_position.x(), m_position.y());
}

void CompositingLayer::DetachChildren(void)
{
    Children children;
    children.swap(m_children);

    for (CompositingLayer *layer : children)
        layer->m_parent = nullptr;
}

void CompositingLayer::DetachFromParent(void)
{
    if (nullptr != m_parent)
        m_parent->RemoveChild(this);
}

const SkBitmap& CompositingLayer::EnsureBitmap(void)
{
    IntSize size = Tile::GetAlignedSize(m_bounds);

    if (m_bitmap)
    {
        const SkImageInfo &info = m_bitmap->info();
        if (size.width() <= info.width() && size.height() <= info.height())
            return *m_bitmap;

        m_bitmap->reset();
    }
    else
    {
        m_bitmap = std::make_unique<SkBitmap>();
        ASSERT(m_bitmap);
    }

    m_bitmap->allocN32Pixels(size.width(), size.height(), kPremul_SkAlphaType);
    return *m_bitmap;
}

void CompositingLayer::InsertChild(size_t index, CompositingLayer *child)
{
    ASSERT(index <= m_children.size());

    child->DetachFromParent();
    child->m_parent = this;
    m_children.insert(m_children.begin() + index, child);
}

void CompositingLayer::RemoveChild(CompositingLayer *child)
{
    ASSERT(this == child->m_parent);

    auto it = std::find(m_children.begin(), m_children.end(), child);
    ASSERT(m_children.end() != it);
    m_children.erase(it);

    child->m_parent = nullptr;
}

void CompositingLayer::SyncToCompositor(std::vector<CompositingLayer *> &compositedLayers)
{
    if (m_drawsContent && !m_bounds.isEmpty())
        compositedLayers.emplace_back(this);
    for (CompositingLayer *child : m_children)
        child->SyncToCompositor(compositedLayers);
}

void CompositingLayer::Update(const DisplayItemList &displayItemList)
{
    SkCanvas canvas(EnsureBitmap());
    displayItemList.Playback(&canvas);
}

#ifndef NDEBUG
void CompositingLayer::DebugPrint(int depth) const
{
    std::string indents;
    for (int i = 0; i < depth; ++i)
        indents.append("  ");
    BKLOG("%s[0x%p]", indents.c_str(), this);
    BKLOG("%s  .position = (%.1f, %.1f)", indents.c_str(), m_position.x(), m_position.y());
    BKLOG("%s  .bounds = %dx%d", indents.c_str(), m_bounds.width(), m_bounds.height());
    if (m_drawsContent)
        BKLOG("%s  .drawsContent", indents.c_str());
    if (m_bitmap)
        BKLOG("%s  .bitmap", indents.c_str());

    if (m_children.empty())
        return;
    BKLOG("%s  .children:", indents.c_str());

    int childrenDepth = depth + 2;
    for (CompositingLayer *child : m_children)
        child->DebugPrint(childrenDepth);
}
#endif

} // namespace BlinKit
