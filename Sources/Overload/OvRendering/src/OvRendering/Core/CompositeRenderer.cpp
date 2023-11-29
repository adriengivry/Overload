/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <functional>

#include "OvRendering/Core/CompositeRenderer.h"

OvRendering::Core::CompositeRenderer::CompositeRenderer(Context::Driver& p_driver)
	: ABaseRenderer(p_driver)
{
}

void OvRendering::Core::CompositeRenderer::BeginFrame(const Data::FrameDescriptor& p_frameDescriptor)
{
	ABaseRenderer::BeginFrame(p_frameDescriptor);

	for (const auto& [_, feature] : m_features)
	{
		feature->OnBeginFrame(p_frameDescriptor);
	}
}

void OvRendering::Core::CompositeRenderer::EndFrame()
{
	for (const auto& [_, feature] : m_features)
	{
		feature->OnEndFrame();
	}

	// Consume descriptors
	m_descriptors.clear();

	return ABaseRenderer::EndFrame();
}

void OvRendering::Core::CompositeRenderer::DrawEntity(const Entities::Drawable& p_drawable)
{
	for (const auto& [_, feature] : m_features)
	{
		feature->OnBeforeDraw(p_drawable);
	}

	ABaseRenderer::DrawEntity(p_drawable);

	for (const auto& [_, feature] : m_features)
	{
		feature->OnAfterDraw(p_drawable);
	}
}
