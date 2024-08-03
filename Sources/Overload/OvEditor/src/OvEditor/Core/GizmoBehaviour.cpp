/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvEditor/Core/GizmoBehaviour.h"
#include "OvEditor/Core/EditorActions.h"
#include "OvEditor/Settings/EditorSettings.h"

float SnapValue(float p_value, float p_step)
{
	return p_value - std::fmod(p_value, p_step);
}

bool OvEditor::Core::GizmoBehaviour::IsSnappedBehaviourEnabled() const
{
	using namespace OvWindowing::Inputs;

	const auto& inputManager = EDITOR_CONTEXT(inputManager);
	return inputManager->GetKeyState(EKey::KEY_LEFT_CONTROL) == EKeyState::KEY_DOWN || inputManager->GetKeyState(EKey::KEY_RIGHT_CONTROL) == EKeyState::KEY_DOWN;
}

void OvEditor::Core::GizmoBehaviour::StartPicking(OvCore::ECS::Actor& p_target, const OvMaths::FVector3& p_cameraPosition, EGizmoOperation p_operation, EDirection p_direction)
{
	m_target = &p_target;
	m_firstMouse = true;
	m_firstPick = true;
	m_originalTransform = p_target.transform.GetFTransform();
	m_distanceToActor = OvMaths::FVector3::Distance(p_cameraPosition, m_target->transform.GetWorldPosition());
	m_currentOperation = p_operation;
	m_direction = p_direction;
	m_cameraPosition = p_cameraPosition;
}

void OvEditor::Core::GizmoBehaviour::StopPicking()
{
	m_target = nullptr;
}

OvMaths::FVector3 OvEditor::Core::GizmoBehaviour::GetFakeDirection() const
{
	auto result = OvMaths::FVector3();

	switch (m_direction)
	{
	case OvEditor::Core::GizmoBehaviour::EDirection::X:
		result = OvMaths::FVector3::Right;
		break;
	case OvEditor::Core::GizmoBehaviour::EDirection::Y:
		result = OvMaths::FVector3::Up;
		break;
	case OvEditor::Core::GizmoBehaviour::EDirection::Z:
		result = OvMaths::FVector3::Forward;
		break;
	}

	return result;
}

OvMaths::FVector3 OvEditor::Core::GizmoBehaviour::GetRealDirection(bool p_relative) const
{
	auto result = OvMaths::FVector3();

	switch (m_direction)
	{
	case OvEditor::Core::GizmoBehaviour::EDirection::X:
		result = p_relative ? m_originalTransform.GetWorldRight() : m_originalTransform.GetLocalRight();
		break;
	case OvEditor::Core::GizmoBehaviour::EDirection::Y:
		result = p_relative ? m_originalTransform.GetWorldUp() : m_originalTransform.GetLocalUp();
		break;
	case OvEditor::Core::GizmoBehaviour::EDirection::Z:
		result = p_relative ? m_originalTransform.GetWorldForward() : m_originalTransform.GetLocalForward();
		break;
	}

	return result;
}

OvMaths::FVector2 OvEditor::Core::GizmoBehaviour::GetScreenDirection(const OvMaths::FMatrix4& p_viewMatrix, const OvMaths::FMatrix4& p_projectionMatrix, const OvMaths::FVector2& p_viewSize) const
{
	auto start = m_originalTransform.GetWorldPosition();
	auto end = m_originalTransform.GetWorldPosition() + GetRealDirection(true) * 0.01f;

	auto start2D = OvMaths::FVector2();
	{
		auto clipSpacePos = p_projectionMatrix * (p_viewMatrix * OvMaths::FVector4{ start.x, start.y, start.z, 1.0f });
		auto ndcSpacePos = OvMaths::FVector3{ clipSpacePos.x, clipSpacePos.y, clipSpacePos.z } / clipSpacePos.w;
		auto windowSpacePos = ((OvMaths::FVector2{ ndcSpacePos.x, ndcSpacePos.y } + 1.0f) / 2.0f);
		windowSpacePos.x *= p_viewSize.x;
		windowSpacePos.y *= p_viewSize.y;
		start2D = windowSpacePos;
	}

	auto end2D = OvMaths::FVector2();
	{
		auto clipSpacePos = p_projectionMatrix * (p_viewMatrix * OvMaths::FVector4{ end.x, end.y, end.z, 1.0f });
		auto ndcSpacePos = OvMaths::FVector3{ clipSpacePos.x, clipSpacePos.y, clipSpacePos.z } / clipSpacePos.w;
		auto windowSpacePos = ((OvMaths::FVector2{ ndcSpacePos.x, ndcSpacePos.y } + 1.0f) / 2.0f);
		windowSpacePos.x *= p_viewSize.x;
		windowSpacePos.y *= p_viewSize.y;
		end2D = windowSpacePos;
	}

	auto result = end2D - start2D;

	result.y *= -1; // Screen coordinates are reversed, so we inverse the Y

	return OvMaths::FVector2::Normalize(result);
}

void OvEditor::Core::GizmoBehaviour::ApplyTranslation(const OvMaths::FMatrix4& p_viewMatrix, const OvMaths::FMatrix4& p_projectionMatrix, const OvMaths::FVector2& p_viewSize)
{
	auto ray = GetMouseRay(m_currentMouse, p_viewMatrix, p_projectionMatrix, p_viewSize);

	const OvMaths::FVector3 planeTangent = OvMaths::FVector3::Cross(GetRealDirection(true), m_target->transform.GetWorldPosition() - m_cameraPosition);
	const OvMaths::FVector3 planeNormal = OvMaths::FVector3::Cross(GetRealDirection(true), planeTangent);

	OvMaths::FVector3 direction = GetRealDirection(true);

	const OvMaths::FVector3 planePoint = m_originalTransform.GetWorldPosition();

	const float denom = OvMaths::FVector3::Dot(ray.second, planeNormal);

	if (std::abs(denom) == 0)
		return;

	const float t = OvMaths::FVector3::Dot(planePoint - ray.first, planeNormal) / denom;

	if (t < 0)
		return;

	OvMaths::FVector3 point = ray.first + ray.second * t;

	if (m_firstPick)
	{
		m_initialOffset = m_originalTransform.GetWorldPosition() - point;
		m_firstPick = false;
	}

	OvMaths::FVector3 projectedPoint = planePoint + direction * OvMaths::FVector3::Dot(point - planePoint + m_initialOffset, direction);

	m_target->transform.SetWorldPosition(projectedPoint);
}

void OvEditor::Core::GizmoBehaviour::ApplyRotation(const OvMaths::FMatrix4& p_viewMatrix, const OvMaths::FMatrix4& p_projectionMatrix, const OvMaths::FVector2& p_viewSize) const
{
	auto unitsPerPixel = 0.2f;
	auto originRotation = m_originalTransform.GetWorldRotation();

	auto screenDirection = GetScreenDirection(p_viewMatrix, p_projectionMatrix, p_viewSize);
	screenDirection = OvMaths::FVector2(-screenDirection.y, screenDirection.x);

	auto totalDisplacement = m_currentMouse - m_originMouse;
	auto rotationCoefficient = OvMaths::FVector2::Dot(totalDisplacement, screenDirection) * unitsPerPixel;

	if (IsSnappedBehaviourEnabled())
	{
		rotationCoefficient = SnapValue(rotationCoefficient, OvEditor::Settings::EditorSettings::RotationSnapUnit);
	}

	auto rotationToApply = OvMaths::FQuaternion(OvMaths::FVector3(GetFakeDirection() * rotationCoefficient));
	m_target->transform.SetWorldRotation(originRotation * rotationToApply);
}

void OvEditor::Core::GizmoBehaviour::ApplyScale(const OvMaths::FMatrix4& p_viewMatrix, const OvMaths::FMatrix4& p_projectionMatrix, const OvMaths::FVector2& p_viewSize) const
{
	auto unitsPerPixel = 0.01f;
	auto originScale = m_originalTransform.GetWorldScale();

	auto screenDirection = GetScreenDirection(p_viewMatrix, p_projectionMatrix, p_viewSize);

	auto totalDisplacement = m_currentMouse - m_originMouse;
	auto scaleCoefficient = OvMaths::FVector2::Dot(totalDisplacement, screenDirection) * unitsPerPixel;

	if (IsSnappedBehaviourEnabled())
	{
		scaleCoefficient = SnapValue(scaleCoefficient, OvEditor::Settings::EditorSettings::ScalingSnapUnit);
	}

	auto newScale = originScale + GetFakeDirection() * scaleCoefficient;

	/* Prevent scale from being negative*/
	newScale.x = std::max(newScale.x, 0.0001f);
	newScale.y = std::max(newScale.y, 0.0001f);
	newScale.z = std::max(newScale.z, 0.0001f);

	m_target->transform.SetWorldScale(newScale);
}

void OvEditor::Core::GizmoBehaviour::ApplyOperation(const OvMaths::FMatrix4& p_viewMatrix, const OvMaths::FMatrix4& p_projectionMatrix, const OvMaths::FVector2& p_viewSize)
{
	switch (m_currentOperation)
	{
	case EGizmoOperation::TRANSLATE:
		ApplyTranslation(p_viewMatrix, p_projectionMatrix, p_viewSize);
		break;

	case EGizmoOperation::ROTATE:
		ApplyRotation(p_viewMatrix, p_projectionMatrix, p_viewSize);
		break;

	case EGizmoOperation::SCALE:
		ApplyScale(p_viewMatrix, p_projectionMatrix, p_viewSize);
		break;
	}
}

void OvEditor::Core::GizmoBehaviour::SetCurrentMouse(const OvMaths::FVector2& p_mousePosition)
{
	if (m_firstMouse)
	{
		m_currentMouse = m_originMouse = p_mousePosition;
		m_firstMouse = false;
	}
	else
	{
		m_currentMouse = p_mousePosition;
	}
}

bool OvEditor::Core::GizmoBehaviour::IsPicking() const
{
	return m_target;
}

OvEditor::Core::GizmoBehaviour::EDirection OvEditor::Core::GizmoBehaviour::GetDirection() const
{
	return m_direction;
}

std::pair<OvMaths::FVector3, OvMaths::FVector3> OvEditor::Core::GizmoBehaviour::GetMouseRay(const OvMaths::FVector2& p_mousePos, const OvMaths::FMatrix4& p_viewMatrix, const OvMaths::FMatrix4& p_projectionMatrix, const OvMaths::FVector2& p_viewSize)
{
	OvMaths::FVector4 rayStartNDC(
		(p_mousePos.x / p_viewSize.x - 0.5f) * 2.0f,
		(p_mousePos.y / p_viewSize.y - 0.5f) * -2.0f,
		-1.0,
		1.0f
	);
	OvMaths::FVector4 rayEndNDC(
		(p_mousePos.x / p_viewSize.x - 0.5f) * 2.0f,
		(p_mousePos.y / p_viewSize.y - 0.5f) * -2.0f,
		0.0,
		1.0f
	);

	OvMaths::FMatrix4 inverseProj = OvMaths::FMatrix4::Inverse(p_projectionMatrix);
	OvMaths::FMatrix4 inverseView = OvMaths::FMatrix4::Inverse(p_viewMatrix);

	OvMaths::FVector4 rayStartWorld = inverseView * inverseProj * rayStartNDC;
	rayStartWorld /= rayStartWorld.w;
	OvMaths::FVector4 rayEndWorld = inverseView * inverseProj * rayEndNDC;
	rayEndWorld /= rayEndWorld.w;

	OvMaths::FVector3 rayDirWorld(OvMaths::FVector3(rayEndWorld.x, rayEndWorld.y, rayEndWorld.z) - OvMaths::FVector3(rayStartWorld.x, rayStartWorld.y, rayStartWorld.z));
	rayDirWorld = OvMaths::FVector3::Normalize(rayDirWorld);

	return std::make_pair(OvMaths::FVector3(rayStartWorld.x, rayStartWorld.y, rayStartWorld.z), rayDirWorld);
}
