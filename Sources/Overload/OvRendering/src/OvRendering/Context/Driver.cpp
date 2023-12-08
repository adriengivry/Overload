/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvDebug/Logger.h>
#include <OvDebug/Assertion.h>

#include "OvRendering/Context/Driver.h"
#include "OvRendering/Utils/Conversions.h"

#include "OvRendering/HAL/GraphicsAPI.h"

using SelectedAPI = OvRendering::HAL::OpenGL;

std::unique_ptr<SelectedAPI> m_driverImpl;

OvRendering::Context::Driver::Driver(const OvRendering::Settings::DriverSettings& p_driverSettings)
{
	m_driverImpl = std::make_unique<SelectedAPI>();

	auto initialPipelineState = m_driverImpl->Init(p_driverSettings.debugMode);

	OVASSERT(initialPipelineState.has_value(), "Failed to initialized driver!");

	if (p_driverSettings.defaultPipelineState)
	{
		m_defaultPipelineState = p_driverSettings.defaultPipelineState.value();
	}
	
	m_pipelineState = initialPipelineState.value();
	SetPipelineState(m_defaultPipelineState);

	m_vendor = m_driverImpl->GetVendor();
	m_hardware = m_driverImpl->GetHardware();
	m_version = m_driverImpl->GetVersion();
	m_shadingLanguageVersion = m_driverImpl->GetShadingLanguageVersion();
}

OvRendering::Context::Driver::~Driver()
{
}

void OvRendering::Context::Driver::SetViewport(uint32_t p_x, uint32_t p_y, uint32_t p_width, uint32_t p_height)
{
	m_driverImpl->SetViewport(p_x, p_y, p_width, p_height);
}

void OvRendering::Context::Driver::Clear(
	bool p_colorBuffer,
	bool p_depthBuffer,
	bool p_stencilBuffer,
	const OvMaths::FVector4& p_color
)
{
	if (p_colorBuffer)
	{
		m_driverImpl->SetClearColor(p_color.x, p_color.y, p_color.z, p_color.w);
	}

	auto pso = CreatePipelineState();

	if (p_stencilBuffer)
	{
		pso.stencilWriteMask = ~0;
	}

	pso.scissorTest = false;

	SetPipelineState(pso);

	m_driverImpl->Clear(p_colorBuffer, p_depthBuffer, p_stencilBuffer);
}

void OvRendering::Context::Driver::ReadPixels(
	uint32_t p_x,
	uint32_t p_y,
	uint32_t p_width,
	uint32_t p_height,
	OvRendering::Settings::EPixelDataFormat p_format,
	OvRendering::Settings::EPixelDataType p_type,
	void* p_data
) const
{
	m_driverImpl->ReadPixels(p_x, p_y, p_width, p_height, p_format, p_type, p_data);
}

void OvRendering::Context::Driver::Draw(
	Data::PipelineState p_pso,
	const Resources::IMesh& p_mesh,
	Settings::EPrimitiveMode p_primitiveMode,
	uint32_t p_instances
)
{
	if (p_instances > 0)
	{
		SetPipelineState(p_pso);

		p_mesh.Bind();

		if (p_mesh.GetIndexCount() > 0)
		{
			if (p_instances == 1)
			{
				m_driverImpl->DrawElements(p_primitiveMode, p_mesh.GetIndexCount());
			}
			else
			{
				m_driverImpl->DrawElementsInstanced(p_primitiveMode, p_mesh.GetIndexCount(), p_instances);
			}
		}
		else
		{
			if (p_instances == 1)
			{
				m_driverImpl->DrawArrays(p_primitiveMode, p_mesh.GetVertexCount());
			}
			else
			{
				m_driverImpl->DrawArraysInstanced(p_primitiveMode, p_mesh.GetVertexCount(), p_instances);
			}
		}

		p_mesh.Unbind();
	}
}

void OvRendering::Context::Driver::SetPipelineState(OvRendering::Data::PipelineState p_state)
{
	using namespace OvRendering::Settings;

	if (p_state.bits != m_pipelineState.bits)
	{
		auto& i = p_state;
		auto& c = m_pipelineState;

		// Rasterization
		if (i.rasterizationMode != c.rasterizationMode) m_driverImpl->SetRasterizationMode(i.rasterizationMode);
		if (i.lineWidthPow2 != c.lineWidthPow2) m_driverImpl->SetRasterizationLinesWidth(Utils::Conversions::Pow2toFloat(i.lineWidthPow2));

		if (i.colorWriting.mask != c.colorWriting.mask) m_driverImpl->SetColorWriting(i.colorWriting.r, i.colorWriting.g, i.colorWriting.b, i.colorWriting.a);
		if (i.depthWriting != c.depthWriting) m_driverImpl->SetDepthWriting(i.depthWriting);

		if (i.blending != c.blending) m_driverImpl->SetCapability(ERenderingCapability::BLEND, i.blending);
		if (i.culling != c.culling) m_driverImpl->SetCapability(ERenderingCapability::CULL_FACE, i.culling);
		if (i.dither != c.dither) m_driverImpl->SetCapability(ERenderingCapability::DITHER, i.dither);
		if (i.polygonOffsetFill != c.polygonOffsetFill) m_driverImpl->SetCapability(ERenderingCapability::POLYGON_OFFSET_FILL, i.polygonOffsetFill);
		if (i.sampleAlphaToCoverage != c.sampleAlphaToCoverage) m_driverImpl->SetCapability(ERenderingCapability::SAMPLE_ALPHA_TO_COVERAGE, i.sampleAlphaToCoverage);
		if (i.depthTest != c.depthTest) m_driverImpl->SetCapability(ERenderingCapability::DEPTH_TEST, i.depthTest);
		if (i.scissorTest != c.scissorTest) m_driverImpl->SetCapability(ERenderingCapability::SCISSOR_TEST, i.scissorTest);
		if (i.stencilTest != c.stencilTest) m_driverImpl->SetCapability(ERenderingCapability::STENCIL_TEST, i.stencilTest);
		if (i.multisample != c.multisample) m_driverImpl->SetCapability(ERenderingCapability::MULTISAMPLE, i.multisample);

		// Stencil algorithm
		if (i.stencilFuncOp != c.stencilFuncOp ||
			i.stencilFuncRef != c.stencilFuncRef ||
			i.stencilFuncMask != c.stencilFuncMask)
			m_driverImpl->SetStencilAlgorithm(i.stencilFuncOp, i.stencilFuncRef, i.stencilFuncMask);

		if (i.stencilWriteMask != c.stencilWriteMask) m_driverImpl->SetStencilMask(i.stencilWriteMask);
		if (i.stencilOpFail != c.stencilOpFail || i.depthOpFail != c.depthOpFail || i.bothOpFail != c.bothOpFail) m_driverImpl->SetStencilOperations(i.stencilOpFail, i.depthOpFail, i.bothOpFail);

		// Depth
		if (i.depthFunc != c.depthFunc) m_driverImpl->SetDepthAlgorithm(i.depthFunc);

		// Culling
		if (i.cullFace != c.cullFace) m_driverImpl->SetCullFace(i.cullFace);

		m_pipelineState = p_state;
	}
}

void OvRendering::Context::Driver::ResetPipelineState()
{
	SetPipelineState(m_defaultPipelineState);
}

const OvRendering::Data::PipelineState& OvRendering::Context::Driver::GetPipelineState() const
{
	return m_pipelineState;
}

OvRendering::Data::PipelineState OvRendering::Context::Driver::CreatePipelineState() const
{
	return m_defaultPipelineState;
}

std::string_view OvRendering::Context::Driver::GetVendor() const
{
	return m_vendor;
}

std::string_view OvRendering::Context::Driver::GetHardware() const
{
	return m_hardware;
}

std::string_view OvRendering::Context::Driver::GetVersion() const
{
	return m_version;
}

std::string_view OvRendering::Context::Driver::GetShadingLanguageVersion() const
{
	return m_shadingLanguageVersion;
}
