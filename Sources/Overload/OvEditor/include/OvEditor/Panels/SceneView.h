/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include "OvEditor/Panels/AViewControllable.h"

namespace OvEditor::Panels
{
	class SceneView : public OvEditor::Panels::AViewControllable
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		SceneView
		(
			const std::string& p_title,
			bool p_opened,
			const OvUI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Custom implementation of the render method
		*/
		virtual void _Render_Impl() override;

		/**
		* Render the actual scene
		* @param p_defaultRenderState
		*/
		void RenderScene(uint8_t p_defaultRenderState);

		/**
		* Render the scene for actor picking and handle the logic behind it
		*/
		void HandleActorPicking();

	private:
		OvCore::SceneSystem::SceneManager& m_sceneManager;
		OvRendering::Buffers::Framebuffer m_actorPickingFramebuffer;
	};
}