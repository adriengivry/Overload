/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvDebug/Logger.h>

#include "OvRendering/Data/Material.h"

namespace OvRendering::Data
{
	template<typename T>
	inline void Material::Set(const std::string p_key, const T& p_value, bool p_singleUse)
	{
		if (HasShader())
		{
			if (m_properties.find(p_key) != m_properties.end())
			{
				m_properties[p_key] = { std::any(p_value), p_singleUse };
			}
		}
		else
		{
			OVLOG_ERROR("Material Set failed: No attached shader");
		}
	}

	template<typename T>
	inline const T& Material::Get(const std::string p_key) const
	{
		if (HasShader())
		{
			if (m_properties.find(p_key) != m_properties.end())
			{
				return std::any_cast<T>(m_properties.at(p_key).value);
			}
			else
			{
				OVLOG_ERROR("Material Get failed: Uniform not found");
			}
		}
		else
		{
			OVLOG_ERROR("Material Get failed: No attached shader");
		}
	}
}