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
		OVASSERT(!m_isDrawing, "Critical Error: You cannot add a render feature while drawing.");
		OVASSERT(!HasFeature<T>(), "Critical Error: feature already added");
		static_assert(std::is_base_of<Features::ARenderFeature, T>::value, "T must inherit from ARenderFeature");
		T* feature = new T(*this, std::forward<Args>(args)...);
		m_renderFeatures.emplace(typeid(T), feature);
		return dynamic_cast<T&>(*feature);
	}

	template<typename T>
	inline bool CompositeRenderer::RemoveFeature()
	{
		OVASSERT(!m_isDrawing, "Critical Error: You cannot add remove a render feature while drawing.");
		static_assert(std::is_base_of<Features::ARenderFeature, T>::value, "T should derive from ARenderFeature");

		if (auto it = m_renderFeatures.find(typeid(T)); it != m_renderFeatures.end())
		{
			m_renderFeatures.erase(it);
			return true;
		}

		return false;
	}

	template<typename T>
	inline T& CompositeRenderer::GetFeature()
	{
		static_assert(std::is_base_of<Features::ARenderFeature, T>::value, "T should derive from ARenderFeature");
		auto it = m_renderFeatures.find(typeid(T));
		OVASSERT(it == m_renderFeatures.end(), "Couldn't find a render feature matching given type T.");
		return *dynamic_cast<T*>(it->second.get());
	}

	template<typename T>
	inline bool CompositeRenderer::HasFeature() const
	{
		static_assert(std::is_base_of<Features::ARenderFeature, T>::value, "T should derive from ARenderFeature");
		auto it = m_renderFeatures.find(typeid(T));
		return it != m_renderFeatures.end();
	}
}