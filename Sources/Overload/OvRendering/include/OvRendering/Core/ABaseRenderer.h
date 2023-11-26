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
		* @param p_outputDesc
		*/
		virtual void BeginFrame(std::optional<Data::RenderOutputDesc> p_outputDesc);

		/**
		* End Frame
		*/
		virtual void EndFrame();

		/**
		* Returns the driver associated with this renderer
		*/
		virtual Context::Driver& GetDriver() const;

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

	protected:
		std::optional<std::reference_wrapper<const Buffers::Framebuffer>> m_target;
		Context::Driver& m_driver;
		OvRendering::Resources::Texture* m_emptyTexture;
		bool m_isDrawing;

	private:
		static std::atomic_bool s_isDrawing;
	};
}
