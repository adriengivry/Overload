/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvRendering/Features/LightingRenderFeature.h"
#include "OvRendering/Core/CompositeRenderer.h"

OvRendering::Features::LightingRenderFeature::LightingRenderFeature(Core::CompositeRenderer& p_renderer, uint32_t p_bufferBindingPoint)
	: ARenderFeature(p_renderer), m_bufferBindingPoint(p_bufferBindingPoint)
{
	m_lightBuffer = std::make_unique<Buffers::ShaderStorageBuffer>(Settings::EAccessSpecifier::STREAM_DRAW);
}

bool IsLightInFrustum(const OvRendering::Entities::Light& p_light, const OvRendering::Data::Frustum& p_frustum)
{
	const auto& position = p_light.transform->GetWorldPosition();
	const auto effectRange = p_light.GetEffectRange();

	// We always consider lights that have an +inf range (Not necessary to test if they are in frustum)
	const bool isOmniscientLight = std::isinf(effectRange);

	return
		isOmniscientLight ||
		p_frustum.SphereInFrustum(position.x, position.y, position.z, p_light.GetEffectRange());
}

void OvRendering::Features::LightingRenderFeature::OnBeginFrame(const Data::FrameDescriptor& p_frameDescriptor)
{
	OVASSERT(m_renderer.HasDescriptor<LightingDescriptor>(), "Cannot find LightingDescriptor attached to this renderer");

	auto& lightDescriptor = m_renderer.GetDescriptor<LightingDescriptor>();
	auto& frameDescriptor = m_renderer.GetFrameDescriptor();

	std::vector<OvMaths::FMatrix4> lightMatrices;
	auto frustum = frameDescriptor.camera->GetLightFrustum();

	for (auto light : lightDescriptor.lights)
	{
		if (!frustum || IsLightInFrustum(light.get(), frustum.value()))
		{
			lightMatrices.push_back(light.get().GenerateMatrix());
		}
	}

	m_lightBuffer->SendBlocks<OvMaths::FMatrix4>(lightMatrices.data(), lightMatrices.size() * sizeof(OvMaths::FMatrix4));

	m_lightBuffer->Bind(m_bufferBindingPoint);
}

void OvRendering::Features::LightingRenderFeature::OnEndFrame()
{
	m_lightBuffer->Unbind();
}
