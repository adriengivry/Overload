/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "OvRendering/Core/CompositeRenderer.h"

namespace OvRendering::Core
{
	template <typename T, typename... Args>
	T& CompositeRenderer::AddFeature(Args&&... args)
	{
		OVASSERT(!m_isDrawing, "You cannot add a render feature while drawing.");
		OVASSERT(!HasFeature<T>(), "Feature already added");
		static_assert(std::is_base_of<Features::ARenderFeature, T>::value, "T must inherit from ARenderFeature");
		T* feature = new T(*this, std::forward<Args>(args)...);
		m_features.emplace(typeid(T), feature);
		return dynamic_cast<T&>(*feature);
	}

	template<typename T>
	inline bool CompositeRenderer::RemoveFeature()
	{
		OVASSERT(!m_isDrawing, "You cannot add remove a render feature while drawing.");
		static_assert(std::is_base_of<Features::ARenderFeature, T>::value, "T should derive from ARenderFeature");

		if (auto it = m_features.find(typeid(T)); it != m_features.end())
		{
			m_features.erase(it);
			return true;
		}

		return false;
	}

	template<typename T>
	inline T& CompositeRenderer::GetFeature()
	{
		static_assert(std::is_base_of<Features::ARenderFeature, T>::value, "T should derive from ARenderFeature");
		auto it = m_features.find(typeid(T));
		OVASSERT(it != m_features.end(), "Couldn't find a render feature matching the given type T.");
		return *dynamic_cast<T*>(it->second.get());
	}

	template<typename T>
	inline bool CompositeRenderer::HasFeature() const
	{
		static_assert(std::is_base_of<Features::ARenderFeature, T>::value, "T should derive from ARenderFeature");
		auto it = m_features.find(typeid(T));
		return it != m_features.end();
	}

	template<typename T>
	inline void CompositeRenderer::AddDescriptor(T&& p_descriptor)
	{
		OVASSERT(!HasDescriptor<T>(), "Descriptor already added");
		m_descriptors.emplace(typeid(T), std::move(p_descriptor));
	}

	template<typename T>
	inline T& CompositeRenderer::GetDescriptor()
	{
		auto it = m_descriptors.find(typeid(T));
		OVASSERT(it != m_descriptors.end(), "Couldn't find a descriptor matching the given type T.");
		return std::any_cast<T&>(it->second);
	}

	template<typename T>
	inline bool CompositeRenderer::HasDescriptor() const
	{
		auto it = m_descriptors.find(typeid(T));
		return it != m_descriptors.end();
	}
}
