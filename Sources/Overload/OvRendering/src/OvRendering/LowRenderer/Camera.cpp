/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include <cmath>

#include "OvRendering/LowRenderer/Camera.h"
#include "OvMaths/FMatrix4.h"

OvRendering::LowRenderer::Camera::Camera() :
	m_yaw(-90.f),
	m_pitch(0.f),
	m_roll(0.f),
	m_fov(45.f),
	m_near(0.1f),
	m_far(1000.f),
	m_clearColor(0.f, 0.f, 0.f)
{
	UpdateCameraVectors();
}

void OvRendering::LowRenderer::Camera::CacheMatrices(uint16_t p_windowWidth, uint16_t p_windowHeight, const OvMaths::FVector3& p_position)
{
	CacheProjectionMatrix(p_windowWidth, p_windowHeight);
	CacheViewMatrix(p_position);
	CacheFrustum(m_viewMatrix, m_projectionMatrix);
}

void OvRendering::LowRenderer::Camera::CacheProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight)
{
	m_projectionMatrix = CalculateProjectionMatrix(p_windowWidth, p_windowHeight);
}

void OvRendering::LowRenderer::Camera::CacheViewMatrix(const OvMaths::FVector3& p_position)
{
	m_viewMatrix = CalculateViewMatrix(p_position);
}

void OvRendering::LowRenderer::Camera::CacheFrustum(const OvMaths::FMatrix4& p_view, const OvMaths::FMatrix4& p_projection)
{
	m_frustum.CalculateFrustum(p_projection * p_view);
}

const OvMaths::FVector3 & OvRendering::LowRenderer::Camera::GetForward() const
{
	return m_forward;
}

const OvMaths::FVector3 & OvRendering::LowRenderer::Camera::GetUp() const
{
	return m_up;
}

const OvMaths::FVector3 & OvRendering::LowRenderer::Camera::GetRight() const
{
	return m_right;
}

float OvRendering::LowRenderer::Camera::GetYaw() const
{
	return m_yaw;
}

float OvRendering::LowRenderer::Camera::GetPitch() const
{
	return m_pitch;
}

float OvRendering::LowRenderer::Camera::GetRoll() const
{
	return m_roll;
}

float OvRendering::LowRenderer::Camera::GetFov() const
{
	return m_fov;
}

float OvRendering::LowRenderer::Camera::GetNear() const
{
	return m_near;
}

float OvRendering::LowRenderer::Camera::GetFar() const
{
	return m_far;
}

const OvMaths::FVector3 & OvRendering::LowRenderer::Camera::GetClearColor() const
{
	return m_clearColor;
}

const OvMaths::FMatrix4& OvRendering::LowRenderer::Camera::GetProjectionMatrix() const
{
	return m_projectionMatrix;
}

const OvMaths::FMatrix4& OvRendering::LowRenderer::Camera::GetViewMatrix() const
{
	return m_viewMatrix;
}

const OvRendering::Data::Frustum& OvRendering::LowRenderer::Camera::GetFrustum() const
{
	return m_frustum;
}

void OvRendering::LowRenderer::Camera::SetYaw(float p_value)
{
	m_yaw = p_value;
	while (m_yaw > 180.0f) m_yaw -= 360.0f;
	UpdateCameraVectors();
}

void OvRendering::LowRenderer::Camera::SetPitch(float p_value)
{
	m_pitch = p_value;
	while (m_pitch > 180.0f) m_pitch -= 360.0f;
	UpdateCameraVectors();
}

void OvRendering::LowRenderer::Camera::SetRoll(float p_value)
{
	m_roll = p_value;
	while (m_roll > 180.0f) m_roll -= 360.0f;
	UpdateCameraVectors();
}

void OvRendering::LowRenderer::Camera::SetFov(float p_value)
{
	m_fov = p_value;
}

void OvRendering::LowRenderer::Camera::SetNear(float p_value)
{
	m_near = p_value;
}

void OvRendering::LowRenderer::Camera::SetFar(float p_value)
{
	m_far = p_value;
}

void OvRendering::LowRenderer::Camera::SetClearColor(const OvMaths::FVector3 & p_clearColor)
{
	m_clearColor = p_clearColor;
}

void OvRendering::LowRenderer::Camera::SetRotation(const OvMaths::FQuaternion & p_rotation)
{
	m_forward = p_rotation * OvMaths::FVector3(0.f, 0.f, 1.f);
	m_right = p_rotation * OvMaths::FVector3(1.f, 0.f, 0.f);
	m_up = p_rotation * OvMaths::FVector3(0.f, 1.f, 0.f);
}

OvMaths::FMatrix4 OvRendering::LowRenderer::Camera::CalculateProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight) const
{
	return OvMaths::FMatrix4::CreatePerspective(m_fov, static_cast<float>(p_windowWidth) / static_cast<float>(p_windowHeight), m_near, m_far);
}

OvMaths::FMatrix4 OvRendering::LowRenderer::Camera::CalculateViewMatrix(const OvMaths::FVector3& p_position) const
{
	return OvMaths::FMatrix4::CreateView
	(
		p_position.x, p_position.y, p_position.z,													// Position
		p_position.x + m_forward.x, p_position.y + m_forward.y, p_position.z + m_forward.z,			// LookAt (Position + Forward)
		m_up.x, m_up.y, m_up.z																		// Up Vector
	);
}

void OvRendering::LowRenderer::Camera::UpdateCameraVectors()
{
	m_forward.x = std::cos(m_yaw * 0.0174f) * std::cos(m_pitch * 0.0174f);
	m_forward.y = std::sin(m_pitch * 0.0174f);
	m_forward.z = std::sin(m_yaw * 0.0174f) * std::cos(m_pitch * 0.0174f);

	m_forward	= OvMaths::FVector3::Normalize(m_forward);
	m_right		= OvMaths::FVector3::Normalize(OvMaths::FVector3::Cross(m_forward, OvMaths::FVector3(0.0f, 1.0f, 0.0f)));
	m_up		= OvMaths::FVector3::Normalize(OvMaths::FVector3::Cross(m_right, m_forward));
}