/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include "OvWindowing/Dialogs/FileDialog.h"

namespace OvWindowing::Dialogs
{
	/**
	* Dialog window that asks the user to select a file from the disk
	*/
	class API_OVWINDOWING OpenFileDialog : public FileDialog
	{
	public:
		/**
		* Constructor
		* @param p_dialogTitle
		*/
		OpenFileDialog(const std::string& p_dialogTitle);

		/**
		* Add a supported file type to the dialog window
		* @param p_label
		* @param p_filter
		*/
		void AddFileType(const std::string& p_label, const std::string& p_filter);
	};
}