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

void OvRendering::Core::CompositeRenderer::Draw()
{
	// TODO: Replace that so that a renderer can have some passes enabled/disabled
	Settings::ERenderPassMask renderPassMask = Settings::ERenderPassMask::ALL;

	constexpr auto firstPass = static_cast<uint32_t>(Settings::ERenderPass::FIRST);
	constexpr auto lastPass = static_cast<uint32_t>(Settings::ERenderPass::LAST);

	for (auto i = firstPass; i <= lastPass; i <<= 1)
	{
		auto renderPass = static_cast<Settings::ERenderPass>(i);

		if (Settings::IsFlagSet(renderPassMask, renderPass))
		{
			for (const auto& [_, feature] : m_features)
			{
				if (Settings::IsFlagSet(feature->GetRenderPassMask(), renderPass))
				{
					// Render feature draw pre-pass
					feature->DrawPrePass(renderPass);
				}
			}

			// Renderer draw pass
			DrawPass(renderPass);

			for (const auto& [_, feature] : m_features)
			{
				if (Settings::IsFlagSet(feature->GetRenderPassMask(), renderPass))
				{
					// Render feature draw pass
					feature->DrawPass(renderPass);
				}
			}
		}
	}
}

void OvRendering::Core::CompositeRenderer::DrawPass(OvRendering::Settings::ERenderPass p_pass)
{

}

void OvRendering::Core::CompositeRenderer::EndFrame()
{
	for (const auto& [_, feature] : m_features)
	{
		feature->OnEndFrame();
	}

	ClearDescriptors();

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
