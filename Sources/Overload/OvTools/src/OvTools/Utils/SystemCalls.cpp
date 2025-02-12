/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvTools/Utils/PathParser.h"
#include "OvTools/Utils/SystemCalls.h"

#include <Windows.h>
#include <ShlObj.h>

void OvTools::Utils::SystemCalls::ShowInExplorer(const std::string & p_path)
{
	ShellExecuteA(NULL, "open", OvTools::Utils::PathParser::MakeWindowsStyle(p_path).c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void OvTools::Utils::SystemCalls::OpenFile(const std::string & p_file, const std::string & p_workingDir)
{
	ShellExecuteA(NULL, "open", OvTools::Utils::PathParser::MakeWindowsStyle(p_file).c_str(), NULL,
		p_workingDir.empty() ? NULL : OvTools::Utils::PathParser::MakeWindowsStyle(p_workingDir).c_str(),
		SW_SHOWNORMAL);
}

void OvTools::Utils::SystemCalls::EditFile(const std::string & p_file)
{
	ShellExecuteW(NULL, NULL, std::wstring(p_file.begin(), p_file.end()).c_str(), NULL, NULL, SW_NORMAL);
}

void OvTools::Utils::SystemCalls::OpenURL(const std::string& p_url)
{
	ShellExecute(0, 0, p_url.c_str(), 0, 0, SW_SHOW);
}

std::string OvTools::Utils::SystemCalls::GetPathToAppdata()
{
	PWSTR path = nullptr;
	std::string result;

	HRESULT getPath = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &path);
	
	// We get a wide string back which we need to convert back to a utf-8 string
	if (SUCCEEDED(getPath)) {
		// If size == 0, we have errors and are not able to convert the wide string
		int size = WideCharToMultiByte(CP_UTF8, 0, path, -1, nullptr, 0, nullptr, nullptr);
		if (size > 0) {
			std::string converted(size - 1, 0);
			WideCharToMultiByte(CP_UTF8, 0, path, -1, &converted[0], size, nullptr, nullptr);
			result = converted;
		}
		CoTaskMemFree(path);
	}
	
	return result;
}