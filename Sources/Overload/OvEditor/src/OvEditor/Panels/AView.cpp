/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include <GL/glew.h>

#include "OvEditor/Panels/AView.h"
#include "OvEditor/Core/EditorActions.h"

OvEditor::Panels::AView::AView
(
	const std::string& p_title,
	bool p_opened,
	const OvUI::Settings::PanelWindowSettings& p_windowSettings
) : PanelWindow(p_title, p_opened, p_windowSettings), m_editorRenderer(EDITOR_RENDERER())
{
	glGenFramebuffers(1, &m_fbo);
	glGenTextures(1, &m_renderTexture);
	glGenRenderbuffers(1, &m_depthStencilBuffer);

	m_cameraPosition = { -10.0f, 4.0f, 10.0f };
	m_camera.SetPitch(-10.0f);
	m_camera.SetYaw(-45.f);

	m_image = &CreateWidget<OvUI::Widgets::Visual::Image>(m_renderTexture, OvMaths::FVector2{ 0.f, 0.f });
}

OvEditor::Panels::AView::~AView()
{
	glDeleteBuffers(1, &m_fbo);
	glDeleteTextures(1, &m_renderTexture);
	glGenRenderbuffers(1, &m_depthStencilBuffer);
}

void OvEditor::Panels::AView::Update(float p_deltaTime)
{
	Bind();

	auto[winWidth, winHeight] = GetSafeSize();

	m_image->size = OvMaths::FVector2(static_cast<float>(winWidth), static_cast<float>(winHeight));

	/* Setup texture */
	glBindTexture(GL_TEXTURE_2D, m_renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, winWidth, winHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	/* Setup depth-stencil buffer (24 + 8 bits) */
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthStencilBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, winWidth, winHeight);

	/* Setup frame buffer */
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthStencilBuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderTexture, 0);

	Unbind();
}

void OvEditor::Panels::AView::Bind()
{
	auto[winWidth, winHeight] = GetSafeSize();

	glViewport(0, 0, winWidth, winHeight);

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void OvEditor::Panels::AView::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OvEditor::Panels::AView::_Draw_Impl()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

	OvUI::Panels::PanelWindow::_Draw_Impl();

	ImGui::PopStyleVar();
}

void OvEditor::Panels::AView::Render()
{
	FillEngineUBO();

	auto [winWidth, winHeight] = GetSafeSize();
	auto projection = m_camera.GetProjectionMatrix(winWidth, winHeight);
	auto view = m_camera.GetViewMatrix(m_cameraPosition);
	EDITOR_CONTEXT(shapeDrawer)->SetViewProjection(projection * view);

	Bind();
	_Render_Impl();
	Unbind();
}

void OvEditor::Panels::AView::SetCameraPosition(const OvMaths::FVector3 & p_position)
{
	m_cameraPosition = p_position;
}

const OvMaths::FVector3 & OvEditor::Panels::AView::GetCameraPosition() const
{
	return m_cameraPosition;
}

OvRendering::LowRenderer::Camera & OvEditor::Panels::AView::GetCamera()
{
	return m_camera;
}

std::pair<uint16_t, uint16_t> OvEditor::Panels::AView::GetSafeSize() const
{
	auto result = GetSize() - OvMaths::FVector2{ 0.f, 25.f }; // 25 == title bar height
	return { static_cast<uint16_t>(result.x), static_cast<uint16_t>(result.y) };
}

const OvMaths::FVector3& OvEditor::Panels::AView::GetGridColor() const
{
	return m_gridColor;
}

void OvEditor::Panels::AView::SetGridColor(const OvMaths::FVector3& p_color)
{
	m_gridColor = p_color;
}

void OvEditor::Panels::AView::FillEngineUBO()
{
	auto& engineUBO = *EDITOR_CONTEXT(engineUBO);

	auto[winWidth, winHeight] = GetSafeSize();

	size_t offset = sizeof(OvMaths::FMatrix4); // We skip the model matrix (Which is a special case, modified every draw calls)
	engineUBO.SetSubData(OvMaths::FMatrix4::Transpose(m_camera.GetViewMatrix(m_cameraPosition)), std::ref(offset));
	engineUBO.SetSubData(OvMaths::FMatrix4::Transpose(m_camera.GetProjectionMatrix(winWidth, winHeight)), std::ref(offset));
	engineUBO.SetSubData(m_cameraPosition, std::ref(offset));
}

