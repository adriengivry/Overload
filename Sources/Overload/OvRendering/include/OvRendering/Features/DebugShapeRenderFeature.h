/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "OvRendering/Features/ARenderFeature.h"

namespace OvRendering::Features
{
	/**
	* The ShapeDrawer handles the drawing of basic shapes
	*/
	class DebugShapeRenderFeature : public ARenderFeature
	{
	public:
		/**
		* Constructor
		* @param p_renderer
		*/
		DebugShapeRenderFeature(Core::CompositeRenderer& p_renderer);

		/**
		* Destructor
		*/
		virtual ~DebugShapeRenderFeature();

		/**
		* Invoked when the BeginFrame is called on the associated base renderer
		* @param p_frameDescriptor
		*/
		virtual void OnBeginFrame(const Data::FrameDescriptor& p_frameDescriptor);

		/**
		* Defines the view projection to use when drawing
		* @param p_viewProjection
		*/
		void SetViewProjection(const OvMaths::FMatrix4& p_viewProjection);

		/**
		* Draw a line in world space
		* @param p_pso
		* @param p_start
		* @param p_end
		* @param p_color
		* @param p_lineWidth
		*/
		void DrawLine(
			OvRendering::Data::PipelineState p_pso,
			const OvMaths::FVector3& p_start,
			const OvMaths::FVector3& p_end,
			const OvMaths::FVector3& p_color,
			float p_lineWidth = 1.0f
		);

	private:
		OvRendering::Resources::Shader* m_lineShader = nullptr;
		OvRendering::Resources::Mesh* m_lineMesh = nullptr;
	};
}