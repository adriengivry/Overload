/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <optional>

#include "OvRendering/Data/FrameDescriptor.h"
#include "OvRendering/Entities/Drawable.h"
#include "OvRendering/Settings/ERenderPass.h"

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
		virtual ~ARenderFeature() = default; // TODO: Maybe make protected

		/**
		* Returns the render pass flags associated with this feature
		*/
		virtual OvRendering::Settings::ERenderPassMask GetRenderPassMask() const;

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
		* Draw before the given render pass
		* If the attached renderer also implements a drawing strategy for this pass,
		* this pre-pass will be executed before
		* @param p_renderPass
		*/
		virtual void DrawPrePass(OvRendering::Settings::ERenderPass p_renderPass);

		/**
		* Draw for the given render pass
		* If the attached renderer also implements a drawing strategy for this pass,
		* the render feature draw pass will be executed last.
		* @param p_renderPass
		*/
		virtual void DrawPass(OvRendering::Settings::ERenderPass p_renderPass);

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

	protected:
		Core::CompositeRenderer& m_renderer;
	};
}