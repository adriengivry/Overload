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

		for (auto lightReference : lightDescriptor.lights)
		{
			const auto& light = lightReference.get();

			if (light.type == OvRendering::Settings::ELightType::DIRECTIONAL)
			{
				material.Set("u_ShadowMap", &light.GetShadowBuffer().GetTexture());
				material.Set("u_LightSpaceMatrix", light.GetLightSpaceMatrix());
			}
		}
	}
}

void OvCore::Rendering::ShadowRenderFeature::OnAfterDraw(OvRendering::Data::PipelineState& p_pso, const OvRendering::Entities::Drawable& p_drawable)
{

}

