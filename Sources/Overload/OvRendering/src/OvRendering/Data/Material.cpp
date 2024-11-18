/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvRendering/Data/Material.h"
#include "OvRendering/Buffers/UniformBuffer.h"
#include "OvRendering/Resources/Texture.h"


//TODO: Add constructor with a shader reference

OvRendering::Data::Material::Material(OvRendering::Resources::Shader* p_shader)
{
	SetShader(p_shader);
}

void OvRendering::Data::Material::SetShader(OvRendering::Resources::Shader* p_shader)
{
	m_shader = p_shader;

	if (m_shader)
	{
		// TODO: Move that line to Engine Material
		// OvRendering::Buffers::UniformBuffer::BindBlockToShader(*m_shader, "EngineUBO");
		FillUniform();
	}
	else
	{
		m_uniformsData.clear();
	}
}

void OvRendering::Data::Material::FillUniform()
{
	m_uniformsData.clear();

	for (const OvRendering::Resources::UniformInfo& element : m_shader->uniforms)
		m_uniformsData.emplace(element.name, element.defaultValue);
}

void OvRendering::Data::Material::Bind(OvRendering::Resources::Texture* p_emptyTexture) const
{
	if (HasShader())
	{
		using namespace OvMaths;
		using namespace OvRendering::Resources;

		m_shader->Bind();

		int textureSlot = 0;

		for (auto& [name, value] : m_uniformsData)
		{
			auto uniformData = m_shader->GetUniformInfo(name);

			if (uniformData)
			{
				switch (uniformData->type)
				{
				case OvRendering::Resources::UniformType::UNIFORM_BOOL:
					if (auto val = std::get_if<bool>(&value))
						m_shader->SetUniformInt(name, *val);
					break;

				case OvRendering::Resources::UniformType::UNIFORM_INT:
					if (auto val = std::get_if<int>(&value))
						m_shader->SetUniformInt(name, *val);
					break;

				case OvRendering::Resources::UniformType::UNIFORM_FLOAT:
					if (auto val = std::get_if<float>(&value))
						m_shader->SetUniformFloat(name, *val);
					break;

				case OvRendering::Resources::UniformType::UNIFORM_FLOAT_VEC2:
					if (auto val = std::get_if<FVector2>(&value))
						m_shader->SetUniformVec2(name, *val);
					break;

				case OvRendering::Resources::UniformType::UNIFORM_FLOAT_VEC3:
					if (auto val = std::get_if<FVector3>(&value))
						m_shader->SetUniformVec3(name, *val);
					break;

				case OvRendering::Resources::UniformType::UNIFORM_FLOAT_VEC4:
					if (auto val = std::get_if<FVector4>(&value))
						m_shader->SetUniformVec4(name, *val);
					break;

				case OvRendering::Resources::UniformType::UNIFORM_FLOAT_MAT4:
					if (auto val = std::get_if<FMatrix4>(&value))
						m_shader->SetUniformMat4(name, *val);
					break;

				case OvRendering::Resources::UniformType::UNIFORM_SAMPLER_2D:
					if (auto val = std::get_if<TextureHandle*>(&value))
					{
						if (const auto handle = static_cast<TextureHandle*>(*val))
						{
							handle->Bind(textureSlot);
							m_shader->SetUniformInt(uniformData->name, textureSlot++);
						}
						else if (p_emptyTexture)
						{
							p_emptyTexture->Bind(textureSlot);
							m_shader->SetUniformInt(uniformData->name, textureSlot++);
						}
					}
					break;
				}
			}
		}
	}
}

void OvRendering::Data::Material::UnBind() const
{
	if (HasShader())
	{
		m_shader->Unbind();
	}
}

void OvRendering::Data::Material::Set(const std::string p_key, const Resources::UniformVariant& p_value)
{
	if (HasShader())
	{
		if (m_uniformsData.find(p_key) != m_uniformsData.end())
		{
			m_uniformsData[p_key] = p_value;
		}
		else
		{
			// OVLOG_WARNING("Material Set failed: Uniform \"" + p_key + "\" not found");
		}
	}
	else
	{
		OVLOG_ERROR("Material Set failed: No attached shader");
	}
}

const OvRendering::Resources::UniformVariant& OvRendering::Data::Material::Get(const std::string p_key) const
{
	OVASSERT(m_uniformsData.find(p_key) != m_uniformsData.end(), "Material Get failed: Uniform \"" + p_key + "\" not found");
	return m_uniformsData.at(p_key);
}

OvRendering::Resources::Shader*& OvRendering::Data::Material::GetShader()
{
	return m_shader;
}

bool OvRendering::Data::Material::HasShader() const
{
	return m_shader;
}

bool OvRendering::Data::Material::IsValid() const
{
	return HasShader();
}

void OvRendering::Data::Material::SetBlendable(bool p_transparent)
{
	m_blendable = p_transparent;
}

void OvRendering::Data::Material::SetBackfaceCulling(bool p_backfaceCulling)
{
	m_backfaceCulling = p_backfaceCulling;
}

void OvRendering::Data::Material::SetFrontfaceCulling(bool p_frontfaceCulling)
{
	m_frontfaceCulling = p_frontfaceCulling;
}

void OvRendering::Data::Material::SetDepthTest(bool p_depthTest)
{
	m_depthTest = p_depthTest;
}

void OvRendering::Data::Material::SetDepthWriting(bool p_depthWriting)
{
	m_depthWriting = p_depthWriting;
}

void OvRendering::Data::Material::SetColorWriting(bool p_colorWriting)
{
	m_colorWriting = p_colorWriting;
}

void OvRendering::Data::Material::SetCastShadows(bool p_castShadows)
{
	m_castShadows = p_castShadows;
}

void OvRendering::Data::Material::SetReceiveShadows(bool p_receiveShadows)
{
	m_receiveShadows = p_receiveShadows;
}

void OvRendering::Data::Material::SetGPUInstances(int p_instances)
{
	m_gpuInstances = p_instances;
}

bool OvRendering::Data::Material::IsBlendable() const
{
	return m_blendable;
}

bool OvRendering::Data::Material::HasBackfaceCulling() const
{
	return m_backfaceCulling;
}

bool OvRendering::Data::Material::HasFrontfaceCulling() const
{
	return m_frontfaceCulling;
}

bool OvRendering::Data::Material::HasDepthTest() const
{
	return m_depthTest;
}

bool OvRendering::Data::Material::HasDepthWriting() const
{
	return m_depthWriting;
}

bool OvRendering::Data::Material::HasColorWriting() const
{
	return m_colorWriting;
}

bool OvRendering::Data::Material::IsShadowCaster() const
{
	return m_castShadows;
}

bool OvRendering::Data::Material::IsShadowReceiver() const
{
	return m_receiveShadows;
}

int OvRendering::Data::Material::GetGPUInstances() const
{
	return m_gpuInstances;
}

const OvRendering::Data::StateMask OvRendering::Data::Material::GenerateStateMask() const
{
	StateMask stateMask;
	stateMask.depthWriting = m_depthWriting;
	stateMask.colorWriting = m_colorWriting;
	stateMask.blendable = m_blendable;
	stateMask.depthTest = m_depthTest;
	stateMask.frontfaceCulling = m_frontfaceCulling;
	stateMask.backfaceCulling = m_backfaceCulling;
	return stateMask;
}

std::map<std::string, OvRendering::Resources::UniformVariant>& OvRendering::Data::Material::GetUniformsData()
{
	return m_uniformsData;
}
