/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <GL/glew.h>

#include <OvDebug/Logger.h>

#include "OvRendering/Resources/Texture.h"
#include "OvRendering/Resources/Shader.h"

OvRendering::Resources::Shader::Shader(const std::string p_path, uint32_t p_id) : path(p_path), id(p_id)
{
	QueryUniforms();
}

OvRendering::Resources::Shader::~Shader()
{
	glDeleteProgram(id);
}

void OvRendering::Resources::Shader::Bind() const
{
	glUseProgram(id);
}

void OvRendering::Resources::Shader::Unbind() const
{
	glUseProgram(0);
}

void OvRendering::Resources::Shader::SetUniformInt(const std::string& p_name, int p_value)
{
	glUniform1i(GetUniformLocation(p_name), p_value);
}

void OvRendering::Resources::Shader::SetUniformFloat(const std::string& p_name, float p_value)
{
	glUniform1f(GetUniformLocation(p_name), p_value);
}

void OvRendering::Resources::Shader::SetUniformVec2(const std::string & p_name, const OvMaths::FVector2 & p_vec2)
{
	glUniform2f(GetUniformLocation(p_name), p_vec2.x, p_vec2.y);
}

void OvRendering::Resources::Shader::SetUniformVec3(const std::string& p_name, const OvMaths::FVector3& p_vec3)
{
	glUniform3f(GetUniformLocation(p_name), p_vec3.x, p_vec3.y, p_vec3.z);
}

void OvRendering::Resources::Shader::SetUniformVec4(const std::string& p_name, const OvMaths::FVector4& p_vec4)
{
	glUniform4f(GetUniformLocation(p_name), p_vec4.x, p_vec4.y, p_vec4.z, p_vec4.w);
}

void OvRendering::Resources::Shader::SetUniformMat4(const std::string& p_name, const OvMaths::FMatrix4& p_mat4)
{
	glUniformMatrix4fv(GetUniformLocation(p_name), 1, GL_TRUE, &p_mat4.data[0]);
}

int OvRendering::Resources::Shader::GetUniformInt(const std::string& p_name)
{
	int value;
	glGetUniformiv(id, GetUniformLocation(p_name), &value);
	return value;
}

float OvRendering::Resources::Shader::GetUniformFloat(const std::string& p_name)
{
	float value;
	glGetUniformfv(id, GetUniformLocation(p_name), &value);
	return value;
}

OvMaths::FVector2 OvRendering::Resources::Shader::GetUniformVec2(const std::string& p_name)
{
	GLfloat values[2];
	glGetUniformfv(id, GetUniformLocation(p_name), values);
	return reinterpret_cast<OvMaths::FVector2&>(values);
}

OvMaths::FVector3 OvRendering::Resources::Shader::GetUniformVec3(const std::string& p_name)
{
	GLfloat values[3];
	glGetUniformfv(id, GetUniformLocation(p_name), values);
	return reinterpret_cast<OvMaths::FVector3&>(values);
}

OvMaths::FVector4 OvRendering::Resources::Shader::GetUniformVec4(const std::string& p_name)
{
	GLfloat values[4];
	glGetUniformfv(id, GetUniformLocation(p_name), values);
	return reinterpret_cast<OvMaths::FVector4&>(values);
}

OvMaths::FMatrix4 OvRendering::Resources::Shader::GetUniformMat4(const std::string& p_name)
{
	GLfloat values[16];
	glGetUniformfv(id, GetUniformLocation(p_name), values);
	return reinterpret_cast<OvMaths::FMatrix4&>(values);
}

bool OvRendering::Resources::Shader::IsEngineUBOMember(const std::string & p_uniformName)
{
	return p_uniformName.rfind("ubo_", 0) == 0;
}

uint32_t OvRendering::Resources::Shader::GetUniformLocation(const std::string& name)
{
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
		return m_uniformLocationCache.at(name);

	const int location = glGetUniformLocation(id, name.c_str());

	if (location == -1)
		OVLOG_WARNING("Uniform: '" + name + "' doesn't exist");

	m_uniformLocationCache[name] = location;

	return location;
}

void OvRendering::Resources::Shader::QueryUniforms()
{
	GLint numActiveUniforms = 0;
	uniforms.clear();
	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
	std::vector<GLchar> nameData(256);
	for (int unif = 0; unif < numActiveUniforms; ++unif)
	{
		GLint arraySize = 0;
		GLenum type = 0;
		GLsizei actualLength = 0;
		glGetActiveUniform(id, unif, static_cast<GLsizei>(nameData.size()), &actualLength, &arraySize, &type, &nameData[0]);
		std::string name(static_cast<char*>(nameData.data()), actualLength);

		if (!IsEngineUBOMember(name))
		{
			std::any defaultValue;

			switch (static_cast<UniformType>(type))
			{
			case OvRendering::Resources::UniformType::UNIFORM_BOOL:			defaultValue = std::make_any<bool>(GetUniformInt(name));					break;
			case OvRendering::Resources::UniformType::UNIFORM_INT:			defaultValue = std::make_any<int>(GetUniformInt(name));						break;
			case OvRendering::Resources::UniformType::UNIFORM_FLOAT:		defaultValue = std::make_any<float>(GetUniformFloat(name));					break;
			case OvRendering::Resources::UniformType::UNIFORM_FLOAT_VEC2:	defaultValue = std::make_any<OvMaths::FVector2>(GetUniformVec2(name));		break;
			case OvRendering::Resources::UniformType::UNIFORM_FLOAT_VEC3:	defaultValue = std::make_any<OvMaths::FVector3>(GetUniformVec3(name));		break;
			case OvRendering::Resources::UniformType::UNIFORM_FLOAT_VEC4:	defaultValue = std::make_any<OvMaths::FVector4>(GetUniformVec4(name));		break;
			case OvRendering::Resources::UniformType::UNIFORM_SAMPLER_2D:	defaultValue = std::make_any<OvRendering::Resources::Texture*>(nullptr);	break;
			}

			if (defaultValue.has_value())
			{
				uniforms.push_back
				({
					static_cast<UniformType>(type),
					name,
					GetUniformLocation(nameData.data()),
					defaultValue
				});
			}
		}
	}
}

const OvRendering::Resources::UniformInfo* OvRendering::Resources::Shader::GetUniformInfo(const std::string& p_name) const
{
	auto found = std::find_if(uniforms.begin(), uniforms.end(), [&p_name](const UniformInfo& p_element)
	{
		return p_name == p_element.name;
	});

	if (found != uniforms.end())
		return &*found;
	else
		return nullptr;
}
