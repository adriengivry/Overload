/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvDebug/Assertion.h>

#include "OvRendering/Data/Describable.h"

namespace OvRendering::Data
{
	template<typename T>
	inline void Describable::AddDescriptor(T&& p_descriptor)
	{
		OVASSERT(!HasDescriptor<T>(), "Descriptor already added");
		m_descriptors.emplace(typeid(T), std::move(p_descriptor));
	}

	template<typename T>
	inline void Describable::RemoveDescriptor()
	{
		OVASSERT(!HasDescriptor<T>(), "Descriptor doesn't exist.");

		auto it = m_descriptors.find(typeid(T));
		if (it != m_descriptors.end())
		{
			m_descriptors.erase(it);
		}
	}

	template<typename T>
	inline bool Describable::HasDescriptor() const
	{
		auto it = m_descriptors.find(typeid(T));
		return it != m_descriptors.end();
	}

	template<typename T>
	inline T& Describable::GetDescriptor()
	{
		auto it = m_descriptors.find(typeid(T));
		OVASSERT(it != m_descriptors.end(), "Couldn't find a descriptor matching the given type T.");
		return std::any_cast<T&>(it->second);
	}

	template<typename T>
	inline bool Describable::TryGetDescriptor(T& p_outDescriptor) const
	{
		auto it = m_descriptors.find(typeid(T));
		if (it != m_descriptors.end())
		{
			p_outDescriptor = std::any_cast<T&>(it->second);
			return true;
		}

		return false;
	}
}
