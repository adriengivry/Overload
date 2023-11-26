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

void OvRendering::Core::ABaseRenderer::BeginFrame(std::optional<Data::RenderOutputDesc> p_outputDesc)
{
	OVASSERT(!s_isDrawing, "Critical Error: Cannot call BeginFrame() when previous frame hasn't finished.");

	if (p_outputDesc)
	{
		m_target = std::make_optional(std::ref(p_outputDesc.value().framebuffer));
	}

	if (m_target)
	{
		m_target->get().Bind();
	}

	m_isDrawing = true;
	s_isDrawing.store(true);
}

void OvRendering::Core::ABaseRenderer::EndFrame()
{
	OVASSERT(s_isDrawing, "Critical Error: Cannot call EndFrame() before calling BeginFrame()");

	if (m_target)
	{
		m_target->get().Unbind();
	}

	m_isDrawing = false;
	s_isDrawing.store(false);
}

OvRendering::Context::Driver& OvRendering::Core::ABaseRenderer::GetDriver() const
{
	return m_driver;
}

void OvRendering::Core::ABaseRenderer::DrawEntity(const Entities::Drawable& p_drawable)
{
	if (p_drawable.material.HasShader() && p_drawable.material.GetGPUInstances() > 0)
	{
		m_driver.ApplyStateMask(p_drawable.stateMask);
		p_drawable.material.Bind(m_emptyTexture);
		DrawMesh(p_drawable.mesh, OvRendering::Settings::EPrimitiveMode::TRIANGLES, p_drawable.material.GetGPUInstances());
		p_drawable.material.UnBind();
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
