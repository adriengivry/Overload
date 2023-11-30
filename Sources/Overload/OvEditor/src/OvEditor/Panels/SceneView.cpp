/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <OvUI/Plugins/DDTarget.h>

#include <OvCore/Rendering/SceneRenderer.h>

#include "OvEditor/Core/EditorActions.h"
#include "OvEditor/Panels/SceneView.h"
#include "OvEditor/Panels/GameView.h"
#include "OvEditor/Settings/EditorSettings.h"

OvEditor::Panels::SceneView::SceneView
(
	const std::string& p_title,
	bool p_opened,
	const OvUI::Settings::PanelWindowSettings& p_windowSettings
) : AViewControllable(p_title, p_opened, p_windowSettings, true),
	m_sceneManager(EDITOR_CONTEXT(sceneManager))
{
	m_camera.SetFar(5000.0f);

	m_image->AddPlugin<OvUI::Plugins::DDTarget<std::pair<std::string, OvUI::Widgets::Layout::Group*>>>("File").DataReceivedEvent += [this](auto p_data)
	{
		std::string path = p_data.first;

		switch (OvTools::Utils::PathParser::GetFileType(path))
		{
		case OvTools::Utils::PathParser::EFileType::SCENE:	EDITOR_EXEC(LoadSceneFromDisk(path));			break;
		case OvTools::Utils::PathParser::EFileType::MODEL:	EDITOR_EXEC(CreateActorWithModel(path, true));	break;
		}
	};

	OvCore::ECS::Actor::DestroyedEvent += [this](const OvCore::ECS::Actor& actor)
	{
		if (m_highlightedActor.has_value() && m_highlightedActor->get().GetID() == actor.GetID())
		{
			m_highlightedActor = std::nullopt;
		}
	};
}

void OvEditor::Panels::SceneView::Update(float p_deltaTime)
{
	AViewControllable::Update(p_deltaTime);

	using namespace OvWindowing::Inputs;

	if (IsFocused() && !m_cameraController.IsRightMousePressed())
	{
		if (EDITOR_CONTEXT(inputManager)->IsKeyPressed(EKey::KEY_W))
		{
			m_currentOperation = OvEditor::Core::EGizmoOperation::TRANSLATE;
		}

		if (EDITOR_CONTEXT(inputManager)->IsKeyPressed(EKey::KEY_E))
		{
			m_currentOperation = OvEditor::Core::EGizmoOperation::ROTATE;
		}

		if (EDITOR_CONTEXT(inputManager)->IsKeyPressed(EKey::KEY_R))
		{
			m_currentOperation = OvEditor::Core::EGizmoOperation::SCALE;
		}
	}
}

OvCore::SceneSystem::Scene* OvEditor::Panels::SceneView::GetScene()
{
	return m_sceneManager.GetCurrentScene();
}

/*
void OvEditor::Panels::SceneView::_Render_Impl()
{
	OvRendering::Context::Driver& driver = *EDITOR_CONTEXT(driver).get();

	driver.UpdateStateMask();
	OvRendering::Data::StateMask stateMask = driver.GetStateMask();
	HandleActorPicking();
	driver.ApplyStateMask(stateMask);
	RenderScene(stateMask);
	driver.ApplyStateMask(stateMask);
}

void OvEditor::Panels::SceneView::RenderScene(OvRendering::Data::StateMask p_defaultStateMask)
{
	auto& driver = *EDITOR_CONTEXT(driver).get();
	auto& currentScene = *m_sceneManager.GetCurrentScene();
	auto& gameView = EDITOR_PANEL(OvEditor::Panels::GameView, "Game View");
	auto& renderer = GetRendererAs<OvCore::Rendering::SceneRenderer>();
	auto& editorViewRenderFeature = renderer.GetFeature<OvEditor::Rendering::EditorViewRenderFeature>();

	// If the game is playing, and ShowLightFrustumCullingInSceneView is true, apply the game view frustum culling to the scene view (For debugging purposes)
	// TODO: Reenable
	if (auto gameViewFrustum = gameView.GetActiveFrustum(); gameViewFrustum.has_value() && gameView.GetCamera().HasFrustumLightCulling() && Settings::EditorSettings::ShowLightFrustumCullingInSceneView)
	{
		m_sceneRenderer.UpdateLightsInFrustum(currentScene, gameViewFrustum.value());
	}
	else
	{
		EDITOR_RENDERER().UpdateLights(currentScene);
	}

	const auto viewportWidth = static_cast<uint16_t>(m_size.x);
	const auto viewportHeight = static_cast<uint16_t>(m_size.y);

	renderer.PrepareScene(currentScene, viewportWidth, viewportHeight);

	driver.SetStencilMask(0xFF);
	OvMaths::FVector3 clearColor = m_camera.GetClearColor();
	driver.SetClearColor(clearColor.x, clearColor.y, clearColor.z);
	driver.Clear(true, true, false);
	driver.SetStencilMask(0x00);

	editorViewRenderFeature.RenderGrid(m_camera.GetPosition(), m_gridColor);
	editorViewRenderFeature.RenderCameras(currentScene);

	// If the game is playing, and ShowGeometryFrustumCullingInSceneView is true, apply the game view frustum culling to the scene view (For debugging purposes)
	if (auto gameViewFrustum = gameView.GetActiveFrustum(); gameViewFrustum.has_value() && gameView.GetCamera().HasFrustumGeometryCulling() && Settings::EditorSettings::ShowGeometryFrustumCullingInSceneView)
	{
		m_camera.SetFrustumGeometryCulling(gameView.HasCamera() ? gameView.GetCamera().HasFrustumGeometryCulling() : false);
		// renderer.RenderScene(currentScene, m_camera, gameViewFrustum);
		m_camera.SetFrustumGeometryCulling(false);
	}
	else
	{
		// renderer.RenderScene(currentScene, m_camera);
	}

	editorViewRenderFeature.RenderLights(currentScene);

	// TODO: Reenable
	if (EDITOR_EXEC(IsAnyActorSelected()))
	{
		auto& selectedActor = EDITOR_EXEC(GetSelectedActor());

		if (selectedActor.IsActive())
		{
			editorViewRenderFeature.RenderActorOutlinePass(selectedActor, true, true);
			driver.ApplyStateMask(p_defaultStateMask);
			editorViewRenderFeature.RenderActorOutlinePass(selectedActor, false, true);
		}

		driver.ApplyStateMask(p_defaultStateMask);
		driver.Clear(false, true, false);

		int highlightedAxis = -1;

		if (m_highlightedGizmoDirection.has_value())
		{
			highlightedAxis = static_cast<int>(m_highlightedGizmoDirection.value());
		}

		editorViewRenderFeature.RenderGizmo(selectedActor.transform.GetWorldPosition(), selectedActor.transform.GetWorldRotation(), m_currentOperation, false, highlightedAxis);
	}

	if (m_highlightedActor.has_value())
	{
		editorViewRenderFeature.RenderActorOutlinePass(m_highlightedActor.value().get(), true, false);
		driver.ApplyStateMask(p_defaultStateMask);
		editorViewRenderFeature.RenderActorOutlinePass(m_highlightedActor.value().get(), false, false);
	}
}

void OvEditor::Panels::SceneView::RenderSceneForActorPicking()
{
	auto& driver = m_renderer->GetDriver();
	auto& renderer = GetRendererAs<OvCore::Rendering::SceneRenderer>();
	auto& editorViewRenderFeature = renderer.GetFeature<OvEditor::Rendering::EditorViewRenderFeature>();

	if (auto scene = m_sceneManager.GetCurrentScene(); scene)
	{
		auto [winWidth, winHeight] = GetSafeSize();

		m_actorPickingFramebuffer.Resize(winWidth, winHeight);
		m_actorPickingFramebuffer.Bind();
		driver.SetClearColor(1.0f, 1.0f, 1.0f);
		driver.Clear();
		editorViewRenderFeature.RenderSceneForActorPicking(*scene);

		if (EDITOR_EXEC(IsAnyActorSelected()))
		{
			auto& selectedActor = EDITOR_EXEC(GetSelectedActor());
			driver.Clear(false, true, false);
			editorViewRenderFeature.RenderGizmo(selectedActor.transform.GetWorldPosition(), selectedActor.transform.GetWorldRotation(), m_currentOperation, true);
		}

		m_actorPickingFramebuffer.Unbind();
	}
}
*/

bool IsResizing()
{
	auto cursor = ImGui::GetMouseCursor();

	return 
		cursor == ImGuiMouseCursor_ResizeEW ||
		cursor == ImGuiMouseCursor_ResizeNS ||
		cursor == ImGuiMouseCursor_ResizeNWSE ||
		cursor == ImGuiMouseCursor_ResizeNESW ||
		cursor == ImGuiMouseCursor_ResizeAll;;
}

void OvEditor::Panels::SceneView::HandleActorPicking()
{
	using namespace OvWindowing::Inputs;

	auto& inputManager = *EDITOR_CONTEXT(inputManager);

	if (inputManager.IsMouseButtonReleased(EMouseButton::MOUSE_BUTTON_LEFT))
	{
		m_gizmoOperations.StopPicking();
	}

	if (IsHovered() && !IsResizing())
	{
		// RenderSceneForActorPicking();

		// Look actor under mouse
		auto [mouseX, mouseY] = inputManager.GetMousePosition();
		mouseX -= m_position.x;
		mouseY -= m_position.y;
		mouseY = GetSafeSize().second - mouseY + 25;

		m_actorPickingFramebuffer.Bind();
		uint8_t pixel[3];
		EDITOR_CONTEXT(driver)->ReadPixels(static_cast<int>(mouseX), static_cast<int>(mouseY), 1, 1, OvRendering::Settings::EPixelDataFormat::RGB, OvRendering::Settings::EPixelDataType::UNSIGNED_BYTE, pixel);
		m_actorPickingFramebuffer.Unbind();

		uint32_t actorID = (0 << 24) | (pixel[2] << 16) | (pixel[1] << 8) | (pixel[0] << 0);
		auto actorUnderMouse = EDITOR_CONTEXT(sceneManager).GetCurrentScene()->FindActorByID(actorID);
		auto direction = m_gizmoOperations.IsPicking() ? m_gizmoOperations.GetDirection() : EDITOR_EXEC(IsAnyActorSelected()) && pixel[0] == 255 && pixel[1] == 255 && pixel[2] >= 252 && pixel[2] <= 254 ? static_cast<OvEditor::Core::GizmoBehaviour::EDirection>(pixel[2] - 252) : std::optional<Core::GizmoBehaviour::EDirection>{};

		m_highlightedActor = {};
		m_highlightedGizmoDirection = {};

		if (!m_cameraController.IsRightMousePressed())
		{
			if (direction.has_value())
			{
				m_highlightedGizmoDirection = direction;

			}
			else if (actorUnderMouse != nullptr)
			{
				m_highlightedActor = std::ref(*actorUnderMouse);
			}
		}

		/* Click */
		if (inputManager.IsMouseButtonPressed(EMouseButton::MOUSE_BUTTON_LEFT) && !m_cameraController.IsRightMousePressed())
		{
			/* Gizmo picking */
			if (direction.has_value())
			{
				m_gizmoOperations.StartPicking(EDITOR_EXEC(GetSelectedActor()), m_camera.GetPosition(), m_currentOperation, direction.value());
			}
			/* Actor picking */
			else
			{

				if (actorUnderMouse)
				{
					EDITOR_EXEC(SelectActor(*actorUnderMouse));
				}
				else
				{
					EDITOR_EXEC(UnselectActor());
				}
			}
		}
	}
	else
	{
		m_highlightedActor = std::nullopt;
	}

	if (m_gizmoOperations.IsPicking())
	{
		auto mousePosition = EDITOR_CONTEXT(inputManager)->GetMousePosition();

		auto [winWidth, winHeight] = GetSafeSize();

		m_gizmoOperations.SetCurrentMouse({ static_cast<float>(mousePosition.first), static_cast<float>(mousePosition.second) });
		m_gizmoOperations.ApplyOperation(m_camera.GetViewMatrix(), m_camera.GetProjectionMatrix(), { static_cast<float>(winWidth), static_cast<float>(winHeight) });
	}
}
