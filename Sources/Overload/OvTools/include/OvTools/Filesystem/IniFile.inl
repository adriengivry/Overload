/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <assert.h>

#include "OvTools/Filesystem/IniFile.h"

namespace OvTools::Filesystem
{
	template<IniType T>
	inline T IniFile::Get(const std::string& p_key)
	{
		if constexpr (std::is_same<bool, T>::value)
		{
			if (!IsKeyExisting(p_key))
				return false;

			return StringToBoolean(m_data[p_key]);
		}
		else if constexpr (std::is_same<std::string, T>::value)
		{
			if (!IsKeyExisting(p_key))
				return std::string("NULL");

			return m_data[p_key];
		}
		else if constexpr (std::is_integral<T>::value)
		{
			if (!IsKeyExisting(p_key))
				return static_cast<T>(0);

			return static_cast<T>(std::atoi(m_data[p_key].c_str()));
		}
		else if constexpr (std::is_floating_point<T>::value)
		{
			if (!IsKeyExisting(p_key))
				return static_cast<T>(0.0f);

			return static_cast<T>(std::atof(m_data[p_key].c_str()));
		}
	}

	template<IniType T>
	inline T IniFile::GetOrDefault(const std::string& p_key, T p_default)
	{
		return IsKeyExisting(p_key) ? Get<T>(p_key) : p_default;
	}

	template<IniType T>
	inline bool IniFile::Set(const std::string& p_key, const T& p_value)
	{
		if (IsKeyExisting(p_key))
		{
			if constexpr (std::is_same<bool, T>::value)
			{
				m_data[p_key] = p_value ? "true" : "false";
			}
			else if constexpr (std::is_same<std::string, T>::value)
			{
				m_data[p_key] = p_value;
			}
			else if constexpr (std::is_integral<T>::value)
			{
				m_data[p_key] = std::to_string(p_value);
			}
			else if constexpr (std::is_floating_point<T>::value)
			{
				m_data[p_key] = std::to_string(p_value);
			}
		}

		return false;
	}

	template<IniType T>
	inline bool IniFile::Add(const std::string & p_key, const T & p_value)
	{
		if (!IsKeyExisting(p_key))
		{
			if constexpr (std::is_same<bool, T>::value)
			{
				RegisterPair(p_key, p_value ? "true" : "false");
			}
			else if constexpr (std::is_same<std::string, T>::value)
			{
				RegisterPair(p_key, p_value);
			}
			else if constexpr (std::is_integral<T>::value)
			{
				RegisterPair(p_key, std::to_string(p_value));
			}
			else if constexpr (std::is_floating_point<T>::value)
			{
				RegisterPair(p_key, std::to_string(p_value));
			}

			return true;
		}

		return false;
	}
}