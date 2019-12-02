/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvEditor/Core/EditorActions.h"

#include "OvUI/imgui/imgui.h"

#include "OvEditor/Core/CameraController.h"

#include <OvCore/ECS/Components/CPhysicalBox.h>
#include <OvCore/ECS/Components/CPhysicalSphere.h>
#include <OvCore/ECS/Components/CPhysicalCapsule.h>
#include <OvCore/ECS/Components/CAmbientBoxLight.h>
#include <OvCore/ECS/Components/CAmbientSphereLight.h>

OvEditor::Core::CameraController::CameraController
(
	OvEditor::Panels::AView& p_view,
	OvRendering::LowRenderer::Camera& p_camera,
	OvMaths::FVector3& p_position,
	bool p_enableFocusInputs
) :
	m_inputManager(*EDITOR_CONTEXT(inputManager)),
	m_window(*EDITOR_CONTEXT(window)),
	m_view(p_view),
	m_camera(p_camera),
	m_cameraPosition(p_position),
	m_enableFocusInputs(p_enableFocusInputs)
{
	m_camera.SetFov(60.0f);
}

float GetActorFocusDist(OvCore::ECS::Actor& p_actor)
{
	float distance = 4.0f;

	if (p_actor.IsActive())
	{
		if (auto pb = p_actor.GetComponent<OvCore::ECS::Components::CPhysicalBox>())
		{
			distance = std::max(distance, std::max
			(
				std::max
				(
					pb->GetSize().x * p_actor.transform.GetWorldScale().x,
					pb->GetSize().y * p_actor.transform.GetWorldScale().y
				),
				pb->GetSize().z * p_actor.transform.GetWorldScale().z
			) * 1.5f);
		}

		if (auto ps = p_actor.GetComponent<OvCore::ECS::Components::CPhysicalSphere>())
		{
			distance = std::max(distance, std::max
			(
				std::max
				(
					ps->GetRadius() * p_actor.transform.GetWorldScale().x,
					ps->GetRadius() * p_actor.transform.GetWorldScale().y
				),
				ps->GetRadius() * p_actor.transform.GetWorldScale().z
			) * 1.5f);
		}

		if (auto pc = p_actor.GetComponent<OvCore::ECS::Components::CPhysicalCapsule>())
		{
			distance = std::max(distance, std::max
			(
				std::max
				(
					pc->GetRadius() * p_actor.transform.GetWorldScale().x,
					pc->GetHeight() * p_actor.transform.GetWorldScale().y
				),
				pc->GetRadius() * p_actor.transform.GetWorldScale().z
			) * 1.5f);
		}

		if (auto modelRenderer = p_actor.GetComponent<OvCore::ECS::Components::CModelRenderer>())
		{
			const bool hasCustomBoundingSphere = modelRenderer->GetFrustumBehaviour() == OvCore::ECS::Components::CModelRenderer::EFrustumBehaviour::CULL_CUSTOM;
			const bool hasModel = modelRenderer->GetModel();
			const auto boundingSphere = hasCustomBoundingSphere ? &modelRenderer->GetCustomBoundingSphere() : hasModel ? &modelRenderer->GetModel()->GetBoundingSphere() : nullptr;
			const auto& actorPosition = p_actor.transform.GetWorldPosition();
			const auto& actorScale = p_actor.transform.GetWorldScale();
			const auto scaleFactor = std::max(std::max(actorScale.x, actorScale.y), actorScale.z);

			distance = std::max(distance, boundingSphere ? (boundingSphere->radius + OvMaths::FVector3::Length(boundingSphere->position)) * scaleFactor * 2.0f : 10.0f);
		}

		for (auto child : p_actor.GetChildren())
			distance = std::max(distance, GetActorFocusDist(*child));
	}

	return distance;
}

void OvEditor::Core::CameraController::HandleInputs(float p_deltaTime)
{
	if (m_view.IsHovered())
	{
		UpdateMouseState();

		ImGui::GetIO().DisableMouseUpdate = m_rightMousePressed || m_middleMousePressed;

		if (!ImGui::IsAnyItemActive() && m_enableFocusInputs)
		{
			if (EDITOR_EXEC(IsAnyActorSelected()))
			{
				auto targetPos = EDITOR_EXEC(GetSelectedActor()).transform.GetWorldPosition();
				float dist = GetActorFocusDist(EDITOR_EXEC(GetSelectedActor()));

				if (m_inputManager.IsKeyPressed(OvWindowing::Inputs::EKey::KEY_F))
					MoveToTarget(EDITOR_EXEC(GetSelectedActor()));

				if (m_inputManager.IsKeyPressed(OvWindowing::Inputs::EKey::KEY_UP))
					m_cameraDestinations.push({ targetPos + OvMaths::FVector3{ 0.f, 1.f, 0.f } * dist, -90.f, -90.f });

				if (m_inputManager.IsKeyPressed(OvWindowing::Inputs::EKey::KEY_DOWN))
					m_cameraDestinations.push({ targetPos + OvMaths::FVector3{ 0.f, -1.f, 0.f } * dist, 90.f, -90.f });

				if (m_inputManager.IsKeyPressed(OvWindowing::Inputs::EKey::KEY_LEFT))
					m_cameraDestinations.push({ targetPos + OvMaths::FVector3{ -1.f, 0.f, 0.f } * dist , 0.f, 0.f });

				if (m_inputManager.IsKeyPressed(OvWindowing::Inputs::EKey::KEY_RIGHT))
					m_cameraDestinations.push({ targetPos + OvMaths::FVector3{ 1.f, 0.f, 0.f } * dist, 0.f, 180.f });

				if (m_inputManager.IsKeyPressed(OvWindowing::Inputs::EKey::KEY_PAGE_DOWN))
					m_cameraDestinations.push({ targetPos + OvMaths::FVector3{ 0.f, 0.f, -1.f } * dist, 0.f, 90.f });

				if (m_inputManager.IsKeyPressed(OvWindowing::Inputs::EKey::KEY_PAGE_UP))
					m_cameraDestinations.push({ targetPos + OvMaths::FVector3{ 0.f, 0.f, 1.f } * dist, 0.f, -90.f });
			}
		}
	}

	if (!m_cameraDestinations.empty())
	{
		m_currentMovementSpeed = 0.0f;

		while (m_cameraDestinations.size() != 1)
			m_cameraDestinations.pop();

		auto& destination = m_cameraDestinations.front();

		OvMaths::FVector3 destPos = std::get<0>(destination);
		float destPitch = std::get<1>(destination);
		float destYaw = std::get<2>(destination);
		float t = m_focusLerpCoefficient * p_deltaTime;

		if (OvMaths::FVector3::Distance(m_cameraPosition, destPos) <= 0.03f)
			m_cameraPosition = destPos;
		else
			m_cameraPosition = OvMaths::FVector3::Lerp(m_cameraPosition, destPos, t);

		if (abs(m_camera.GetPitch() - destPitch) <= 0.03f)
			m_camera.SetPitch(destPitch);
		else
			m_camera.SetPitch(m_camera.GetPitch() + (1) * (destPitch - m_camera.GetPitch()));

		if (abs(m_camera.GetYaw() - destYaw) <= 0.03f)
			m_camera.SetYaw(destYaw);
		else
			m_camera.SetYaw(m_camera.GetYaw() + (1) *(destYaw - m_camera.GetYaw()));

		if (m_cameraPosition == destPos && m_camera.GetYaw() == destYaw && m_camera.GetPitch() == destPitch)
			m_cameraDestinations.pop();
	} 
	else
	{
		if (m_middleMousePressed)
			HandleCameraXYMovement(p_deltaTime);

		if (m_view.IsHovered())
			HandleCameraZMovement(p_deltaTime);

		if (m_rightMousePressed)
			HandleCameraRotation(p_deltaTime);

		HandleKeyboardMovement(p_deltaTime);
	}
}

void OvEditor::Core::CameraController::MoveToTarget(OvCore::ECS::Actor& p_target)
{
	m_cameraDestinations.push({ p_target.transform.GetWorldPosition() - m_camera.GetForward() * GetActorFocusDist(p_target), m_camera.GetPitch(), m_camera.GetYaw() });
}

void OvEditor::Core::CameraController::SetSpeed(float p_speed)
{
	m_cameraMoveSpeed = p_speed;
}

float OvEditor::Core::CameraController::GetSpeed() const
{
	return m_cameraMoveSpeed;
}

void OvEditor::Core::CameraController::SetPosition(const OvMaths::FVector3 & p_position)
{
	m_cameraPosition = p_position;
}

void OvEditor::Core::CameraController::SetRotation(const OvMaths::FQuaternion & p_rotation)
{
	m_camera.SetRotation(p_rotation);
}

OvMaths::FVector3 OvEditor::Core::CameraController::GetPosition() const
{
	return m_cameraPosition;
}

bool OvEditor::Core::CameraController::IsRightMousePressed() const
{
	return m_rightMousePressed;
}

void OvEditor::Core::CameraController::HandleCameraXYMovement(float p_deltaTime)
{
	ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

	auto[xPos, yPos] = m_inputManager.GetMousePosition();

	if (m_firstMouse)
	{
		m_lastMousePosX = xPos;
		m_lastMousePosY = yPos;
		m_firstMouse = false;
	}

	float xoffset = static_cast<float>(xPos - m_lastMousePosX);
	float yoffset = static_cast<float>(m_lastMousePosY - yPos);

	m_lastMousePosX = xPos;
	m_lastMousePosY = yPos;

	m_cameraPosition -= m_camera.GetRight() * xoffset * m_cameraDragSpeed;
	m_cameraPosition -= m_camera.GetUp() * yoffset * m_cameraDragSpeed;
}

void OvEditor::Core::CameraController::HandleCameraZMovement(float p_deltaTime)
{
	m_cameraPosition += m_camera.GetForward() * ImGui::GetIO().MouseWheel;
}

void OvEditor::Core::CameraController::HandleCameraRotation(float p_deltaTime)
{
	auto[xPos, yPos] = m_inputManager.GetMousePosition();

	if (m_firstMouse)
	{
		m_lastMousePosX = xPos;
		m_lastMousePosY = yPos;
		m_firstMouse = false;
	}

	float xoffset = static_cast<float>(xPos - m_lastMousePosX);
	float yoffset = static_cast<float>(m_lastMousePosY - yPos);

	m_lastMousePosX = xPos;
	m_lastMousePosY = yPos;

	m_camera.SetYaw(m_camera.GetYaw() + xoffset * m_mouseSensitivity);
	m_camera.SetPitch(m_camera.GetPitch() + yoffset * m_mouseSensitivity);

	if (m_camera.GetPitch() > 89.0f)
		m_camera.SetPitch(89.0f);

	if (m_camera.GetPitch() < -89.0f)
		m_camera.SetPitch(-89.0f);
}

void OvEditor::Core::CameraController::HandleKeyboardMovement(float p_deltaTime)
{
	bool run = m_inputManager.GetKeyState(OvWindowing::Inputs::EKey::KEY_LEFT_SHIFT) == OvWindowing::Inputs::EKeyState::KEY_DOWN;
	float velocity = m_cameraMoveSpeed * p_deltaTime * (run ? 2.0f : 1.0f);

	m_targetSpeed = OvMaths::FVector3(0.f, 0.f, 0.f);

	if (m_rightMousePressed)
	{
		if (m_inputManager.GetKeyState(OvWindowing::Inputs::EKey::KEY_W) == OvWindowing::Inputs::EKeyState::KEY_DOWN)
			m_targetSpeed += m_camera.GetForward() * velocity;
		if (m_inputManager.GetKeyState(OvWindowing::Inputs::EKey::KEY_S) == OvWindowing::Inputs::EKeyState::KEY_DOWN)
			m_targetSpeed += m_camera.GetForward() * -velocity;
		if (m_inputManager.GetKeyState(OvWindowing::Inputs::EKey::KEY_A) == OvWindowing::Inputs::EKeyState::KEY_DOWN)
			m_targetSpeed += m_camera.GetRight() * -velocity;
		if (m_inputManager.GetKeyState(OvWindowing::Inputs::EKey::KEY_D) == OvWindowing::Inputs::EKeyState::KEY_DOWN)
			m_targetSpeed += m_camera.GetRight() * velocity;
		if (m_inputManager.GetKeyState(OvWindowing::Inputs::EKey::KEY_E) == OvWindowing::Inputs::EKeyState::KEY_DOWN)
			m_targetSpeed += {0.0f, velocity, 0.0f};
		if (m_inputManager.GetKeyState(OvWindowing::Inputs::EKey::KEY_Q) == OvWindowing::Inputs::EKeyState::KEY_DOWN)
			m_targetSpeed += {0.0f, -velocity, 0.0f};
	}

	m_currentMovementSpeed = OvMaths::FVector3::Lerp(m_currentMovementSpeed, m_targetSpeed, 10.0f * p_deltaTime);
	m_cameraPosition += m_currentMovementSpeed;
}

void OvEditor::Core::CameraController::UpdateMouseState()
{
	if (m_inputManager.IsMouseButtonPressed(OvWindowing::Inputs::EMouseButton::MOUSE_BUTTON_LEFT))
		m_leftMousePressed = true;

	if (m_inputManager.IsMouseButtonReleased(OvWindowing::Inputs::EMouseButton::MOUSE_BUTTON_LEFT))
	{
		m_leftMousePressed = false;
		m_firstMouse = true;
	}

	if (m_inputManager.IsMouseButtonPressed(OvWindowing::Inputs::EMouseButton::MOUSE_BUTTON_MIDDLE))
		m_middleMousePressed = true;

	if (m_inputManager.IsMouseButtonReleased(OvWindowing::Inputs::EMouseButton::MOUSE_BUTTON_MIDDLE))
	{
		m_middleMousePressed = false;
		m_firstMouse = true;
	}

	if (m_inputManager.IsMouseButtonPressed(OvWindowing::Inputs::EMouseButton::MOUSE_BUTTON_RIGHT))
	{
		m_rightMousePressed = true;
		m_window.SetCursorMode(OvWindowing::Cursor::ECursorMode::DISABLED);
	}

	if (m_inputManager.IsMouseButtonReleased(OvWindowing::Inputs::EMouseButton::MOUSE_BUTTON_RIGHT))
	{
		m_rightMousePressed = false;
		m_firstMouse = true;
		m_window.SetCursorMode(OvWindowing::Cursor::ECursorMode::NORMAL);
	}
}
