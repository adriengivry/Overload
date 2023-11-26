/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvRendering/Features/ARenderFeature.h"
#include "OvRendering/Core/ABaseRenderer.h"
#include "OvRendering/Core/CompositeRenderer.h"

OvRendering::Features::ARenderFeature::ARenderFeature(Core::CompositeRenderer& p_renderer)
    : m_renderer(p_renderer), m_driver(p_renderer.GetDriver())
{
}

bool OvRendering::Features::ARenderFeature::Validate() const
{
    return true;
}

void OvRendering::Features::ARenderFeature::OnBeforeDraw(const Entities::Drawable& p_drawable)
{
}

void OvRendering::Features::ARenderFeature::OnAfterDraw(const Entities::Drawable& p_drawable)
{
}

void OvRendering::Features::ARenderFeature::OnBeginFrame(std::optional<Data::RenderOutputDesc>& p_outputDesc)
{
}

void OvRendering::Features::ARenderFeature::OnEndFrame()
{
}
