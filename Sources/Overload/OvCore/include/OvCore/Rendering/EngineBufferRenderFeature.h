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
		struct EngineBufferDescriptor
		{
			OvRendering::Entities::Camera& camera;
		};

		/**
		* Constructor
		* @param p_renderer
		*/
		EngineBufferRenderFeature(OvRendering::Core::CompositeRenderer& p_renderer);

		/**
		* Invoked at the beginning of a frame, bind the engine buffer and update elapsed time
		* @param p_frameDescriptor
		*/
		virtual void OnBeginFrame(const OvRendering::Data::FrameDescriptor& p_frameDescriptor) override;

		/**
		* Invoked at the end of a frame, unbind the engine buffer
		*/
		virtual void OnEndFrame() override;

		/**
		* Invoked before drawing, handle uploading entity data to the GPU
		* @param p_drawable
		*/
		virtual void OnBeforeDraw(const OvRendering::Entities::Drawable& p_drawable) override;

	private:
		std::chrono::high_resolution_clock::time_point m_startTime;
		std::unique_ptr<OvRendering::Buffers::UniformBuffer> m_engineBuffer;
	};
}
