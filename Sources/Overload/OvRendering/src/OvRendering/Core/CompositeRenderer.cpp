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

void OvRendering::Core::CompositeRenderer::BeginFrame(std::optional<Data::RenderOutputDesc> p_outputDesc)
{
	ABaseRenderer::BeginFrame(p_outputDesc);

	for (const auto& [_, feature] : m_renderFeatures)
	{
		OVASSERT(feature->Validate(), "Critical Error: Couldn't validate render feature.");
	}

	for (const auto& [_, feature] : m_renderFeatures)
	{
		feature->OnBeginFrame(p_outputDesc);
	}
}

void OvRendering::Core::CompositeRenderer::EndFrame()
{
	for (const auto& [_, feature] : m_renderFeatures)
	{
		feature->OnEndFrame();
	}

	return ABaseRenderer::EndFrame();
}

void OvRendering::Core::CompositeRenderer::DrawEntity(const Entities::Drawable& p_drawable)
{
	for (const auto& [_, feature] : m_renderFeatures)
	{
		feature->OnBeforeDraw(p_drawable);
	}

	ABaseRenderer::DrawEntity(p_drawable);

	for (const auto& [_, feature] : m_renderFeatures)
	{
		feature->OnAfterDraw(p_drawable);
	}
}
