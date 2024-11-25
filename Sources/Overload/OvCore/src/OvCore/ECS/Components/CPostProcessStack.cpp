/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvCore/ECS/Components/CPostProcessStack.h>
#include <OvUI/Widgets/Visual/Separator.h>
#include <OvUI/Widgets/Layout/Spacing.h>
#include <OvUI/Widgets/Selection/ComboBox.h>

OvCore::ECS::Components::CPostProcessStack::CPostProcessStack(ECS::Actor& p_owner) : AComponent(p_owner)
{
}

std::string OvCore::ECS::Components::CPostProcessStack::GetName()
{
	return "Post Process Stack";
}

void OvCore::ECS::Components::CPostProcessStack::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	auto& bloomSettings = m_settings.Get<Rendering::PostProcess::BloomEffect, Rendering::PostProcess::BloomSettings>();
	auto& fxaaSettings = m_settings.Get<Rendering::PostProcess::FXAAEffect, Rendering::PostProcess::FXAASettings>();
	auto& tonemappingSettings = m_settings.Get<Rendering::PostProcess::TonemappingEffect, Rendering::PostProcess::TonemappingSettings>();

	Helpers::Serializer::SerializeBoolean(p_doc, p_node, "bloom_enabled", bloomSettings.enabled);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "bloom_threshold", bloomSettings.threshold);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "bloom_radius", bloomSettings.radius);
	Helpers::Serializer::SerializeInt(p_doc, p_node, "bloom_kernel_size", bloomSettings.kernelSize);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "bloom_intensity", bloomSettings.intensity);

	Helpers::Serializer::SerializeBoolean(p_doc, p_node, "tonemapping_enabled", tonemappingSettings.enabled);
	Helpers::Serializer::SerializeFloat(p_doc, p_node, "tonemapping_exposure", tonemappingSettings.exposure);
	Helpers::Serializer::SerializeInt(p_doc, p_node, "tonemapping_mode", static_cast<int>(tonemappingSettings.mode));
	Helpers::Serializer::SerializeBoolean(p_doc, p_node, "tonemapping_gamma_correction", tonemappingSettings.gammaCorrection);

	Helpers::Serializer::SerializeBoolean(p_doc, p_node, "fxaa_enabled", fxaaSettings.enabled);
}

void OvCore::ECS::Components::CPostProcessStack::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	auto& bloomSettings = m_settings.Get<Rendering::PostProcess::BloomEffect, Rendering::PostProcess::BloomSettings>();
	auto& fxaaSettings = m_settings.Get<Rendering::PostProcess::FXAAEffect, Rendering::PostProcess::FXAASettings>();
	auto& tonemappingSettings = m_settings.Get<Rendering::PostProcess::TonemappingEffect, Rendering::PostProcess::TonemappingSettings>();

	Helpers::Serializer::DeserializeBoolean(p_doc, p_node, "bloom_enabled", bloomSettings.enabled);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "bloom_threshold", bloomSettings.threshold);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "bloom_radius", bloomSettings.radius);
	Helpers::Serializer::DeserializeInt(p_doc, p_node, "bloom_kernel_size", bloomSettings.kernelSize);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "bloom_intensity", bloomSettings.intensity);

	Helpers::Serializer::DeserializeBoolean(p_doc, p_node, "tonemapping_enabled", tonemappingSettings.enabled);
	Helpers::Serializer::DeserializeFloat(p_doc, p_node, "tonemapping_exposure", tonemappingSettings.exposure);
	Helpers::Serializer::DeserializeInt(p_doc, p_node, "tonemapping_mode", reinterpret_cast<int&>(tonemappingSettings.mode));
	Helpers::Serializer::DeserializeBoolean(p_doc, p_node, "tonemapping_gamma_correction", tonemappingSettings.gammaCorrection);

	Helpers::Serializer::DeserializeBoolean(p_doc, p_node, "fxaa_enabled", fxaaSettings.enabled);
}

void OvCore::ECS::Components::CPostProcessStack::OnInspector(OvUI::Internal::WidgetContainer& p_root)
{
	auto& bloomSettings = m_settings.Get<Rendering::PostProcess::BloomEffect, Rendering::PostProcess::BloomSettings>();
	auto& fxaaSettings = m_settings.Get<Rendering::PostProcess::FXAAEffect, Rendering::PostProcess::FXAASettings>();
	auto& tonemappingSettings = m_settings.Get<Rendering::PostProcess::TonemappingEffect, Rendering::PostProcess::TonemappingSettings>();

	OvCore::Helpers::GUIDrawer::DrawBoolean(p_root, "Bloom Enabled", bloomSettings.enabled);
	OvCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Bloom Threshold", bloomSettings.threshold, 0.05f, 0.0f);
	OvCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Bloom Radius", bloomSettings.radius, 0.1f, 0.0f);
	OvCore::Helpers::GUIDrawer::DrawScalar<int>(p_root, "Bloom Kernel Size", bloomSettings.kernelSize, 1.0f, 1, 10);
	OvCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Bloom Intensity", bloomSettings.intensity, 0.05f, 0.0f);
	OvCore::Helpers::GUIDrawer::DrawScalar<int>(p_root, "Bloom Passes", bloomSettings.passes, 1, 1, 10);
	
	p_root.CreateWidget<OvUI::Widgets::Visual::Separator>();
	p_root.CreateWidget<OvUI::Widgets::Layout::Spacing>();

	OvCore::Helpers::GUIDrawer::DrawBoolean(p_root, "Tonemapping Enabled", tonemappingSettings.enabled);
	OvCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Tonemapping Exposure", tonemappingSettings.exposure, 0.05f, 0.0f);
	Helpers::GUIDrawer::CreateTitle(p_root, "Tonemapping Mode");
	auto& tonemappingMode = p_root.CreateWidget<OvUI::Widgets::Selection::ComboBox>(static_cast<int>(tonemappingSettings.mode));
	tonemappingMode.choices.emplace(0, "Neutral");
	tonemappingMode.choices.emplace(1, "Reinhard");
	tonemappingMode.choices.emplace(2, "Reinhard-Jodie");
	tonemappingMode.choices.emplace(3, "Uncharted 2");
	tonemappingMode.choices.emplace(4, "Uncharted 2 (Filmic)");
	tonemappingMode.choices.emplace(5, "ACES");
	tonemappingMode.ValueChangedEvent += [this](int p_choice)
	{
		auto& tonemappingSettings = m_settings.Get<Rendering::PostProcess::TonemappingEffect, Rendering::PostProcess::TonemappingSettings>();
		tonemappingSettings.mode = static_cast<OvCore::Rendering::PostProcess::ETonemappingMode>(p_choice);
	};
	OvCore::Helpers::GUIDrawer::DrawBoolean(p_root, "Tonemapping Gamma Correction", tonemappingSettings.gammaCorrection);

	p_root.CreateWidget<OvUI::Widgets::Visual::Separator>();
	p_root.CreateWidget<OvUI::Widgets::Layout::Spacing>();

	OvCore::Helpers::GUIDrawer::DrawBoolean(p_root, "FXAA Enabled", fxaaSettings.enabled);
}

const OvCore::Rendering::PostProcess::PostProcessStack& OvCore::ECS::Components::CPostProcessStack::GetStack()
{
	return m_settings;
}

const OvCore::Rendering::PostProcess::BloomSettings& OvCore::ECS::Components::CPostProcessStack::GetBloomSettings() const
{
	return m_settings.Get<Rendering::PostProcess::BloomEffect, Rendering::PostProcess::BloomSettings>();
}

const OvCore::Rendering::PostProcess::TonemappingSettings& OvCore::ECS::Components::CPostProcessStack::GetTonemappingSettings() const
{
	return m_settings.Get<Rendering::PostProcess::TonemappingEffect, Rendering::PostProcess::TonemappingSettings>();
}

const OvCore::Rendering::PostProcess::FXAASettings& OvCore::ECS::Components::CPostProcessStack::GetFXAASettings() const
{
	return m_settings.Get<Rendering::PostProcess::FXAAEffect, Rendering::PostProcess::FXAASettings>();
}

void OvCore::ECS::Components::CPostProcessStack::SetBloomSettings(const OvCore::Rendering::PostProcess::BloomSettings& p_settings)
{
	m_settings.Set<Rendering::PostProcess::BloomEffect>(p_settings);
}

void OvCore::ECS::Components::CPostProcessStack::SetTonemappingSettings(const OvCore::Rendering::PostProcess::TonemappingSettings& p_settings)
{
	m_settings.Set<Rendering::PostProcess::TonemappingEffect>(p_settings);
}

void OvCore::ECS::Components::CPostProcessStack::SetFXAASettings(const OvCore::Rendering::PostProcess::FXAASettings& p_settings)
{
	m_settings.Set<Rendering::PostProcess::FXAAEffect>(p_settings);
}
