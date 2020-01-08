/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include "OvEditor/Core/Context.h"
#include "OvEditor/Core/Editor.h"

namespace OvEditor::Core
{
	/**
	* Entry point of OvEditor
	*/
	class Application
	{
	public:
		/**
		* Constructor
		* @param p_projectPath
		* @param p_projectName
		*/
		Application(const std::string& p_projectPath, const std::string& p_projectName);

		/**
		* Destructor
		*/
		~Application();

		/**
		* Run the app
		*/
		void Run();

		/**
		* Returns true if the app is running
		*/
		bool IsRunning() const;

	private:
		Context m_context;
		Editor m_editor;
	};
}