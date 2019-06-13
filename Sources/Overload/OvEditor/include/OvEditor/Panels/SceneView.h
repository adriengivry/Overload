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
		* Update the scene view (Inputs logic)
		* @param p_deltaTime
		*/
		virtual void Update(float p_deltaTime) override;

		/**
		* Custom implementation of the render method
		*/
		virtual void _Render_Impl() override;

	private:
		OvCore::SceneSystem::SceneManager& m_sceneManager;
	};
}