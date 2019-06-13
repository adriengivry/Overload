/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <OvCore/SceneSystem/SceneManager.h>

#include "OvEditor/Panels/AView.h"

namespace OvEditor::Core { class EditorRenderer; }

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
		GameView
		(
			const std::string& p_title,
			bool p_opened,
			const OvUI::Settings::PanelWindowSettings& p_windowSettings
		);

		/**
		* Update the view
		* @param p_deltaTime
		*/
		virtual void Update(float p_deltaTime) override;

		/**
		* Custom implementation of the render method
		*/
		virtual void _Render_Impl() override;

	private:
		OvCore::SceneSystem::SceneManager& m_sceneManager;
		bool m_hasCamera = false;
	};
}