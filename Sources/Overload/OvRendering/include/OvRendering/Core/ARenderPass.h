/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "OvRendering/Data/FrameDescriptor.h"
#include "OvRendering/Settings/ERenderPassOrder.h"

namespace OvRendering::Core
{
	class CompositeRenderer;
}

namespace OvRendering::Core
{
	class ARenderPass
	{
	public:
		/**
		* Constructor
		* @param p_renderer
		*/
		ARenderPass(Core::CompositeRenderer& p_renderer);

		/**
		* Destructor
		*/
		virtual ~ARenderPass() = default;

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
		* Draw for the render pass
		* @param p_pso
		*/
		virtual void Draw(OvRendering::Data::PipelineState p_pso) = 0;

		Core::CompositeRenderer& m_renderer;

		friend class Core::CompositeRenderer;
	};
}