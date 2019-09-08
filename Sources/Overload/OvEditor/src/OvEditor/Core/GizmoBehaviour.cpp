/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvEditor/Core/GizmoBehaviour.h"

void OvEditor::Core::GizmoBehaviour::StartPicking(OvCore::ECS::Actor& p_target, const OvMaths::FVector3& p_cameraPosition, EGizmoOperation p_operation, EDirection p_direction)
{
	m_target = &p_target;
	m_firstMouse = true;
	m_originalTransform = p_target.transform.GetFTransform();
	m_distanceToActor = OvMaths::FVector3::Distance(p_cameraPosition, m_target->transform.GetWorldPosition());
	m_currentOperation = p_operation;
	m_direction = p_direction;
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
	auto start = m_target->transform.GetWorldPosition();
	auto end = m_target->transform.GetWorldPosition() + GetRealDirection(true);

	auto start2D = OvMaths::FVector2();
	{
		auto clipSpacePos = p_projectionMatrix * (p_viewMatrix * OvMaths::FVector4{ start.x, start.y, start.z, 1.0f });
		auto ndcSpacePos = OvMaths::FVector3{ clipSpacePos.x, clipSpacePos.y, clipSpacePos.z } / clipSpacePos.w;
		auto windowSpacePos = ((OvMaths::FVector2{ ndcSpacePos.x, ndcSpacePos.y } +1.0) / 2.0);
		windowSpacePos.x *= p_viewSize.x;
		windowSpacePos.y *= p_viewSize.y;
		start2D = windowSpacePos;
	}

	auto end2D = OvMaths::FVector2();
	{
		auto clipSpacePos = p_projectionMatrix * (p_viewMatrix * OvMaths::FVector4{ end.x, end.y, end.z, 1.0f });
		auto ndcSpacePos = OvMaths::FVector3{ clipSpacePos.x, clipSpacePos.y, clipSpacePos.z } / clipSpacePos.w;
		auto windowSpacePos = ((OvMaths::FVector2{ ndcSpacePos.x, ndcSpacePos.y } +1.0) / 2.0);
		windowSpacePos.x *= p_viewSize.x;
		windowSpacePos.y *= p_viewSize.y;
		end2D = windowSpacePos;
	}

	auto result = end2D - start2D;

	result.y *= -1; // Screen coordinates are reversed, so we inverse the Y

	return OvMaths::FVector2::Normalize(result);
}

void OvEditor::Core::GizmoBehaviour::ApplyTranslation(const OvMaths::FMatrix4& p_viewMatrix, const OvMaths::FMatrix4& p_projectionMatrix, const OvMaths::FVector2& p_viewSize) const
{
	auto unitsPerPixel = 0.001f * m_distanceToActor;
	auto originPosition = m_originalTransform.GetLocalPosition();

	auto screenDirection = GetScreenDirection(p_viewMatrix, p_projectionMatrix, p_viewSize);

	auto totalDisplacement = m_currentMouse - m_originMouse;
	auto translationCoefficient = OvMaths::FVector2::Dot(totalDisplacement, screenDirection);

	m_target->transform.SetLocalPosition(originPosition + GetRealDirection() * translationCoefficient * unitsPerPixel);
}

void OvEditor::Core::GizmoBehaviour::ApplyRotation(const OvMaths::FMatrix4& p_viewMatrix, const OvMaths::FMatrix4& p_projectionMatrix, const OvMaths::FVector2& p_viewSize) const
{
	auto unitsPerPixel = 0.2f;
	auto originRotation = m_originalTransform.GetLocalRotation();

	auto screenDirection = GetScreenDirection(p_viewMatrix, p_projectionMatrix, p_viewSize);
	screenDirection = OvMaths::FVector2(-screenDirection.y, screenDirection.x);

	auto totalDisplacement = m_currentMouse - m_originMouse;
	auto rotationCoefficient = OvMaths::FVector2::Dot(totalDisplacement, screenDirection);

	auto rotationToApply = OvMaths::FQuaternion(OvMaths::FVector3(GetFakeDirection() * rotationCoefficient * unitsPerPixel));
	m_target->transform.SetLocalRotation(originRotation * rotationToApply);
}

void OvEditor::Core::GizmoBehaviour::ApplyScale(const OvMaths::FMatrix4& p_viewMatrix, const OvMaths::FMatrix4& p_projectionMatrix, const OvMaths::FVector2& p_viewSize) const
{
	auto unitsPerPixel = 0.01f;
	auto originScale = m_originalTransform.GetLocalScale();

	auto screenDirection = GetScreenDirection(p_viewMatrix, p_projectionMatrix, p_viewSize);

	auto totalDisplacement = m_currentMouse - m_originMouse;
	auto scaleCoefficient = OvMaths::FVector2::Dot(totalDisplacement, screenDirection);

	auto newScale = originScale + GetFakeDirection() * scaleCoefficient * unitsPerPixel;

	/* Prevent scale from being negative*/
	newScale.x = std::max(newScale.x, 0.0001f);
	newScale.y = std::max(newScale.y, 0.0001f);
	newScale.z = std::max(newScale.z, 0.0001f);

	m_target->transform.SetLocalScale(newScale);
}

void OvEditor::Core::GizmoBehaviour::ApplyOperation(const OvMaths::FMatrix4& p_viewMatrix, const OvMaths::FMatrix4& p_projectionMatrix, const OvMaths::FVector2& p_viewSize)
{
	switch (m_currentOperation)
	{
	case EGizmoOperation::TRANSLATION:
		ApplyTranslation(p_viewMatrix, p_projectionMatrix, p_viewSize);
		break;

	case EGizmoOperation::ROTATION:
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