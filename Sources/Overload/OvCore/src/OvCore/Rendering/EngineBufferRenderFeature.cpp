/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvCore/Rendering/EngineBufferRenderFeature.h"

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
		OvRendering::Buffers::EAccessSpecifier::STREAM_DRAW
	);

	m_startTime = std::chrono::high_resolution_clock::now();
}

void OvCore::Rendering::EngineBufferRenderFeature::UploadViewData(
	OvRendering::Entities::Camera& p_camera,
	uint16_t p_windowWidth,
	uint16_t p_windowHeight
)
{
	p_camera.CacheMatrices(p_windowWidth, p_windowHeight);

	// We skip the model matrix (Which is a special case, modified every draw calls)
	size_t offset = sizeof(OvMaths::FMatrix4);
	m_engineBuffer->SetSubData(OvMaths::FMatrix4::Transpose(p_camera.GetViewMatrix()), std::ref(offset));
	m_engineBuffer->SetSubData(OvMaths::FMatrix4::Transpose(p_camera.GetProjectionMatrix()), std::ref(offset));
	m_engineBuffer->SetSubData(p_camera.GetPosition(), std::ref(offset));
}

void OvCore::Rendering::EngineBufferRenderFeature::OnBeforeDraw(const OvRendering::Entities::Drawable& p_drawable)
{
	m_engineBuffer->SetSubData(OvMaths::FMatrix4::Transpose(p_drawable.modelMatrix), 0);
	m_engineBuffer->SetSubData
	(
		p_drawable.userMatrix,

		// UBO layout offset
		sizeof(OvMaths::FMatrix4) +
		sizeof(OvMaths::FMatrix4) +
		sizeof(OvMaths::FMatrix4) +
		sizeof(OvMaths::FVector3) +
		sizeof(float)
	);
	m_engineBuffer->Bind();
}

void OvCore::Rendering::EngineBufferRenderFeature::OnBeginFrame(std::optional<OvRendering::Data::RenderOutputDesc>& p_outputDesc)
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime - m_startTime);
	m_engineBuffer->SetSubData(elapsedTime.count(), 3 * sizeof(OvMaths::FMatrix4) + sizeof(OvMaths::FVector3));
}

void OvCore::Rendering::EngineBufferRenderFeature::OnEndFrame()
{
	m_engineBuffer->Unbind();
}
