/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvTools/Utils/PathParser.h>

#include <OvCore/Helpers/GUIDrawer.h>
#include <OvCore/Global/ServiceLocator.h>
#include <OvCore/ResourceManagement/ModelManager.h>
#include <OvCore/ResourceManagement/TextureManager.h>

#include <OvUI/Widgets/Visual/Separator.h>
#include <OvUI/Widgets/Layout/Group.h>
#include <OvUI/Widgets/Buttons/Button.h>
#include <OvUI/Widgets/Selection/ComboBox.h>

#include "OvEditor/Panels/AssetMetadataEditor.h"
#include "OvEditor/Panels/AssetView.h"
#include "OvEditor/Core/EditorActions.h"

OvEditor::Panels::AssetMetadataEditor::AssetMetadataEditor
(
    const std::string& p_title,
    bool p_opened,
    const OvUI::Settings::PanelWindowSettings& p_windowSettings
) :
    PanelWindow(p_title, p_opened, p_windowSettings)
{
    CreateHeaderButtons();
    CreateWidget<OvUI::Widgets::Visual::Separator>();
    m_settings = &CreateWidget<OvUI::Widgets::Layout::Columns<2>>();
    m_settings->widths[0] = 150;
}

void OvEditor::Panels::AssetMetadataEditor::SetTarget(const std::string& p_path)
{
    m_resource = p_path;
    m_metadata.reset(new OvTools::Filesystem::IniFile(m_resource + ".meta"));
    CreateSettings();
}

void OvEditor::Panels::AssetMetadataEditor::Preview()
{
    auto& assetView = EDITOR_PANEL(OvEditor::Panels::AssetView, "Asset View");

    auto resourcePath = EDITOR_EXEC(GetResourcePath(m_resource));
    auto fileType = OvTools::Utils::PathParser::GetFileType(m_resource);

    if (fileType == OvTools::Utils::PathParser::EFileType::MODEL)
    {
        if (auto resource = OVSERVICE(OvCore::ResourceManagement::ModelManager).GetResource(resourcePath))
        {
            assetView.SetResource(resource);
        }
    }
    else if (fileType == OvTools::Utils::PathParser::EFileType::TEXTURE)
    {
        if (auto resource = OVSERVICE(OvCore::ResourceManagement::TextureManager).GetResource(resourcePath))
        {
            assetView.SetResource(resource);
        }
    }

    assetView.Open();
}

void OvEditor::Panels::AssetMetadataEditor::CreateHeaderButtons()
{
    auto& applyButton = CreateWidget<OvUI::Widgets::Buttons::Button>("Apply");
    applyButton.idleBackgroundColor = { 0.0f, 0.5f, 0.0f };
    applyButton.ClickedEvent += [this]
    {
        Apply();
    };

    applyButton.lineBreak = false;

    auto& reloadButton = CreateWidget<OvUI::Widgets::Buttons::Button>("Revert");
    reloadButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };
    reloadButton.ClickedEvent += [this]
    {
        SetTarget(m_resource);
    };

    reloadButton.lineBreak = false;

    auto& previewButton = CreateWidget<OvUI::Widgets::Buttons::Button>("Preview");
    previewButton.idleBackgroundColor = { 0.7f, 0.5f, 0.0f };
    previewButton.ClickedEvent += std::bind(&AssetMetadataEditor::Preview, this);
    previewButton.lineBreak = false;

    auto& defaultButton = CreateWidget<OvUI::Widgets::Buttons::Button>("Reset to default");
    defaultButton.idleBackgroundColor = { 0.5f, 0.0f, 0.0f };
    defaultButton.ClickedEvent += [this]
    {
        m_metadata->RemoveAll();
        CreateSettings();
    };
}

void OvEditor::Panels::AssetMetadataEditor::CreateSettings()
{
    m_settings->RemoveAllWidgets();

    auto fileType = OvTools::Utils::PathParser::GetFileType(m_resource);
    if (fileType == OvTools::Utils::PathParser::EFileType::MODEL)
    {
        CreateModelSettings();
    }
    else if (fileType == OvTools::Utils::PathParser::EFileType::TEXTURE)
    {
        CreateTextureSettings();
    }
}

#define MODEL_FLAG_ENTRY(setting) OvCore::Helpers::GUIDrawer::DrawBoolean(*m_settings, setting, [&]() { return m_metadata->Get<bool>(setting); }, [&](bool value) { m_metadata->Set<bool>(setting, value); })

void OvEditor::Panels::AssetMetadataEditor::CreateModelSettings()
{
    m_metadata->Add("CALC_TANGENT_SPACE", true);
    m_metadata->Add("JOIN_IDENTICAL_VERTICES", true);
    m_metadata->Add("MAKE_LEFT_HANDED", false);
    m_metadata->Add("TRIANGULATE", true);
    m_metadata->Add("REMOVE_COMPONENT", false);
    m_metadata->Add("GEN_NORMALS", false);
    m_metadata->Add("GEN_SMOOTH_NORMALS", true);
    m_metadata->Add("SPLIT_LARGE_MESHES", false);
    m_metadata->Add("PRE_TRANSFORM_VERTICES", true);
    m_metadata->Add("LIMIT_BONE_WEIGHTS", false);
    m_metadata->Add("VALIDATE_DATA_STRUCTURE", false);
    m_metadata->Add("IMPROVE_CACHE_LOCALITY", true);
    m_metadata->Add("REMOVE_REDUNDANT_MATERIALS", false);
    m_metadata->Add("FIX_INFACING_NORMALS", false);
    m_metadata->Add("SORT_BY_PTYPE", false);
    m_metadata->Add("FIND_DEGENERATES", false);
    m_metadata->Add("FIND_INVALID_DATA", true);
    m_metadata->Add("GEN_UV_COORDS", true);
    m_metadata->Add("TRANSFORM_UV_COORDS", false);
    m_metadata->Add("FIND_INSTANCES", true);
    m_metadata->Add("OPTIMIZE_MESHES", true);
    m_metadata->Add("OPTIMIZE_GRAPH", true);
    m_metadata->Add("FLIP_UVS", false);
    m_metadata->Add("FLIP_WINDING_ORDER", false);
    m_metadata->Add("SPLIT_BY_BONE_COUNT", false);
    m_metadata->Add("DEBONE", true);

    MODEL_FLAG_ENTRY("CALC_TANGENT_SPACE");
    MODEL_FLAG_ENTRY("JOIN_IDENTICAL_VERTICES");
    MODEL_FLAG_ENTRY("MAKE_LEFT_HANDED");
    MODEL_FLAG_ENTRY("TRIANGULATE");
    MODEL_FLAG_ENTRY("REMOVE_COMPONENT");
    MODEL_FLAG_ENTRY("GEN_NORMALS");
    MODEL_FLAG_ENTRY("GEN_SMOOTH_NORMALS");
    MODEL_FLAG_ENTRY("SPLIT_LARGE_MESHES");
    MODEL_FLAG_ENTRY("PRE_TRANSFORM_VERTICES");
    MODEL_FLAG_ENTRY("LIMIT_BONE_WEIGHTS");
    MODEL_FLAG_ENTRY("VALIDATE_DATA_STRUCTURE");
    MODEL_FLAG_ENTRY("IMPROVE_CACHE_LOCALITY");
    MODEL_FLAG_ENTRY("REMOVE_REDUNDANT_MATERIALS");
    MODEL_FLAG_ENTRY("FIX_INFACING_NORMALS");
    MODEL_FLAG_ENTRY("SORT_BY_PTYPE");
    MODEL_FLAG_ENTRY("FIND_DEGENERATES");
    MODEL_FLAG_ENTRY("FIND_INVALID_DATA");
    MODEL_FLAG_ENTRY("GEN_UV_COORDS");
    MODEL_FLAG_ENTRY("TRANSFORM_UV_COORDS");
    MODEL_FLAG_ENTRY("FIND_INSTANCES");
    MODEL_FLAG_ENTRY("OPTIMIZE_MESHES");
    MODEL_FLAG_ENTRY("OPTIMIZE_GRAPH");
    MODEL_FLAG_ENTRY("FLIP_UVS");
    MODEL_FLAG_ENTRY("FLIP_WINDING_ORDER");
    MODEL_FLAG_ENTRY("SPLIT_BY_BONE_COUNT");
    MODEL_FLAG_ENTRY("DEBONE");
};

void OvEditor::Panels::AssetMetadataEditor::CreateTextureSettings()
{
    m_metadata->Add("MIN_FILTER", static_cast<int>(OvRendering::Settings::ETextureFilteringMode::LINEAR_MIPMAP_LINEAR));
    m_metadata->Add("MAG_FILTER", static_cast<int>(OvRendering::Settings::ETextureFilteringMode::LINEAR));
    m_metadata->Add("ENABLE_MIPMAPPING", true);

    std::map<int, std::string> filteringModes;
    filteringModes.emplace(0x2600, "NEAREST");
    filteringModes.emplace(0x2601, "LINEAR");
    filteringModes.emplace(0x2700, "NEAREST_MIPMAP_NEAREST");
    filteringModes.emplace(0x2703, "LINEAR_MIPMAP_LINEAR");
    filteringModes.emplace(0x2701, "LINEAR_MIPMAP_NEAREST");
    filteringModes.emplace(0x2702, "NEAREST_MIPMAP_LINEAR");

    OvCore::Helpers::GUIDrawer::CreateTitle(*m_settings, "MIN_FILTER");
    auto& minFilter = m_settings->CreateWidget<OvUI::Widgets::Selection::ComboBox>(m_metadata->Get<int>("MIN_FILTER"));
    minFilter.choices = filteringModes;
    minFilter.ValueChangedEvent += [this](int p_choice)
    {
        m_metadata->Set("MIN_FILTER", p_choice);
    };

    OvCore::Helpers::GUIDrawer::CreateTitle(*m_settings, "MAG_FILTER");
    auto& magFilter = m_settings->CreateWidget<OvUI::Widgets::Selection::ComboBox>(m_metadata->Get<int>("MAG_FILTER"));
    magFilter.choices = filteringModes;
    magFilter.ValueChangedEvent += [this](int p_choice)
    {
        m_metadata->Set("MAG_FILTER", p_choice);
    };

    OvCore::Helpers::GUIDrawer::DrawBoolean(*m_settings, "ENABLE_MIPMAPPING", [&]() { return m_metadata->Get<bool>("ENABLE_MIPMAPPING"); }, [&](bool value) { m_metadata->Set<bool>("ENABLE_MIPMAPPING", value); });
}

void OvEditor::Panels::AssetMetadataEditor::Apply()
{
    m_metadata->Rewrite();

    auto resourcePath = EDITOR_EXEC(GetResourcePath(m_resource));
    auto fileType = OvTools::Utils::PathParser::GetFileType(m_resource);

    if (fileType == OvTools::Utils::PathParser::EFileType::MODEL)
    {
        auto& modelManager = OVSERVICE(OvCore::ResourceManagement::ModelManager);
        if (modelManager.IsResourceRegistered(resourcePath))
        {
            modelManager.AResourceManager::ReloadResource(resourcePath);
        }
    }
    else if (fileType == OvTools::Utils::PathParser::EFileType::TEXTURE)
    {
        auto& textureManager = OVSERVICE(OvCore::ResourceManagement::TextureManager);
        if (textureManager.IsResourceRegistered(resourcePath))
        {
            textureManager.AResourceManager::ReloadResource(resourcePath);
        }
    }
}
