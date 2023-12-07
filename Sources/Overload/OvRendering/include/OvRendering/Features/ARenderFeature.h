/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "OvRendering/Data/FrameDescriptor.h"
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
		/**
		* Constructor
		* @param p_renderer
		*/
		ARenderFeature(Core::CompositeRenderer& p_renderer);

		/**
		* Destructor
		*/
		virtual ~ARenderFeature() = default;

	protected:
		/**
		* Invoked when the BeginFrame is called on the associated base renderer
		* @param p_frameDescriptor
		*/
		virtual void OnBeginFrame(const Data::FrameDescriptor& p_frameDescriptor);

		/**
		* Invoked when the EndFrame is called on the associated base renderer
		*/
		virtual void OnEndFrame();

		/**
		* Invoked before drawing a drawable entity
		* @param p_drawable
		*/
		virtual void OnBeforeDraw(Data::PipelineState& p_pso, const Entities::Drawable& p_drawable);

		/**
		* Invoked after drawing a drawable entity
		* @param p_drawable
		*/
		virtual void OnAfterDraw(const Entities::Drawable& p_drawable);

		Core::CompositeRenderer& m_renderer;

		friend class Core::CompositeRenderer;
	};
}