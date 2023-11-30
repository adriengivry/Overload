/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <functional>

#include "OvRendering/Core/ABaseRenderer.h"
#include "OvRendering/Resources/Loaders/TextureLoader.h"

std::atomic_bool OvRendering::Core::ABaseRenderer::s_isDrawing{ false };

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

	m_frameDescriptor = p_frameDescriptor;

	m_driver.UpdateStateMask();
	m_previousStateMask = m_driver.GetStateMask();

	if (p_frameDescriptor.outputBuffer)
	{
		p_frameDescriptor.outputBuffer->Bind();
	}

	m_driver.SetViewPort(0, 0, p_frameDescriptor.renderWidth, p_frameDescriptor.renderHeight);

	if (p_frameDescriptor.clearColorBuffer || p_frameDescriptor.clearDepthBuffer || p_frameDescriptor.clearStencilBuffer)
	{
		Clear(
			p_frameDescriptor.clearColor,
			p_frameDescriptor.clearColorBuffer,
			p_frameDescriptor.clearDepthBuffer,
			p_frameDescriptor.clearStencilBuffer
		);
	}

	m_isDrawing = true;
	s_isDrawing.store(true);
}

void OvRendering::Core::ABaseRenderer::EndFrame()
{
	OVASSERT(s_isDrawing, "Cannot call EndFrame() before calling BeginFrame()");

	m_driver.ApplyStateMask(m_previousStateMask);

	if (m_frameDescriptor.value().outputBuffer)
	{
		m_frameDescriptor.value().outputBuffer->Unbind();
	}

	m_isDrawing = false;
	s_isDrawing.store(false);
}

OvRendering::Context::Driver& OvRendering::Core::ABaseRenderer::GetDriver() const
{
	return m_driver;
}

void OvRendering::Core::ABaseRenderer::Clear(const OvMaths::FVector3& p_color, bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer)
{
	m_driver.SetClearColor(p_color.x, p_color.y, p_color.z);
	m_driver.Clear(p_colorBuffer, p_depthBuffer, p_stencilBuffer);
}

const OvRendering::Data::FrameDescriptor& OvRendering::Core::ABaseRenderer::GetFrameDescriptor() const
{
	OVASSERT(m_frameDescriptor.has_value(), "Cannot call GetFrameDescriptor() outside of a frame");
	return m_frameDescriptor.value();
}

void OvRendering::Core::ABaseRenderer::DrawEntity(const Entities::Drawable& p_drawable)
{
	auto material = p_drawable.material;
	auto mesh = p_drawable.mesh;

	if (mesh && material && material.get().HasShader() && material.get().GetGPUInstances() > 0)
	{
		m_driver.ApplyStateMask(p_drawable.stateMask);
		p_drawable.material.get().Bind(m_emptyTexture);
		DrawMesh(mesh.get(), OvRendering::Settings::EPrimitiveMode::TRIANGLES, p_drawable.material.get().GetGPUInstances());
		p_drawable.material.get().UnBind();
	}
}

void OvRendering::Core::ABaseRenderer::DrawMesh(const Resources::IMesh& p_mesh, Settings::EPrimitiveMode p_primitiveMode, uint32_t p_instances)
{
	if (p_instances > 0)
	{
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

			DrawEntity(element);
		}
	}
}
