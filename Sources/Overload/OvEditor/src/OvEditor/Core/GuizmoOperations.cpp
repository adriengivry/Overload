/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvEditor/Core/GuizmoOperations.h"

void OvEditor::Core::GuizmoOperations::StartPicking(OvCore::ECS::Actor& p_target, const OvMaths::FVector3& p_direction, const OvMaths::FVector3& p_cameraPosition)
{
	m_target = &p_target;
	m_direction = p_direction;
	m_firstMouse = true;
	m_distanceToActor = OvMaths::FVector3::Distance(p_cameraPosition, m_target->transform.GetWorldPosition());

}

void OvEditor::Core::GuizmoOperations::StopPicking()
{
	m_target = nullptr;
}

void OvEditor::Core::GuizmoOperations::ApplyTranslation(const OvMaths::FMatrix4& p_viewMatrix, const OvMaths::FMatrix4& p_projectionMatrix, const OvMaths::FVector2& p_viewSize) const
{
	float unitsPerPixel = 0.001f;

	auto start = m_target->transform.GetWorldPosition();
	auto end = m_target->transform.GetWorldPosition() + m_direction;
	auto result = OvMaths::FVector2();

	{
		auto clipSpacePos = p_projectionMatrix * (p_viewMatrix * OvMaths::FVector4{ end.x, end.y, end.z, 1.0f });
		auto ndcSpacePos = OvMaths::FVector3{ clipSpacePos.x, clipSpacePos.y, clipSpacePos.z } / clipSpacePos.w;
		auto windowSpacePos = ((OvMaths::FVector2{ ndcSpacePos.x, ndcSpacePos.y } +1.0) / 2.0);
		windowSpacePos.x *= p_viewSize.x;
		windowSpacePos.y *= p_viewSize.y;
		result = windowSpacePos;
	}

	{
		auto clipSpacePos = p_projectionMatrix * (p_viewMatrix * OvMaths::FVector4{ start.x, start.y, start.z, 1.0f });
		auto ndcSpacePos = OvMaths::FVector3{ clipSpacePos.x, clipSpacePos.y, clipSpacePos.z } / clipSpacePos.w;
		auto windowSpacePos = ((OvMaths::FVector2{ ndcSpacePos.x, ndcSpacePos.y } +1.0) / 2.0);
		windowSpacePos.x *= p_viewSize.x;
		windowSpacePos.y *= p_viewSize.y;
		result -= windowSpacePos;
	}

	result.y *= -1;
	result = OvMaths::FVector2::Normalize(result);

	auto displacement = m_currentMouse - m_previousMouse;

	auto translationUnit = OvMaths::FVector2::Dot(displacement, result);
	
	m_target->transform.TranslateLocal(m_direction * translationUnit * unitsPerPixel * m_distanceToActor);
}

void OvEditor::Core::GuizmoOperations::SetCurrentMouse(const OvMaths::FVector2& p_mousePosition)
{
	if (m_firstMouse)
	{
		m_currentMouse = m_previousMouse = p_mousePosition;
		m_firstMouse = false;
	}
	else
	{
		m_previousMouse = m_currentMouse;
		m_currentMouse = p_mousePosition;
	}
}

bool OvEditor::Core::GuizmoOperations::IsPicking() const
{
	return m_target;
}
