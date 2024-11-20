/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvCore/ECS/Components/CMaterialRenderer.h>
#include <OvCore/Rendering/EngineDrawableDescriptor.h>
#include <OvCore/Rendering/EngineBufferRenderFeature.h>
#include <OvCore/Rendering/ShadowRenderFeature.h>
#include <OvCore/Rendering/ShadowRenderPass.h>
#include <OvRendering/Features/LightingRenderFeature.h>

constexpr uint16_t kShadowMapSize = 1024;

// Limited to one shadow map at the moment. Support for material uniform arrays is needed
constexpr uint8_t kMaxShadowMaps = 1;

OvCore::Rendering::ShadowRenderFeature::ShadowRenderFeature(OvRendering::Core::CompositeRenderer& p_renderer) :
	ARenderFeature(p_renderer)
{
}

void OvCore::Rendering::ShadowRenderFeature::OnBeforeDraw(OvRendering::Data::PipelineState& p_pso, const OvRendering::Entities::Drawable& p_drawable)
{
	auto& material = p_drawable.material.value();

	if (material.IsShadowReceiver())
	{
		auto& shadowRenderPass = m_renderer.GetPass<ShadowRenderPass>("Shadows");

		OVASSERT(m_renderer.HasDescriptor<OvRendering::Features::LightingRenderFeature::LightingDescriptor>(), "Cannot find LightingDescriptor attached to this renderer");

		auto& lightDescriptor = m_renderer.GetDescriptor<OvRendering::Features::LightingRenderFeature::LightingDescriptor>();

		uint8_t lightIndex = 0;

		for (auto lightReference : lightDescriptor.lights)
		{
			const auto& light = lightReference.get();

			if (light.type == OvRendering::Settings::ELightType::DIRECTIONAL && lightIndex < kMaxShadowMaps)
			{
				material.Set("_ShadowMap", light.GetShadowBuffer().GetTexture());
				material.Set("_LightSpaceMatrix", light.GetLightSpaceMatrix());
				++lightIndex;
			}
		}
	}
}

void OvCore::Rendering::ShadowRenderFeature::OnAfterDraw(OvRendering::Data::PipelineState& p_pso, const OvRendering::Entities::Drawable& p_drawable)
{

}

