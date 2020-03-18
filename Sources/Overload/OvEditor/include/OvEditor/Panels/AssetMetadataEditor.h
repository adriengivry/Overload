/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <variant>

#include <OvTools/Filesystem/IniFile.h>

#include <OvUI/Widgets/Texts/Text.h>
#include <OvUI/Panels/PanelWindow.h>
#include <OvUI/Widgets/Layout/Group.h>
#include <OvUI/Widgets/Layout/Columns.h>

#include <OvRendering/Resources/Model.h>
#include <OvRendering/Resources/Texture.h>

namespace OvEditor::Panels
{
    class AssetMetadataEditor : public OvUI::Panels::PanelWindow
    {
    public:
        using EditableAssets = std::variant<OvRendering::Resources::Model*, OvRendering::Resources::Texture*>;

        /**
        * Constructor
        * @param p_title
        * @param p_opened
        * @param p_windowSettings
        */
        AssetMetadataEditor
        (
            const std::string& p_title,
            bool p_opened,
            const OvUI::Settings::PanelWindowSettings& p_windowSettings
        );

        /**
        * Defines the target of the asset settings editor
        * @param p_path
        */
        void SetTarget(const std::string& p_path);

        /**
        * Launch the preview of the target asset
        */
        void Preview();

    private:
        void CreateHeaderButtons();
        void CreateSettings();
        void CreateModelSettings();
        void CreateTextureSettings();
        void Apply();

    private:
        std::string m_resource;

        OvUI::Widgets::Layout::Columns<2>* m_settings = nullptr;
        std::unique_ptr<OvTools::Filesystem::IniFile> m_metadata;
    };
}