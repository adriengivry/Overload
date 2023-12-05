/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/


#include "OvRendering/Core/ARenderPass.h"

OvRendering::Core::ARenderPass::ARenderPass(Core::CompositeRenderer& p_renderer)
	: m_renderer(p_renderer)
{
}

void OvRendering::Core::ARenderPass::OnBeginFrame(const Data::FrameDescriptor& p_frameDescriptor)
{
}

void OvRendering::Core::ARenderPass::OnEndFrame()
{
}
