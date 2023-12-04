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
#include "OvRendering/Data/Describable.h"

#include <OvDebug/Assertion.h>

namespace OvRendering::Core
{
	/**
	* A renderer relying on composition to define rendering logic.
	*/
	class CompositeRenderer : public ABaseRenderer, public Data::Describable
	{
	public:
		/**
		* Constructor of the base renderer
		* @param p_driver
		*/
		CompositeRenderer(Context::Driver& p_driver);

		/**
		* Begin Frame
		* @param p_frameDescriptor
		*/
		virtual void BeginFrame(const Data::FrameDescriptor& p_frameDescriptor);

		/**
		* Handle the drawing logic of render pass, invoking DrawPass on the renderer and its
		* associated render features.
		*/
		virtual void Draw() final;

		/**
		* Draw the given render pass
		* @param p_pass
		*/
		virtual void DrawPass(OvRendering::Settings::ERenderPass p_pass);

		/**
		* End Frame
		*/
		virtual void EndFrame();

		/**
		* Draw a drawable entity
		* @param p_pso
		* @param p_drawable
		*/
		virtual void DrawEntity(
			OvRendering::Data::PipelineState p_pso,
			const Entities::Drawable& p_drawable
		) override;

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
		* Return true if the a feature matching the given type has been found
		*/
		template<typename T>
		bool HasFeature() const;

	protected:
		std::unordered_map<std::type_index, std::unique_ptr<Features::ARenderFeature>> m_features;
	};
}

#include "OvRendering/Core/CompositeRenderer.inl"
