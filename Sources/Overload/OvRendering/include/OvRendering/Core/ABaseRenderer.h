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
		* Returns the driver associated with this renderer
		*/
		virtual Context::Driver& GetDriver() const;

		/**
		* Clear the screen
		* @param p_color
		* @param p_colorBuffer
		* @param p_depthBuffer
		* @param p_stencilBuffer
		*/
		virtual void Clear(const OvMaths::FVector3& p_color, bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer);

		/**
		* Returns a reference to the current frame descriptor
		* @note Cannot be called outside of a frame drawing operation
		*/
		const Data::FrameDescriptor& GetFrameDescriptor() const;

		/**
		* Draw a drawable entity
		* @param p_drawable
		*/
		virtual void DrawEntity(const Entities::Drawable& p_drawable);

		/**
		* Draw a mesh
		* @param p_mesh
		* @param p_primitiveMode
		* @param p_instances
		*/
		virtual void DrawMesh(const Resources::IMesh& p_mesh, Settings::EPrimitiveMode p_primitiveMode = Settings::EPrimitiveMode::TRIANGLES, uint32_t p_instances = 1);

		/**
		* Draw a model with a single material
		* @param p_model
		* @param p_material
		* @param p_modelMatrix
		* @param p_fallbackMaterial (material to use if the given material is invalid)
		*/
		virtual void DrawModelWithSingleMaterial(
			OvRendering::Resources::Model& p_model,
			OvRendering::Data::Material& p_material,
			const OvMaths::FMatrix4& p_modelMatrix,
			std::optional<std::reference_wrapper<OvRendering::Data::Material>> p_fallbackMaterial = std::nullopt
		);

	protected:
		Data::FrameDescriptor m_frameDescriptor;
		Context::Driver& m_driver;
		OvRendering::Resources::Texture* m_emptyTexture;
		OvRendering::Data::StateMask m_previousStateMask;
		bool m_isDrawing;

	private:
		static std::atomic_bool s_isDrawing;
	};
}
