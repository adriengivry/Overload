/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Rendering/DebugModelRenderFeature.h"
#include "OvEditor/Rendering/ShadowRenderFeature.h"
#include "OvEditor/Rendering/ShadowRenderPass.h"
#include "OvEditor/Core/EditorActions.h"
#include "OvEditor/Settings/EditorSettings.h"
#include "OvEditor/Rendering/DebugSceneRenderer.h"

#include <OvCore/ECS/Components/CMaterialRenderer.h>
#include <OvCore/Rendering/EngineDrawableDescriptor.h>
#include <OvCore/Rendering/EngineBufferRenderFeature.h>
#include <OvRendering/Features/LightingRenderFeature.h>

constexpr uint16_t kShadowMapSize = 1024;

OvEditor::Rendering::ShadowRenderFeature::ShadowRenderFeature(OvRendering::Core::CompositeRenderer& p_renderer) :
	ARenderFeature(p_renderer)
{
}

void OvEditor::Rendering::ShadowRenderFeature::OnBeforeDraw(OvRendering::Data::PipelineState& p_pso, const OvRendering::Entities::Drawable& p_drawable)
{
	auto& material = p_drawable.material.value();

	if (material.IsShadowReceiver())
	{
		auto& shadowRenderPass = m_renderer.GetPass<ShadowRenderPass>("Shadows");

		OVASSERT(m_renderer.HasDescriptor<OvRendering::Features::LightingRenderFeature::LightingDescriptor>(), "Cannot find LightingDescriptor attached to this renderer");

		auto& lightDescriptor = m_renderer.GetDescriptor<OvRendering::Features::LightingRenderFeature::LightingDescriptor>();

		for (auto lightReference : lightDescriptor.lights)
		{
			const auto& light = lightReference.get();

			if (light.type == OvRendering::Settings::ELightType::DIRECTIONAL)
			{
				material.Set("u_ShadowMap", light.GetShadowBuffer().GetTexture());
				material.Set("u_LightSpaceMatrix", light.GetLightSpaceMatrix());
			}
		}
	}
}

void OvEditor::Rendering::ShadowRenderFeature::OnAfterDraw(OvRendering::Data::PipelineState& p_pso, const OvRendering::Entities::Drawable& p_drawable)
{

}

