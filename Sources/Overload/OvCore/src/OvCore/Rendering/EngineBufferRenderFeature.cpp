/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvRendering/Core/CompositeRenderer.h>

#include "OvCore/Rendering/EngineBufferRenderFeature.h"
#include "OvCore/Rendering/EngineDrawableDescriptor.h"

OvCore::Rendering::EngineBufferRenderFeature::EngineBufferRenderFeature(OvRendering::Core::CompositeRenderer& p_renderer)
	: ARenderFeature(p_renderer)
{
	m_engineBuffer = std::make_unique<OvRendering::Buffers::UniformBuffer>(
		/* UBO Data Layout */
		sizeof(OvMaths::FMatrix4) +
		sizeof(OvMaths::FMatrix4) +
		sizeof(OvMaths::FMatrix4) +
		sizeof(OvMaths::FVector3) +
		sizeof(float) +
		sizeof(OvMaths::FMatrix4),
		0, 0,
		OvRendering::Settings::EAccessSpecifier::STREAM_DRAW
	);

	m_startTime = std::chrono::high_resolution_clock::now();
}

void OvCore::Rendering::EngineBufferRenderFeature::OnBeginFrame(const OvRendering::Data::FrameDescriptor& p_frameDescriptor)
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - m_startTime);

	size_t offset = sizeof(OvMaths::FMatrix4);
	m_engineBuffer->SetSubData(OvMaths::FMatrix4::Transpose(p_frameDescriptor.camera->GetViewMatrix()), std::ref(offset));
	m_engineBuffer->SetSubData(OvMaths::FMatrix4::Transpose(p_frameDescriptor.camera->GetProjectionMatrix()), std::ref(offset));
	m_engineBuffer->SetSubData(p_frameDescriptor.camera->GetPosition(), std::ref(offset));
	m_engineBuffer->SetSubData(elapsedTime.count(), std::ref(offset));
	m_engineBuffer->Bind(0);
}

void OvCore::Rendering::EngineBufferRenderFeature::OnEndFrame()
{
	m_engineBuffer->Unbind();
}

void OvCore::Rendering::EngineBufferRenderFeature::OnBeforeDraw(OvRendering::Data::PipelineState& p_pso, const OvRendering::Entities::Drawable& p_drawable)
{
	OvTools::Utils::OptRef<const EngineDrawableDescriptor> descriptor;
	if (p_drawable.TryGetDescriptor<EngineDrawableDescriptor>(descriptor))
	{
		m_engineBuffer->SetSubData(OvMaths::FMatrix4::Transpose(descriptor->modelMatrix), 0);
		m_engineBuffer->SetSubData
		(
			descriptor->userMatrix,

			// UBO layout offset
			sizeof(OvMaths::FMatrix4) +
			sizeof(OvMaths::FMatrix4) +
			sizeof(OvMaths::FMatrix4) +
			sizeof(OvMaths::FVector3) +
			sizeof(float)
		);
	}
}
