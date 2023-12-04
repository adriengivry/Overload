/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <atomic>

#include "OvRendering/Core/IRenderer.h"
#include "OvRendering/Data/FrameInfo.h"
#include "OvRendering/Resources/IMesh.h"
#include "OvRendering/Resources/Texture.h"
#include "OvRendering/Entities/Drawable.h"

#include <OvDebug/Assertion.h>

namespace OvRendering::Core
{
	/**
	* A simple base renderer that doesn't handle any object binding, but provide a strong base for other renderers
	* to implement their own logic.
	*/
	class ABaseRenderer : public IRenderer
	{
	public:
		/**
		* Constructor of the base renderer
		* @param p_driver
		*/
		ABaseRenderer(Context::Driver& p_driver);

		/**
		* Destructor of the base renderer
		*/
		virtual ~ABaseRenderer();

		/**
		* Begin Frame
		* @param p_frameDescriptor
		*/
		virtual void BeginFrame(const Data::FrameDescriptor& p_frameDescriptor);

		/**
		* End Frame
		*/
		virtual void EndFrame();

		/**
		* Returns a reference to the current frame descriptor
		* @note Cannot be called outside of a frame drawing operation
		*/
		const Data::FrameDescriptor& GetFrameDescriptor() const;

		/**
		* Create a pipeline state object.
		* The settings are set with default settings provided by the renderer
		*/
		Data::PipelineState CreatePipelineState() const;

		/**
		 * Read a block of pixels from the frame buffer.
		 * @param p_pso
		 * @param p_x
		 * @param p_y
		 * @param p_width
		 * @param p_height
		 * @param p_format
		 * @param p_type
		 * @param p_data
		 */
		// TODO: Move out of the base renderer, or rework
		void ReadPixels(
			OvRendering::Data::PipelineState p_pso,
			uint32_t p_x,
			uint32_t p_y,
			uint32_t p_width,
			uint32_t p_height,
			Settings::EPixelDataFormat p_format,
			Settings::EPixelDataType p_type,
			void* p_data
		) const;

		/**
		* Clear the screen
		* @param p_pso
		* @param p_color
		* @param p_colorBuffer
		* @param p_depthBuffer
		* @param p_stencilBuffer
		*/
		virtual void Clear(
			OvRendering::Data::PipelineState p_pso,
			const OvMaths::FVector3& p_color,
			bool p_colorBuffer,
			bool p_depthBuffer,
			bool p_stencilBuffer
		);

		/**
		* Draw a drawable entity
		* @param p_pso
		* @param p_drawable
		*/
		virtual void DrawEntity(
			OvRendering::Data::PipelineState p_pso,
			const Entities::Drawable& p_drawable
		);

		/**
		* Draw a mesh
		* @param p_pso
		* @param p_mesh
		* @param p_primitiveMode
		* @param p_instances
		*/
		virtual void DrawMesh(
			OvRendering::Data::PipelineState p_pso,
			const Resources::IMesh& p_mesh,
			Settings::EPrimitiveMode p_primitiveMode = Settings::EPrimitiveMode::TRIANGLES,
			uint32_t p_instances = 1
		);

		/**
		* Draw a model with a single material
		* @param p_pso
		* @param p_model
		* @param p_material
		* @param p_modelMatrix
		* @param p_fallbackMaterial (material to use if the given material is invalid)
		*/
		virtual void DrawModelWithSingleMaterial(
			OvRendering::Data::PipelineState p_pso,
			OvRendering::Resources::Model& p_model,
			OvRendering::Data::Material& p_material,
			const OvMaths::FMatrix4& p_modelMatrix,
			std::optional<std::reference_wrapper<OvRendering::Data::Material>> p_fallbackMaterial = std::nullopt
		);

	protected:
		Data::FrameDescriptor m_frameDescriptor;
		Context::Driver& m_driver;
		OvRendering::Resources::Texture* m_emptyTexture;
		OvRendering::Data::PipelineState m_basePipelineState;
		bool m_isDrawing;

	private:
		static std::atomic_bool s_isDrawing;
	};
}
