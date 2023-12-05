/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvRendering/Features/ARenderFeature.h"
#include "OvRendering/Core/ABaseRenderer.h"
#include "OvRendering/Core/CompositeRenderer.h"

OvRendering::Features::ARenderFeature::ARenderFeature(Core::CompositeRenderer& p_renderer)
	: m_renderer(p_renderer)
{
}

void OvRendering::Features::ARenderFeature::OnBeginFrame(const Data::FrameDescriptor& p_frameDescriptor)
{
}

void OvRendering::Features::ARenderFeature::OnEndFrame()
{
}

void OvRendering::Features::ARenderFeature::OnBeforeDraw(Data::PipelineState& p_pso, const Entities::Drawable& p_drawable)
{
}

void OvRendering::Features::ARenderFeature::OnAfterDraw(const Entities::Drawable& p_drawable)
{
}
