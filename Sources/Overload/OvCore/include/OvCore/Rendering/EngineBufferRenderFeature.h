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
		* Temporarily overrides the camera with the given one
		* @param p_camera
		*/
		void ApplyOverrideCamera(OvRendering::Entities::Camera& p_camera);

		/**
		* Revert the camera override
		*/
		void RevertOverrideCamera();

	protected:
		virtual void OnBeginFrame(const OvRendering::Data::FrameDescriptor& p_frameDescriptor) override;
		virtual void OnEndFrame() override;
		virtual void OnBeforeDraw(OvRendering::Data::PipelineState& p_pso, const OvRendering::Entities::Drawable& p_drawable) override;

	protected:
		std::chrono::high_resolution_clock::time_point m_startTime;
		std::unique_ptr<OvRendering::Buffers::UniformBuffer> m_engineBuffer;
		OvRendering::Data::FrameDescriptor m_cachedFrameDescriptor;
	};
}
