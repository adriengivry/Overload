/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <map>
#include <chrono>

#include <OvRendering/Features/ARenderFeature.h>
#include <OvRendering/Buffers/UniformBuffer.h>
#include <OvRendering/Entities/Camera.h>

namespace OvCore::Rendering
{
	/**
	* Render feature handling engine buffer (UBO) updates
	*/
	class EngineBufferRenderFeature : public OvRendering::Features::ARenderFeature
	{
	public:
		/**
		* Constructor
		* @param p_renderer
		*/
		EngineBufferRenderFeature(OvRendering::Core::CompositeRenderer& p_renderer);

		/**
		* Upload view data to the GPU
		* @param p_camera
		* @param p_windowWidth
		* @param p_windowHeight
		*/
		void UploadViewData(
			OvRendering::Entities::Camera& p_camera,
			uint16_t p_windowWidth,
			uint16_t p_windowHeight
		);

		/**
		* Invoked before drawing, handle uploading entity data to the GPU
		* @param p_drawable
		*/
		virtual void OnBeforeDraw(const OvRendering::Entities::Drawable& p_drawable) override;

		/**
		* Invoked at the beginning of a frame, bind the engine buffer and update elapsed time
		* @param p_outputDesc
		*/
		virtual void OnBeginFrame(std::optional<OvRendering::Data::RenderOutputDesc>& p_outputDesc) override;

		/**
		* Invoked at the end of a frame, unbind the engine buffer
		*/
		virtual void OnEndFrame() override;

	private:
		std::chrono::high_resolution_clock::time_point m_startTime;
		std::unique_ptr<OvRendering::Buffers::UniformBuffer> m_engineBuffer;
	};
}
