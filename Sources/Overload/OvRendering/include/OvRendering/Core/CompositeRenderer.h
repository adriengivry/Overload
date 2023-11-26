/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <typeindex>
#include <memory>

#include "OvRendering/Core/ABaseRenderer.h"
#include "OvRendering/Features/ARenderFeature.h"

#include <OvDebug/Assertion.h>

namespace OvRendering::Core
{
	/**
	* A simple base renderer that doesn't handle any object binding, but provide a strong base for other renderers
	* to implement their own logic.
	*/
	class CompositeRenderer : public ABaseRenderer
	{
	public:
		/**
		* Constructor of the base renderer
		* @param p_driver
		*/
		CompositeRenderer(Context::Driver& p_driver);

		/**
		* Begin Frame
		* @param p_outputDesc
		*/
		virtual void BeginFrame(std::optional<Data::RenderOutputDesc> p_outputDesc);

		/**
		* End Frame
		*/
		virtual void EndFrame();

		/**
		* Draw a drawable entity
		* @param p_drawable
		*/
		virtual void DrawEntity(const Entities::Drawable& p_drawable) override;

		/**
		* Add a render feature to the renderer
		* @param p_args (Parameter pack forwared to the render feature constructor)
		*/
		template<typename T, typename ... Args>
		T& AddFeature(Args&&... p_args);

		/**
		* Remove the given render feature
		*/
		template<typename T>
		bool RemoveFeature();

		/**
		* Retrieve the render feature matching the given type
		* @note Fails if the feature doesn't exist
		*/
		template<typename T>
		T& GetFeature();

		/**
		* Try to get the given render feature (Returns nullptr on failure)
		*/
		template<typename T>
		bool HasFeature() const;

	protected:
		std::unordered_map<std::type_index, std::unique_ptr<Features::ARenderFeature>> m_renderFeatures;
	};
}

#include "OvRendering/Core/CompositeRenderer.inl"
