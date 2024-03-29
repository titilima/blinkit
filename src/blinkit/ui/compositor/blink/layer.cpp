// -------------------------------------------------
// BlinKit - BlinKit Library
// -------------------------------------------------
//   File Name: layer.cpp
// Description: Layer Class
//      Author: Ziming Li
//     Created: 2021-12-04
// -------------------------------------------------
// Copyright (C) 2021 MingYang Software Technology.
// -------------------------------------------------

#include "./layer.h"

#include <optional>
#include "blinkit/app/app_impl.h"
#include "blinkit/blink/renderer/platform/geometry/LayoutPoint.h"
#include "blinkit/ui/compositor/compositor.h"
#include "blinkit/ui/compositor/layer_tree_host.h"
#include "blinkit/ui/compositor/raster/raster_context.h"
#include "blinkit/ui/compositor/tasks/layer_tasks.h"
#include "blinkit/ui/compositor/tasks/raster_task.h"

namespace BlinKit {

#ifndef NDEBUG
std::unordered_set<Layer *> g_allLayers;
#endif

Layer::Layer(LayerClient *client)
    : m_client(client)
    , m_dirty(false)
    , m_doubleSided(false)
    , m_drawsContent(true)
    , m_fullyInvalidation(false)
    , m_haveScrollEventHandlers(false)
    , m_haveWheelEventHandlers(false)
    , m_ignoreSetNeedsCommit(false)
    , m_isContainerForFixedPositionLayers(false)
    , m_masksToBounds(false)
    , m_needsRebuild(false)
    , m_opaque(false)
    , m_shouldScrollOnMainThread(true)
    , m_useParentBackfaceVisibility(false)
    , m_userScrollableHorizontal(true)
    , m_userScrollableVertical(true)
{
#ifndef NDEBUG
    g_allLayers.emplace(this);
#endif
}

Layer::~Layer(void)
{
    ASSERT(nullptr == m_parent);
    ASSERT(nullptr == m_layerTreeHost);

    for (Layer *child : m_children)
        child->m_parent = nullptr;

    PostTaskToCompositor(new ReleaseSnapshotTask(this));
#ifndef NDEBUG
    g_allLayers.erase(this);
#endif
}

void Layer::addChild(WebLayer *child)
{
    insertChild(child, m_children.size());
}

void Layer::insertChild(WebLayer *child, size_t index)
{
    ASSERT(IsPropertyChangeAllowed());

    Layer *childImpl = static_cast<Layer *>(child);

    childImpl->removeFromParent();
    childImpl->SetParent(this);

    if (m_children.size() < index)
        index = m_children.size();
    m_children.insert(m_children.begin() + index, childImpl);

    SetNeedsFullTreeSync();
}

void Layer::invalidate(void)
{
    if (m_fullyInvalidation)
        return;

    SetNeedsPushProperties();
    m_fullyInvalidation = true;

    if (drawsContent())
    {
        m_dirty = true;
        SetNeedsUpdate();
    }
}

void Layer::invalidateRect(const IntRect &rect)
{
    if (rect.isEmpty() || m_fullyInvalidation)
        return;

    SetNeedsPushProperties();
    m_dirtyRect.unite(rect);

    if (drawsContent())
    {
        m_dirty = true;
        SetNeedsUpdate();
    }
}

void Layer::PostTaskToCompositor(CompositorTask *task)
{
    AppImpl::Get().GetCompositor().PostTask(task);
}

void Layer::removeAllChildren(void)
{
    ASSERT(IsPropertyChangeAllowed());

    if (m_children.empty())
        return;

    Children children;
    children.swap(m_children);
    for (Layer *child : children)
    {
        ASSERT(this == child->m_parent);
        child->m_parent = nullptr;
    }

    SetNeedsFullTreeSync();
}

void Layer::RemoveChildOrDependent(Layer *child)
{
    if (m_mask == child)
    {
        m_mask->SetParent(nullptr);
        m_mask = nullptr;
        SetNeedsFullTreeSync();
        return;
    }

    if (m_replica == child)
    {
        m_replica->SetParent(nullptr);
        m_replica = nullptr;
        SetNeedsFullTreeSync();
        return;
    }

    for (auto it = m_children.begin(); m_children.end() != it; ++it)
    {
        if (*it != child)
            continue;

        child->SetParent(nullptr);
        m_children.erase(it);

        SetNeedsFullTreeSync();
        return;
    }
}

void Layer::removeFromParent(void)
{
    ASSERT(IsPropertyChangeAllowed());
    if (nullptr == m_parent)
        return;
    m_parent->RemoveChildOrDependent(this);
}

void Layer::setBackgroundColor(WebColor color)
{
    ASSERT(IsPropertyChangeAllowed());

    if (m_backgroundColor == color)
        return;

    m_backgroundColor = color;
    SetNeedsCommit();
}

void Layer::setBackgroundFilters(FilterOperations &&backgroundFilters)
{
    ASSERT(backgroundFilters.isEmpty()); // BKTODO:
}

void Layer::setBounds(const IntSize &size)
{
    ASSERT(IsPropertyChangeAllowed());

    if (m_bounds == size)
        return;

    m_bounds = size;

#if 0 // BKTODO: Check the logic later.
    if (ClipNode* clip_node = layer_tree_host_->property_trees()->clip_tree.Node(
        clip_tree_index())) {
        if (clip_node->owner_id == id()) {
            clip_node->data.clip.set_size(gfx::SizeF(size));
            layer_tree_host_->property_trees()->clip_tree.set_needs_update(true);
        }
    }
#else
    m_dirty = m_fullyInvalidation = true;
#endif

    if (nullptr != m_layerTreeHost)
        SetNeedsCommitNoRebuild();
}

void Layer::setClipParent(WebLayer *parent)
{
    ASSERT(IsPropertyChangeAllowed());

    Layer *clipParent = nullptr;
    if (nullptr != parent)
        clipParent = static_cast<Layer *>(parent);

    if (m_clipParent == clipParent)
        return;

    if (nullptr != m_clipParent)
        ASSERT(false); // BKTODO: m_clipParent->RemoveClipChild(this);

    m_clipParent = clipParent;

    if (nullptr != m_clipParent)
        ASSERT(false); // BKTODO: m_clipParent->AddClipChild(this);

    SetNeedsCommit();
    if (nullptr != m_layerTreeHost)
        ASSERT(false); // BKTODO: m_layerTreeHost->SetNeedsMetaInfoRecomputation(true);
}

void Layer::setDoubleSided(bool doubleSided)
{
    ASSERT(IsPropertyChangeAllowed());

    if (m_doubleSided == doubleSided)
        return;

    m_doubleSided = doubleSided;
    SetNeedsCommit();
}

void Layer::setDrawsContent(bool drawsContent)
{
    if (m_drawsContent == drawsContent)
        return;
    m_drawsContent = drawsContent;
    SetNeedsCommit();
}

void Layer::setFilters(FilterOperations &&filters)
{
    ASSERT(filters.isEmpty()); // BKTODO:
}

void Layer::setIsContainerForFixedPositionLayers(bool isContainer)
{
    if (m_isContainerForFixedPositionLayers == isContainer)
        return;
    m_isContainerForFixedPositionLayers = isContainer;

#if 0 // BKTODO: Check the logic later.
    if (nullptr != m_layerTreeHost && m_layerTreeHost->CommitRequested())
        return;

    // Only request a commit if we have a fixed positioned descendant.
    if (DescendantIsFixedToContainerLayer())
        SetNeedsCommit();
#else
    SetNeedsCommit();
#endif
}

void Layer::SetLayerTreeHost(LayerTreeHost *host)
{
    if (m_layerTreeHost == host)
        return;

    if (nullptr != m_layerTreeHost)
    {
        // BKTODO: layer_tree_host_->property_trees()->needs_rebuild = true;
        m_layerTreeHost->UnregisterLayer(this);
    }
    if (nullptr != host)
    {
        // BKTODO: host->property_trees()->needs_rebuild = true;
        host->RegisterLayer(this);
    }

    // BKTODO: InvalidatePropertyTreesIndices();

    m_layerTreeHost = host;

    // When changing hosts, the layer needs to commit its properties to the impl
    // side for the new host.
    SetNeedsPushProperties();

    for (Layer *child : m_children)
        child->SetLayerTreeHost(host);

    if (nullptr != m_mask)
        m_mask->SetLayerTreeHost(host);
    if (nullptr != m_replica)
        m_replica->SetLayerTreeHost(host);

#if 0 // BKTODO:
    if (nullptr != host)
        RegisterForAnimations(host->animation_registrar());

    bool has_any_animation = false;
    if (layer_animation_controller_)
        has_any_animation = layer_animation_controller_->has_any_animation();
    else if (layer_tree_host_)
        has_any_animation = layer_tree_host_->HasAnyAnimation(this);

    if (host && has_any_animation)
        host->SetNeedsCommit();
#endif
}

void Layer::setMaskLayer(WebLayer *maskLayer)
{
    ASSERT(IsPropertyChangeAllowed());

    Layer *maskImpl = static_cast<Layer *>(maskLayer);
    if (m_mask == maskImpl)
        return;

    if (nullptr != m_mask)
    {
        ASSERT(this == m_mask->m_parent);
        m_mask->removeFromParent();
    }
    m_mask = maskImpl;
    if (nullptr != m_mask)
    {
        ASSERT(nullptr == m_mask->m_parent);
        m_mask->removeFromParent();
        m_mask->SetParent(this);
        ASSERT(false); // BKTODO: m_mask->SetIsMask(true);
    }
    SetNeedsFullTreeSync();
}

void Layer::setMasksToBounds(bool masksToBounds)
{
    ASSERT(IsPropertyChangeAllowed());
    if (m_masksToBounds == masksToBounds)
        return;
    m_masksToBounds = masksToBounds;
    SetNeedsCommit();
}

void Layer::SetNeedsCommit(void)
{
    if (nullptr == m_layerTreeHost)
        return;

    SetNeedsPushProperties();
    m_needsRebuild = true;
    // BKTODO: layer_tree_host_->property_trees()->needs_rebuild = true;
    if (m_ignoreSetNeedsCommit)
        return;

    m_layerTreeHost->SetNeedsCommit();
}

void Layer::SetNeedsCommitNoRebuild(void)
{
    if (nullptr == m_layerTreeHost)
        return;

    SetNeedsPushProperties();
    if (m_ignoreSetNeedsCommit)
        return;

    m_layerTreeHost->SetNeedsCommit();
}

void Layer::SetNeedsFullTreeSync(void)
{
    if (nullptr == m_layerTreeHost)
        return;
    m_layerTreeHost->SetNeedsFullTreeSync();
}

void Layer::SetNeedsPushProperties(void)
{
#if 0 // BKTODO:
    if (needs_push_properties_)
        return;
    if (!parent_should_know_need_push_properties() && parent_)
        parent_->AddDependentNeedsPushProperties();
    needs_push_properties_ = true;
#endif
}

void Layer::SetNeedsUpdate(void)
{
    if (nullptr != m_layerTreeHost && !m_ignoreSetNeedsCommit)
        m_layerTreeHost->SetNeedsUpdateLayers();
}

void Layer::setNonFastScrollableRegion(const Region &region)
{
    ASSERT(IsPropertyChangeAllowed());
    if (m_nonFastScrollableRegion == region)
        return;
    m_nonFastScrollableRegion = region;
    SetNeedsCommit();
}

void Layer::setOpacity(float opacity)
{
    ASSERT(IsPropertyChangeAllowed());
    if (zed::almost_equals(m_opacity, opacity))
        return;
    m_opacity = opacity;
    SetNeedsCommit();
}

void Layer::setOpaque(bool opaque)
{
    ASSERT(IsPropertyChangeAllowed());
    if (m_opaque == opaque)
        return;
    m_opaque = opaque;
    SetNeedsCommit();
}

void Layer::SetParent(Layer *layer)
{
    ASSERT(nullptr == layer || !layer->HasAncestor(this));

#if 0 // BKTODO:
    if (parent_should_know_need_push_properties()) {
        if (parent_)
            parent_->RemoveDependentNeedsPushProperties();
        if (layer)
            layer->AddDependentNeedsPushProperties();
    }
#endif

    m_parent = layer;
    SetLayerTreeHost(nullptr != m_parent ? m_parent->GetLayerTreeHost() : nullptr);

    if (nullptr == m_layerTreeHost)
        return;

    // BKTODO: layer_tree_host_->property_trees()->needs_rebuild = true;
}

void Layer::setPosition(const FloatPoint &position)
{
    ASSERT(IsPropertyChangeAllowed());
    if (position == m_position)
        return;
    m_position = position;

#if 0 // BKTODO: Check the logic later.
    if (TransformNode* transform_node =
        layer_tree_host_->property_trees()->transform_tree.Node(
            transform_tree_index())) {
        if (transform_node->owner_id == id()) {
            transform_node->data.update_post_local_transform(position,
                transform_origin());
            transform_node->data.needs_local_transform_update = true;
            layer_tree_host_->property_trees()->transform_tree.set_needs_update(true);
            SetNeedsCommitNoRebuild();
            return;
        }
    }
#endif

    if (nullptr != m_layerTreeHost)
        SetNeedsCommit();
}

void Layer::setPositionConstraint(const WebLayerPositionConstraint &constraint)
{
    ASSERT(IsPropertyChangeAllowed());

    if (m_positionConstraint == constraint)
        return;

    m_positionConstraint = constraint;
    SetNeedsCommit();
}

void Layer::setRenderingContext(int id)
{
    ASSERT(IsPropertyChangeAllowed());

    if (id == m_sortingContextId)
        return;
    m_sortingContextId = id;
    SetNeedsCommit();
}

void Layer::setReplicaLayer(WebLayer *replicaLayer)
{
    ASSERT(IsPropertyChangeAllowed());

    Layer *layer = nullptr;
    if (nullptr != replicaLayer)
        layer = static_cast<Layer *>(replicaLayer);

    if (m_replica == layer)
        return;

    if (nullptr != m_replica)
    {
        ASSERT(this == m_replica->Parent());
        m_replica->removeFromParent();
    }
    m_replica = layer;
    if (nullptr != m_replica)
    {
        ASSERT(nullptr == m_replica->Parent());
        m_replica->removeFromParent();
        m_replica->SetParent(this);
    }
    SetNeedsFullTreeSync();
}

void Layer::setScrollBlocksOn(WebScrollBlocksOn scrollBlocksOn)
{
    if (m_scrollBlocksOn == scrollBlocksOn)
        return;
    m_scrollBlocksOn = scrollBlocksOn;
    SetNeedsCommit();
}

void Layer::setScrollClipLayer(WebLayer *clipLayer)
{
    ASSERT(IsPropertyChangeAllowed());

    int clipLayerId = nullptr != clipLayer
        ? static_cast<Layer *>(clipLayer)->m_scrollClipLayerId
        : Layer::INVALID_ID;

    if (m_scrollClipLayerId == clipLayerId)
        return;
    m_scrollClipLayerId = clipLayerId;
    SetNeedsCommit();
}

void Layer::setScrollCompensationAdjustment(const DoublePoint &scrollCompensationAdjustment)
{
    if (m_scrollCompensationAdjustment == scrollCompensationAdjustment)
        return;
    m_scrollCompensationAdjustment = scrollCompensationAdjustment;
    SetNeedsCommit();
}

void Layer::setScrollParent(WebLayer *parent)
{
    ASSERT(IsPropertyChangeAllowed());

    Layer *scrollParent = nullptr;
    if (nullptr != parent)
        scrollParent = static_cast<Layer *>(parent);

    if (m_scrollParent == scrollParent)
        return;

    if (nullptr != m_scrollParent)
        ASSERT(false); // BKTODO: m_scrollParent->RemoveScrollChild(this);

    m_scrollParent = scrollParent;

    if (nullptr != m_scrollParent)
        ASSERT(false); // BKTODO: m_scrollParent->AddScrollChild(this);

    SetNeedsCommit();
}

void Layer::setScrollPositionDouble(const DoublePoint &scrollPosition)
{
    ASSERT(IsPropertyChangeAllowed());

    if (m_scrollPosition == scrollPosition)
        return;
    m_scrollPosition = scrollPosition;

    if (nullptr == m_layerTreeHost)
        return;

#if 0 // BKTODO: Check the logic later.
    if (TransformNode* transform_node =
        layer_tree_host_->property_trees()->transform_tree.Node(
            transform_tree_index())) {
        if (transform_node->owner_id == id()) {
            transform_node->data.scroll_offset = CurrentScrollOffset();
            transform_node->data.needs_local_transform_update = true;
            layer_tree_host_->property_trees()->transform_tree.set_needs_update(true);
            SetNeedsCommitNoRebuild();
            return;
        }
    }
#endif

    SetNeedsCommit();
}

void Layer::setShouldScrollOnMainThread(bool shouldScrollOnMainThread)
{
    ASSERT(IsPropertyChangeAllowed());

    if (m_shouldScrollOnMainThread == shouldScrollOnMainThread)
        return;

    m_shouldScrollOnMainThread = shouldScrollOnMainThread;
    SetNeedsCommit();
}

void Layer::setTransform(const SkMatrix44 &transform)
{
    ASSERT(IsPropertyChangeAllowed());

    if (m_transform == transform)
        return;

    if (nullptr != m_layerTreeHost)
    {
#if 0 // BKTODO: Check the logic later.
        if (TransformNode* transform_node =
            layer_tree_host_->property_trees()->transform_tree.Node(
                transform_tree_index())) {
            if (transform_node->owner_id == id()) {
                // We need to trigger a rebuild if we could have affected 2d axis
                // alignment. We'll check to see if transform and transform_ are axis
                // align with respect to one another.
                bool invertible = false;
                bool preserves_2d_axis_alignment =
                    Are2dAxisAligned(transform_, transform, &invertible);
                transform_node->data.local = transform;
                transform_node->data.needs_local_transform_update = true;
                layer_tree_host_->property_trees()->transform_tree.set_needs_update(
                    true);
                if (preserves_2d_axis_alignment)
                    SetNeedsCommitNoRebuild();
                else
                    SetNeedsCommit();
                transform_ = transform;
                transform_is_invertible_ = invertible;
                return;
            }
        }
#endif
    }

    m_transform = transform;
    // BKTODO: transform_is_invertible_ = transform.IsInvertible();

    SetNeedsCommit();
}

void Layer::setTransformOrigin(const FloatPoint3D &transformOrigin)
{
    ASSERT(IsPropertyChangeAllowed());

    if (m_transformOrigin == transformOrigin)
        return;
    m_transformOrigin = transformOrigin;

    if (nullptr == m_layerTreeHost)
        return;

#if 0 // BKTODO: Check the logic later.
    if (TransformNode* transform_node =
        layer_tree_host_->property_trees()->transform_tree.Node(
            transform_tree_index())) {
        if (transform_node->owner_id == id()) {
            transform_node->data.update_pre_local_transform(transform_origin);
            transform_node->data.update_post_local_transform(position(),
                transform_origin);
            transform_node->data.needs_local_transform_update = true;
            layer_tree_host_->property_trees()->transform_tree.set_needs_update(true);
            SetNeedsCommitNoRebuild();
            return;
        }
    }
#endif

    SetNeedsCommit();
}

void Layer::setUserScrollable(bool horizontal, bool vertical)
{
    ASSERT(IsPropertyChangeAllowed());

    if (m_userScrollableHorizontal == horizontal && m_userScrollableVertical == vertical)
        return;
    m_userScrollableHorizontal = horizontal;
    m_userScrollableVertical = vertical;
    SetNeedsCommit();
}

void Layer::Update(const RasterContext &context, RasterTask &session)
{
    std::optional<bool> needsRebuild;
    if (m_needsRebuild)
    {
        needsRebuild = session.SetNeedsRebuild(true);
        m_needsRebuild = false;
    }

    if (m_drawsContent)
        UpdateContent(context, session);

    {
        RasterContext contextForChildren(context, *this);
        for (Layer *child : m_children)
            child->Update(contextForChildren, session);
    }

    if (nullptr != m_mask)
        ASSERT(false); // BKTODO: m_mask->Update(context, rasterSession);

    if (needsRebuild.has_value())
        session.SetNeedsRebuild(needsRebuild.value());
}

void Layer::UpdateContent(const RasterContext &context, RasterTask &session)
{
    ASSERT(m_drawsContent);

    const IntRect visibleRect = context.CalculateVisibleRect(m_position, m_bounds, session.GetViewportRect());
    if (m_dirty || session.NeedsRebuild())
    {
        LayerContext &layerContext = session.RequireLayerContext(*this, m_bounds);
        layerContext.positionOfViewport = context.CalculatePosition(m_position);
        if (m_dirty)
        {
            if (m_fullyInvalidation)
            {
                layerContext.dirtyRectOfLayer.setSize(m_bounds);
                session.UpdateDirtyRect(visibleRect);
            }
            else
            {
                layerContext.dirtyRectOfLayer = m_dirtyRect;
                m_dirtyRect.moveBy(layerContext.positionOfViewport);
                session.UpdateDirtyRect(m_dirtyRect);
            }

            m_fullyInvalidation = false;
            m_dirtyRect = IntRect();
            m_dirty = false;
        }
        else
        {
            session.UpdateDirtyRect(visibleRect);
        }
    }

    session.Rasterize(*this, visibleRect);
}

#ifndef NDEBUG

void Layer::DebugPrint(int depth) const
{
    std::string indents;
    for (int i = 0; i < depth; ++i)
        indents.append("  ");

    BKLOG("%s[Layer 0x%p]", indents.c_str(), this);

    if (FloatPoint::zero() != m_position)
        ZLOG("{}  .position = {}", indents, m_position);
    if (!m_bounds.isEmpty())
        ZLOG("{}  .bounds = {}", indents, m_bounds);
    if (DoublePoint::zero() != m_scrollPosition)
        BKLOG("%s  .scrollPosition = (%.2lf, %.2lf)", indents.c_str(), m_scrollPosition.x(), m_scrollPosition.y());

    if (m_doubleSided)
        ZLOG("{}  .doubleSided", indents);
    if (m_drawsContent)
        ZLOG("{}  .drawsContent", indents);
    if (m_haveScrollEventHandlers)
        ZLOG("{}  .haveScrollEventHandlers", indents);
    if (m_haveWheelEventHandlers)
        ZLOG("{}  .haveWheelEventHandlers", indents);
    if (m_isContainerForFixedPositionLayers)
        ZLOG("{}  .isContainerForFixedPositionLayers", indents);
    if (m_masksToBounds)
        ZLOG("{}  .masksToBounds", indents);
    if (m_opaque)
        ZLOG("{}  .opaque", indents);
    if (!zed::almost_equals(m_opacity, 1.f))
        BKLOG("%s  .opacity = %.2f", indents.c_str(), m_opacity);
    if (!m_positionConstraint.IsDefault())
        ZLOG("{}  .positionConstraint = {}", indents, m_positionConstraint);
    if (!m_transformOrigin.isZero())
        ZLOG("{}  .transformOrigin = {}", indents, m_transformOrigin);
    if (m_useParentBackfaceVisibility)
        ZLOG("{}  .useParentBackfaceVisibility", indents);
    if (!m_userScrollableHorizontal || !m_userScrollableVertical)
        ZLOG("{}  .userScrollable = ({}, {})", indents, m_userScrollableHorizontal, m_userScrollableVertical);

    if (m_children.empty())
        return;
    BKLOG("%s  .children(%d):", indents.c_str(), m_children.size());

    int childrenDepth = depth + 2;
    for (Layer *child : m_children)
        child->DebugPrint(childrenDepth);
}

bool Layer::HasAncestor(const Layer *ancestor) const
{
    for (const Layer *layer = m_parent; nullptr != layer; layer = layer->m_parent)
    {
        if (layer == ancestor)
            return true;
    }
    return false;
}

bool Layer::IsPropertyChangeAllowed(void) const
{
    if (nullptr == m_layerTreeHost)
        return true;

#if 0 // BKTODO: Check the logic later.
    if (!layer_tree_host_->settings().strict_layer_property_change_checking)
        return true;
#endif

    return !m_layerTreeHost->InPaintLayerContents();
}
#endif

} // namespace BlinKit
