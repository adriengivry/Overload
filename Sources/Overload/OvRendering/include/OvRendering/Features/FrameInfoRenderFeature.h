/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "OvRendering/Core/CompositeRenderer.h"
#include "OvRendering/Features/ARenderFeature.h"
#include "OvRendering/Data/FrameInfo.h"

namespace OvRendering::Features
{
	class FrameInfoRenderFeature : public OvRendering::Features::ARenderFeature
	{
	public:
		/**
		* Constructor
		* @param p_renderer
		*/
		FrameInfoRenderFeature(OvRendering::Core::CompositeRenderer& p_renderer);

		/**
		* Invoked when a frame begins. Take care of initializing the frame info data
		* @param p_outputDesc
		*/
		virtual void OnBeginFrame(std::optional<Data::RenderOutputDesc>& p_outputDesc) override;

		/**
		* Invoked when a frame ends. Validate the frame info data
		*/
		virtual void OnEndFrame() override;

		/**
		* Invoked after drawing an entity. Append drawing info to the frame info.
		* @param p_drawable
		*/
		virtual void OnAfterDraw(const OvRendering::Entities::Drawable& p_drawable) override;

		/**
		* Return a reference to the last frame info
		* @note Will throw an error if called during the rendering of a frame
		*/
		const OvRendering::Data::FrameInfo& GetFrameInfo() const;

	private:
		bool m_isFrameInfoDataValid;
		OvRendering::Data::FrameInfo m_frameInfo;
	};
}
