/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once


#include <OvUI/Panels/PanelWindow.h>
#include <OvUI/Widgets/Visual/Image.h>
#include <OvRendering/Buffers/UniformBuffer.h>
#include <OvRendering/Buffers/Framebuffer.h>
#include <OvRendering/LowRenderer/Camera.h>

namespace OvEditor::Core { class EditorRenderer; }

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
		AView
		(
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
		* Defines the camera position
		* @param p_position
		*/
		void SetCameraPosition(const OvMaths::FVector3& p_position);

		/**
		* Returns the camera position
		*/
		const OvMaths::FVector3& GetCameraPosition() const;

		/**
		* Returns the camera used by this view
		*/
		OvRendering::LowRenderer::Camera& GetCamera();

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
		void FillEngineUBO();

	protected:
		OvEditor::Core::EditorRenderer& m_editorRenderer;
		OvRendering::LowRenderer::Camera m_camera;
		OvMaths::FVector3 m_cameraPosition;
		OvUI::Widgets::Visual::Image* m_image;

		OvMaths::FVector3 m_gridColor = OvMaths::FVector3::One;

		OvRendering::Buffers::Framebuffer m_fbo;
	};
}