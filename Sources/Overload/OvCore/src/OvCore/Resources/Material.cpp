/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvCore/Resources/Material.h"

void OvCore::Resources::Material::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	using namespace OvCore::Helpers;
	using namespace OvRendering::Resources;
	using namespace OvMaths;

	Serializer::SerializeShader(p_doc, p_node, "shader", m_shader);

	tinyxml2::XMLNode* settingsNode = p_doc.NewElement("settings");
	p_node->InsertEndChild(settingsNode);

	Serializer::SerializeBoolean(p_doc, settingsNode, "blendable", m_blendable);
	Serializer::SerializeBoolean(p_doc, settingsNode, "backface_culling", m_backfaceCulling);
	Serializer::SerializeBoolean(p_doc, settingsNode, "frontface_culling", m_frontfaceCulling);
	Serializer::SerializeBoolean(p_doc, settingsNode, "depth_test", m_depthTest);
	Serializer::SerializeBoolean(p_doc, settingsNode, "depth_writing", m_depthWriting);
	Serializer::SerializeBoolean(p_doc, settingsNode, "color_writing", m_colorWriting);
	Serializer::SerializeBoolean(p_doc, settingsNode, "cast_shadows", m_castShadows);
	Serializer::SerializeBoolean(p_doc, settingsNode, "receive_shadows", m_receiveShadows);
	Serializer::SerializeInt(p_doc, settingsNode, "gpu_instances", m_gpuInstances);

	// Create "Uniforms" (Every uniform will be attached to "Uniforms")
	tinyxml2::XMLNode* uniformsNode = p_doc.NewElement("uniforms");
	p_node->InsertEndChild(uniformsNode);

	for (const auto&[uniformName, uniformValue] : m_uniformsData)
	{
		tinyxml2::XMLNode* uniform = p_doc.NewElement("uniform");
		uniformsNode->InsertEndChild(uniform); // Instead of p_node, use uniformNode (To create)

		const OvRendering::Resources::UniformInfo* uniformInfo = m_shader->GetUniformInfo(uniformName);

		Serializer::SerializeString(p_doc, uniform, "name", uniformName);

		if (uniformInfo && uniformValue.has_value())
		{
			switch (uniformInfo->type)
			{
			case UniformType::UNIFORM_BOOL:
				if (uniformValue.type() == typeid(bool)) Serializer::SerializeInt(p_doc, uniform, "value", std::any_cast<bool>(uniformValue));
				break;

			case UniformType::UNIFORM_INT:
				if (uniformValue.type() == typeid(int)) Serializer::SerializeInt(p_doc, uniform, "value", std::any_cast<int>(uniformValue));
				break;

			case UniformType::UNIFORM_FLOAT:
				if (uniformValue.type() == typeid(float)) Serializer::SerializeFloat(p_doc, uniform, "value", std::any_cast<float>(uniformValue));
				break;

			case UniformType::UNIFORM_FLOAT_VEC2:
				if (uniformValue.type() == typeid(FVector2)) Serializer::SerializeVec2(p_doc, uniform, "value", std::any_cast<FVector2>(uniformValue));
				break;

			case UniformType::UNIFORM_FLOAT_VEC3:
				if (uniformValue.type() == typeid(FVector3)) Serializer::SerializeVec3(p_doc, uniform, "value", std::any_cast<FVector3>(uniformValue));
				break;

			case UniformType::UNIFORM_FLOAT_VEC4:
				if (uniformValue.type() == typeid(FVector4)) Serializer::SerializeVec4(p_doc, uniform, "value", std::any_cast<FVector4>(uniformValue));
				break;

			case UniformType::UNIFORM_SAMPLER_2D:
				if (uniformValue.type() == typeid(Texture*)) Serializer::SerializeTexture(p_doc, uniform, "value", std::any_cast<Texture*>(uniformValue));
				break;
			}
		}
	}
}

void OvCore::Resources::Material::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	using namespace OvCore::Helpers;

	tinyxml2::XMLNode* settingsNode = p_node->FirstChildElement("settings");
	
	if (settingsNode)
	{
		Serializer::DeserializeBoolean(p_doc, settingsNode, "blendable", m_blendable);
		Serializer::DeserializeBoolean(p_doc, settingsNode, "backface_culling", m_backfaceCulling);
		Serializer::DeserializeBoolean(p_doc, settingsNode, "frontface_culling", m_frontfaceCulling);
		Serializer::DeserializeBoolean(p_doc, settingsNode, "depth_test", m_depthTest);
		Serializer::DeserializeBoolean(p_doc, settingsNode, "depth_writing", m_depthWriting);
		Serializer::DeserializeBoolean(p_doc, settingsNode, "color_writing", m_colorWriting);
		Serializer::DeserializeBoolean(p_doc, settingsNode, "cast_shadows", m_castShadows);
		Serializer::DeserializeBoolean(p_doc, settingsNode, "receive_shadows", m_receiveShadows);
		Serializer::DeserializeInt(p_doc, settingsNode, "gpu_instances", m_gpuInstances);
	}

	/* We get the shader with Deserialize method */
	OvRendering::Resources::Shader* deserializedShader = OvCore::Helpers::Serializer::DeserializeShader(p_doc, p_node, "shader");

	/* We verify that the shader is valid (Not null) */
	if (deserializedShader)
	{
		/* If the shader is valid, we set it to the material (Modify m_shader pointer + Query + FillUniforms) */
		SetShader(deserializedShader);

		tinyxml2::XMLNode* uniformsNode = p_node->FirstChildElement("uniforms"); // Access to "Uniforms" (Every uniform will be attached to "Uniforms")		

		if (uniformsNode)
		{
			/* We iterate over every <uniform>...</uniform> */
			for (auto uniform = uniformsNode->FirstChildElement("uniform"); uniform; uniform = uniform->NextSiblingElement("uniform"))
			{
				/* Verify that the uniform node contains a "name" element */
				if (auto uniformNameElement = uniform->FirstChildElement("name"); uniformNameElement)
				{
					const std::string uniformName = uniformNameElement->GetText();

					/* We collect information about the uniform (The uniform is identified in the shader by its name) */
					const OvRendering::Resources::UniformInfo* uniformInfo = m_shader->GetUniformInfo(uniformName);

					/* We verify that the uniform is existant is the current shader */
					if (uniformInfo)
					{
						/* Deserialization of the uniform value depending on the uniform type (Deserialization result to std::any) */
						switch (uniformInfo->type)
						{
						case OvRendering::Resources::UniformType::UNIFORM_BOOL:
							m_uniformsData[uniformInfo->name] = OvCore::Helpers::Serializer::DeserializeBoolean(p_doc, uniform, "value");
							break;

						case OvRendering::Resources::UniformType::UNIFORM_INT:
							m_uniformsData[uniformInfo->name] = OvCore::Helpers::Serializer::DeserializeInt(p_doc, uniform, "value");
							break;

						case OvRendering::Resources::UniformType::UNIFORM_FLOAT:
							m_uniformsData[uniformInfo->name] = OvCore::Helpers::Serializer::DeserializeFloat(p_doc, uniform, "value");
							break;

						case OvRendering::Resources::UniformType::UNIFORM_FLOAT_VEC2:
							m_uniformsData[uniformInfo->name] = OvCore::Helpers::Serializer::DeserializeVec2(p_doc, uniform, "value");
							break;

						case OvRendering::Resources::UniformType::UNIFORM_FLOAT_VEC3:
							m_uniformsData[uniformInfo->name] = OvCore::Helpers::Serializer::DeserializeVec3(p_doc, uniform, "value");
							break;

						case OvRendering::Resources::UniformType::UNIFORM_FLOAT_VEC4:
							m_uniformsData[uniformInfo->name] = OvCore::Helpers::Serializer::DeserializeVec4(p_doc, uniform, "value");
							break;

						case OvRendering::Resources::UniformType::UNIFORM_FLOAT_MAT4:
							m_uniformsData[uniformInfo->name] = OvCore::Helpers::Serializer::DeserializeMat4(p_doc, uniform, "value");
							break;

						case OvRendering::Resources::UniformType::UNIFORM_SAMPLER_2D:
							m_uniformsData[uniformInfo->name] = OvCore::Helpers::Serializer::DeserializeTexture(p_doc, uniform, "value");
							break;
						}
					}
				}
			}
		}
	}
}