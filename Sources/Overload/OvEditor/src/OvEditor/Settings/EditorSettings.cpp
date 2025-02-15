/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Settings/EditorSettings.h"
#include <OvTools/Filesystem/IniFile.h>

template<class T>
void LoadIniEntry(OvTools::Filesystem::IniFile& iniFile, const std::string& entry, OvEditor::Settings::EditorSettings::Property<T>& out)
{
	if (iniFile.IsKeyExisting(entry))
	{
		out = iniFile.Get<T>(entry);
	}
}

void OvEditor::Settings::EditorSettings::Save()
{
	std::string editorSettingsPath = std::string(getenv("APPDATA")) + "\\OverloadTech\\OvEditor\\editor.ini";
	OvTools::Filesystem::IniFile iniFile(editorSettingsPath);
	iniFile.RemoveAll();
	iniFile.Add("show_geometry_bounds", ShowGeometryBounds.Get());
	iniFile.Add("show_light_bounds", ShowLightBounds.Get());
	iniFile.Add("editor_frustum_geometry_culling", EditorFrustumGeometryCulling.Get());
	iniFile.Add("editor_frustum_light_culling", EditorFrustumLightCulling.Get());
	iniFile.Add("light_billboard_scale", LightBillboardScale.Get());
	iniFile.Add("translation_snap_unit", TranslationSnapUnit.Get());
	iniFile.Add("rotation_snap_unit", RotationSnapUnit.Get());
	iniFile.Add("scaling_snap_unit", ScalingSnapUnit.Get());
	iniFile.Rewrite();
}

void OvEditor::Settings::EditorSettings::Load()
{
	std::string editorSettingsPath = std::string(getenv("APPDATA")) + "\\OverloadTech\\OvEditor\\editor.ini";
	OvTools::Filesystem::IniFile iniFile(editorSettingsPath);

	LoadIniEntry<bool>(iniFile, "show_geometry_bounds", ShowGeometryBounds);
	LoadIniEntry<bool>(iniFile, "show_light_bounds", ShowLightBounds);
	LoadIniEntry<bool>(iniFile, "show_geometry_frustum_culling_in_scene_view", EditorFrustumGeometryCulling);
	LoadIniEntry<bool>(iniFile, "show_light_frustum_culling_in_scene_view", EditorFrustumLightCulling);
	LoadIniEntry<float>(iniFile, "light_billboard_scale", LightBillboardScale);
	LoadIniEntry<float>(iniFile, "translation_snap_unit", TranslationSnapUnit);
	LoadIniEntry<float>(iniFile, "rotation_snap_unit", RotationSnapUnit);
	LoadIniEntry<float>(iniFile, "scaling_snap_unit", ScalingSnapUnit);
}
