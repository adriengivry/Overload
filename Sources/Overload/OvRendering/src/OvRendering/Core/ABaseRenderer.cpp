/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <functional>

#include "OvRendering/Core/ABaseRenderer.h"
#include "OvRendering/Resources/Loaders/TextureLoader.h"

std::atomic_bool OvRendering::Core::ABaseRenderer::s_isDrawing{ false };

const OvRendering::Entities::Camera kDefaultCamera;

OvRendering::Core::ABaseRenderer::ABaseRenderer(Context::Driver& p_driver) : 
	m_driver(p_driver),
	m_isDrawing(false),
	m_emptyTexture(OvRendering::Resources::Loaders::TextureLoader::CreatePixel(255, 255, 255, 255))
{
}

OvRendering::Core::ABaseRenderer::~ABaseRenderer()
{
	OvRendering::Resources::Loaders::TextureLoader::Destroy(m_emptyTexture);
}

void OvRendering::Core::ABaseRenderer::BeginFrame(const Data::FrameDescriptor& p_frameDescriptor)
{
	OVASSERT(!s_isDrawing, "Cannot call BeginFrame() when previous frame hasn't finished.");
	OVASSERT(p_frameDescriptor.IsValid(), "Invalid FrameDescriptor!");

	m_frameDescriptor = p_frameDescriptor;

	if (p_frameDescriptor.outputBuffer)
	{
		p_frameDescriptor.outputBuffer.value().Bind();
	}

	m_basePipelineState = m_driver.CreatePipelineState();
	m_driver.SetViewport(0, 0, p_frameDescriptor.renderWidth, p_frameDescriptor.renderHeight);

	Clear(
		p_frameDescriptor.camera->GetClearColorBuffer(),
		p_frameDescriptor.camera->GetClearDepthBuffer(),
		p_frameDescriptor.camera->GetClearStencilBuffer(),
		p_frameDescriptor.camera.value().GetClearColor()
	);

	p_frameDescriptor.camera->CacheMatrices(p_frameDescriptor.renderWidth, p_frameDescriptor.renderHeight);

	m_isDrawing = true;
	s_isDrawing.store(true);
}

void OvRendering::Core::ABaseRenderer::EndFrame()
{
	OVASSERT(s_isDrawing, "Cannot call EndFrame() before calling BeginFrame()");

	if (m_frameDescriptor.outputBuffer)
	{
		m_frameDescriptor.outputBuffer.value().Unbind();
	}

	m_isDrawing = false;
	s_isDrawing.store(false);
}

const OvRendering::Data::FrameDescriptor& OvRendering::Core::ABaseRenderer::GetFrameDescriptor() const
{
	OVASSERT(m_isDrawing, "Cannot call GetFrameDescriptor() outside of a frame");
	return m_frameDescriptor;
}

OvRendering::Data::PipelineState OvRendering::Core::ABaseRenderer::CreatePipelineState() const
{
	return m_basePipelineState;
}

void OvRendering::Core::ABaseRenderer::ReadPixels(
	uint32_t p_x,
	uint32_t p_y,
	uint32_t p_width,
	uint32_t p_height,
	Settings::EPixelDataFormat p_format,
	Settings::EPixelDataType p_type,
	void* p_data
) const
{
	return m_driver.ReadPixels(p_x, p_y, p_width, p_height, p_format, p_type, p_data);
}

void OvRendering::Core::ABaseRenderer::Clear(
	bool p_colorBuffer,
	bool p_depthBuffer,
	bool p_stencilBuffer,
	const OvMaths::FVector4& p_color
)
{
	m_driver.Clear(p_colorBuffer, p_depthBuffer, p_stencilBuffer, p_color);
}

void OvRendering::Core::ABaseRenderer::DrawEntity(
	OvRendering::Data::PipelineState p_pso,
	const Entities::Drawable& p_drawable
)
{
	auto material = p_drawable.material;
	auto mesh = p_drawable.mesh;

	if (mesh && material && material.value().HasShader() && material.value().GetGPUInstances() > 0)
	{
		p_pso.depthWriting = p_drawable.stateMask.depthWriting;
		p_pso.colorWriting.mask = p_drawable.stateMask.colorWriting ? 0xFF : 0x00;
		p_pso.blending = p_drawable.stateMask.blendable;
		p_pso.culling = p_drawable.stateMask.frontfaceCulling || p_drawable.stateMask.backfaceCulling;
		p_pso.depthTest = p_drawable.stateMask.depthTest;

		if (p_pso.culling)
		{
			if (p_drawable.stateMask.backfaceCulling && p_drawable.stateMask.frontfaceCulling)
			{
				p_pso.cullFace = Settings::ECullFace::FRONT_AND_BACK;
			}
			else
			{
				p_pso.cullFace =
					p_drawable.stateMask.backfaceCulling ?
					Settings::ECullFace::BACK :
					Settings::ECullFace::FRONT;
			}
		}

		p_drawable.material.value().Bind(m_emptyTexture);
		DrawMesh(p_pso, mesh.value(), OvRendering::Settings::EPrimitiveMode::TRIANGLES, p_drawable.material.value().GetGPUInstances());
		p_drawable.material.value().UnBind();
	}
}

void OvRendering::Core::ABaseRenderer::DrawMesh(
	OvRendering::Data::PipelineState p_pso,
	const Resources::IMesh& p_mesh,
	Settings::EPrimitiveMode p_primitiveMode,
	uint32_t p_instances
)
{
	if (p_instances > 0)
	{
		m_driver.SetPipelineState(p_pso);

		p_mesh.Bind();
		
		if (p_mesh.GetIndexCount() > 0)
		{
			if (p_instances == 1)
			{
				m_driver.DrawElements(p_primitiveMode, p_mesh.GetIndexCount());
			}
			else
			{
				m_driver.DrawElementsInstanced(p_primitiveMode, p_mesh.GetIndexCount(), p_instances);
			}
		}
		else
		{
			if (p_instances == 1)
			{
				m_driver.DrawArrays(p_primitiveMode, p_mesh.GetVertexCount());
			}
			else
			{
				m_driver.DrawArraysInstanced(p_primitiveMode, p_mesh.GetVertexCount(), p_instances);
			}
		}

		p_mesh.Unbind();
	}
}

void OvRendering::Core::ABaseRenderer::DrawModelWithSingleMaterial(
	OvRendering::Data::PipelineState p_pso,
	OvRendering::Resources::Model& p_model,
	OvRendering::Data::Material& p_material,
	const OvMaths::FMatrix4& p_modelMatrix,
	std::optional<std::reference_wrapper<OvRendering::Data::Material>> p_fallbackMaterial
)
{
	std::optional<std::reference_wrapper<Data::Material>> targetMaterial =
		p_material.GetShader() ?
		std::ref(p_material) :
		p_fallbackMaterial;

	if (targetMaterial)
	{
		Data::Material& material = targetMaterial.value().get();
		Data::StateMask stateMask = material.GenerateStateMask();
		OvMaths::FMatrix4 userMatrix = OvMaths::FMatrix4::Identity;

		for (auto mesh : p_model.GetMeshes())
		{
			OvRendering::Entities::Drawable element;
			element.modelMatrix = p_modelMatrix;
			element.mesh = *mesh;
			element.material = material;
			element.stateMask = stateMask;
			element.userMatrix = userMatrix;

			DrawEntity(p_pso, element);
		}
	}
}
