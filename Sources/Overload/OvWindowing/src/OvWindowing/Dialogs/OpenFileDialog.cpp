/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvWindowing/Dialogs/OpenFileDialog.h"

#include <Windows.h>

OvWindowing::Dialogs::OpenFileDialog::OpenFileDialog(const std::string & p_dialogTitle) : FileDialog(GetOpenFileNameA, p_dialogTitle)
{
}

void OvWindowing::Dialogs::OpenFileDialog::AddFileType(const std::string & p_label, const std::string & p_filter)
{
	m_filter += p_label + '\0' + p_filter + '\0';
}
