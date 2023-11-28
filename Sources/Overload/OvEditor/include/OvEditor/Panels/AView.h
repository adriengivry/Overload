/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once


#include <OvUI/Panels/PanelWindow.h>
#include <OvUI/Widgets/Visual/Image.h>
#include <OvRendering/Buffers/UniformBuffer.h>
#include <OvRendering/Buffers/Framebuffer.h>
#include <OvRendering/Entities/Camera.h>
#include <OvRendering/Core/IRenderer.h>

namespace OvEditor::Panels
{
	/**
	* Base class for any view
	*/
	class AView : public OvUI::Panels::PanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		AView(
			const std::string& p_title,
			bool p_opened,
			const OvUI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Update the view
		* @param p_deltaTime
		*/
		virtual void Update(float p_deltaTime);

		/**
		* Custom implementation of the draw method
		*/
		void _Draw_Impl() override;

		/**
		* Custom implementation of the render method to define in dervied classes
		*/
		virtual void _Render_Impl() = 0;

		/**
		* Render the view
		*/
		void Render();

		/**
		* Returns the camera used by this view
		*/
		OvRendering::Entities::Camera& GetCamera();

		/**
		* Returns the size of the panel ignoring its titlebar height
		*/
		std::pair<uint16_t, uint16_t> GetSafeSize() const;

		/**
		* Returns the grid color of the view
		*/
		const OvMaths::FVector3& GetGridColor() const;

		/**
		* Defines the grid color of the view
		* @param p_color
		*/
		void SetGridColor(const OvMaths::FVector3& p_color);

		/**
		* Fill the UBO using the view settings
		*/
		// void FillEngineUBO();

	protected:
		/**
		* Update camera matrices
		*/
		// void PrepareCamera();

		template<class T>
		T& GetRendererAs()
		{
			OVASSERT(m_renderer != nullptr, "Cannot retrieve the renderer! Make sure to create one first.");
			return *static_cast<T*>(m_renderer.get());
		}

	protected:
		OvRendering::Entities::Camera m_camera;
		OvUI::Widgets::Visual::Image* m_image;

        OvMaths::FVector3 m_gridColor = OvMaths::FVector3 { 0.176f, 0.176f, 0.176f };

		OvRendering::Buffers::Framebuffer m_fbo;
		std::unique_ptr<OvRendering::Core::IRenderer> m_renderer;
	};
}