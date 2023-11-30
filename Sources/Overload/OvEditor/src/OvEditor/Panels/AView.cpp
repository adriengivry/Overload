/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <GL/glew.h>

#include "OvEditor/Panels/AView.h"
#include "OvEditor/Core/EditorActions.h"

OvEditor::Panels::AView::AView
(
	const std::string& p_title,
	bool p_opened,
	const OvUI::Settings::PanelWindowSettings& p_windowSettings
) : PanelWindow(p_title, p_opened, p_windowSettings)
{
	m_image = &CreateWidget<OvUI::Widgets::Visual::Image>(m_fbo.GetTextureID(), OvMaths::FVector2{ 0.f, 0.f });
	scrollable = false;
}

void OvEditor::Panels::AView::Update(float p_deltaTime)
{
	auto[winWidth, winHeight] = GetSafeSize();
	m_image->size = OvMaths::FVector2(static_cast<float>(winWidth), static_cast<float>(winHeight));
	m_fbo.Resize(winWidth, winHeight);
}

void OvEditor::Panels::AView::_Draw_Impl()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	OvUI::Panels::PanelWindow::_Draw_Impl();
	ImGui::PopStyleVar();
}

void OvEditor::Panels::AView::InitFrame()
{
	// Views should probably override this method
	// and add appropriate descriptors for their
	// respective renderers.
}

void OvEditor::Panels::AView::Render()
{
	auto [winWidth, winHeight] = GetSafeSize();
	auto camera = GetCamera();
	auto scene = GetScene();

	if (winWidth > 0 && winHeight > 0 && camera && scene)
	{
		InitFrame();

		m_renderer->AddDescriptor<OvCore::Rendering::SceneRenderer::SceneDescriptor>({
			*scene,
			*camera
		});

		OvRendering::Data::FrameDescriptor frameDescriptor;
		frameDescriptor.clearColor = camera ? camera->GetClearColor() : OvMaths::FVector3::Zero;
		frameDescriptor.renderWidth = winWidth;
		frameDescriptor.renderHeight = winHeight;
		frameDescriptor.outputBuffer = &m_fbo;

		m_renderer->BeginFrame(frameDescriptor);
		DrawFrame();
		m_renderer->EndFrame();
	}
}

void OvEditor::Panels::AView::DrawFrame()
{
	m_renderer->Draw();
}

std::pair<uint16_t, uint16_t> OvEditor::Panels::AView::GetSafeSize() const
{
	auto result = GetSize() - OvMaths::FVector2{ 0.f, 25.f }; // 25 == title bar height
	return { static_cast<uint16_t>(result.x), static_cast<uint16_t>(result.y) };
}

/*
void OvEditor::Panels::AView::FillEngineUBO()
{
	auto& engineUBO = *EDITOR_CONTEXT(engineUBO);

	auto[winWidth, winHeight] = GetSafeSize();

	size_t offset = sizeof(OvMaths::FMatrix4); // We skip the model matrix (Which is a special case, modified every draw calls)
	engineUBO.SetSubData(OvMaths::FMatrix4::Transpose(m_camera.GetViewMatrix()), std::ref(offset));
	engineUBO.SetSubData(OvMaths::FMatrix4::Transpose(m_camera.GetProjectionMatrix()), std::ref(offset));
	engineUBO.SetSubData(m_cameraPosition, std::ref(offset));
}

void OvEditor::Panels::AView::PrepareCamera()
{
	auto [winWidth, winHeight] = GetSafeSize();
	m_camera.CacheMatrices(winWidth, winHeight, m_cameraPosition, m_cameraRotation);
}
*/