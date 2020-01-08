/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvRendering/Core/Renderer.h>
#include <OvWindowing/Context/Device.h>
#include <OvWindowing/Window.h>
#include <OvRendering/Context/Driver.h>
#include <OvUI/Core/UIManager.h>
#include <OvUI/Panels/PanelWindow.h>

namespace OvEditor::Core
{
	/**
	* A simple panel that allow the user to select the project to launch
	*/
	class ProjectHub
	{
	public:
		/**
		* Constructor
		*/
		ProjectHub();

		/**
		* Run the project hub logic
		*/
		std::tuple<bool, std::string, std::string> Run();

		/**
		* Setup the project hub specific context (minimalist context)
		*/
		void SetupContext();

		/**
		* Register the project (identified from the given path) into the project hub
		* @param p_path
		*/
		void RegisterProject(const std::string& p_path);

	private:
		std::unique_ptr<OvWindowing::Context::Device>		m_device;
		std::unique_ptr<OvWindowing::Window>				m_window;
		std::unique_ptr<OvRendering::Context::Driver>		m_driver;
		std::unique_ptr<OvRendering::Core::Renderer>		m_renderer;
		std::unique_ptr<OvUI::Core::UIManager>				m_uiManager;

		OvUI::Modules::Canvas m_canvas;
		std::unique_ptr<OvUI::Panels::PanelWindow>			m_mainPanel;

		std::string m_projectPath = "";
		std::string m_projectName = "";
		bool m_readyToGo = false;
	};
}