/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvRendering/Features/FrameInfoRenderFeature.h"
#include "OvRendering/Core/CompositeRenderer.h"

OvRendering::Features::FrameInfoRenderFeature::FrameInfoRenderFeature(OvRendering::Core::CompositeRenderer& p_renderer)
	: ARenderFeature(p_renderer), m_isFrameInfoDataValid(false)
{
}

void OvRendering::Features::FrameInfoRenderFeature::OnBeginFrame(const Data::FrameDescriptor& p_frameDescriptor)
{
	m_frameInfo.batchCount = 0;
	m_frameInfo.instanceCount = 0;
	m_frameInfo.polyCount = 0;
	m_frameInfo.vertexCount = 0;

	m_isFrameInfoDataValid = false;
}

void OvRendering::Features::FrameInfoRenderFeature::OnEndFrame()
{
	m_isFrameInfoDataValid = true;
}

void OvRendering::Features::FrameInfoRenderFeature::OnAfterDraw(const OvRendering::Entities::Drawable& p_drawable)
{
	// TODO: Calculate vertex count from the primitive mode
	constexpr uint32_t kVertexCountPerPolygon = 3;

	const int instances = p_drawable.material.value().GetGPUInstances();
	
	if (instances > 0)
	{
		++m_frameInfo.batchCount;
		m_frameInfo.instanceCount += instances;
		m_frameInfo.polyCount += (p_drawable.mesh.value().GetIndexCount() / kVertexCountPerPolygon) * instances;
		m_frameInfo.vertexCount += p_drawable.mesh.value().GetVertexCount() * instances;
	}
}

const OvRendering::Data::FrameInfo& OvRendering::Features::FrameInfoRenderFeature::GetFrameInfo() const
{
	OVASSERT(m_isFrameInfoDataValid, "Invalid FrameInfo data! Make sure to retrieve frame info after the frame got fully rendered");
	return m_frameInfo;
}
