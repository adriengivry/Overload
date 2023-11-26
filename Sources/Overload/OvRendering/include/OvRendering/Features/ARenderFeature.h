/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <optional>

#include "OvRendering/Data/RenderOutputDesc.h"
#include "OvRendering/Entities/Drawable.h"

namespace OvRendering::Core
{
	class CompositeRenderer;
}

namespace OvRendering::Features
{
	class ARenderFeature
	{
	public:
		virtual ~ARenderFeature() = default; // TODO: Maybe make protected

		/**
		* Returns true if the render feature requirements and dependencies are met.
		*/
		virtual bool Validate() const;

		/**
		* Invoked before drawing a drawable entity
		* @param p_drawable
		*/
		virtual void OnBeforeDraw(const Entities::Drawable& p_drawable);

		/**
		* Invoked after drawing a drawable entity
		* @param p_drawable
		*/
		virtual void OnAfterDraw(const Entities::Drawable& p_drawable);

		/**
		* Invoked when the BeginFrame is called on the associated base renderer
		* @param p_outputDesc
		*/
		virtual void OnBeginFrame(std::optional<Data::RenderOutputDesc>& p_outputDesc);

		/**
		* Invoked when the EndFrame is called on the associated base renderer
		*/
		virtual void OnEndFrame();

	protected:
		ARenderFeature(Core::CompositeRenderer& p_renderer);

		Core::CompositeRenderer& m_renderer;
		Context::Driver& m_driver;
	};
}