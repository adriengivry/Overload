/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvCore/SceneSystem/SceneManager.h>

#include "OvEditor/Panels/AView.h"

namespace OvEditor::Panels
{
	class GameView : public OvEditor::Panels::AView
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		GameView(
			const std::string& p_title,
			bool p_opened,
			const OvUI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Prepare the renderer for rendering
		*/
		virtual void InitFrame() override;

		/**
		* Returns the main camera used by the attached scene
		*/
		virtual OvRendering::Entities::Camera* GetCamera();

		/**
		* Returns the game view camera frustum or nothing if the game isn't playing
		*/
		std::optional<OvRendering::Data::Frustum> GetActiveFrustum();

	private:
		OvCore::SceneSystem::SceneManager& m_sceneManager;
	};
}