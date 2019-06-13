/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include <OvUI/Widgets/Drags/DragFloat.h>
#include <OvUI/Plugins/DataDispatcher.h>

#include "OvCore/ECS/Components/CCamera.h"
#include "OvCore/ECS/Actor.h"

OvCore::ECS::Components::CCamera::CCamera(ECS::Actor& p_owner) : AComponent(p_owner)
{
	/* Default clear color for the CCamera (Different from Camera default clear color) */
	SetClearColor({ 0.1921569f, 0.3019608f, 0.4745098f });

	/* Handle the collider scaling when the actor transform changes */
	m_transformNotificationHandlerID = owner.transform.GetFTransform().Notifier.AddNotificationHandler([this](auto p_notification)
	{
		if (p_notification == OvMaths::Internal::TransformNotifier::ENotification::TRANSFORM_CHANGED)
		{
			m_camera.SetRotation(owner.transform.GetWorldRotation());
		}
	});
}

OvCore::ECS::Components::CCamera::~CCamera()
{
	owner.transform.GetFTransform().Notifier.RemoveNotificationHandler(m_transformNotificationHandlerID);
}

std::string OvCore::ECS::Components::CCamera::GetName()
{
	return "Camera";
}


OvMaths::FMatrix4 OvCore::ECS::Components::CCamera::GetProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight)
{
	return m_camera.GetProjectionMatrix(p_windowWidth, p_windowHeight);
}

OvMaths::FMatrix4 OvCore::ECS::Components::CCamera::GetViewMatrix()
{
	return m_camera.GetViewMatrix(owner.transform.GetWorldPosition());
}

void OvCore::ECS::Components::CCamera::SetFov(float p_value)
{
	m_camera.SetFov(p_value);
}

void OvCore::ECS::Components::CCamera::SetNear(float p_value)
{
	m_camera.SetNear(p_value);
}

void OvCore::ECS::Components::CCamera::SetFar(float p_value)
{
	m_camera.SetFar(p_value);
}

float OvCore::ECS::Components::CCamera::GetFov() const
{
	return m_camera.GetFov();
}

float OvCore::ECS::Components::CCamera::GetNear() const
{
	return m_camera.GetNear();
}

float OvCore::ECS::Components::CCamera::GetFar() const
{
	return m_camera.GetFar();
}

const OvMaths::FVector3 & OvCore::ECS::Components::CCamera::GetClearColor() const
{
	return m_camera.GetClearColor();
}

void OvCore::ECS::Components::CCamera::SetClearColor(const OvMaths::FVector3 & p_clearColor)
{
	m_camera.SetClearColor(p_clearColor);
}

OvRendering::LowRenderer::Camera & OvCore::ECS::Components::CCamera::GetCamera()
{
	return m_camera;
}

void OvCore::ECS::Components::CCamera::OnSerialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	OvCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "fov", m_camera.GetFov());
	OvCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "near", m_camera.GetNear());
	OvCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "far", m_camera.GetFar());
	OvCore::Helpers::Serializer::SerializeVec3(p_doc, p_node, "clear_color", m_camera.GetClearColor());
}

void OvCore::ECS::Components::CCamera::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	m_camera.SetFov(OvCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "fov"));
	m_camera.SetNear(OvCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "near"));
	m_camera.SetFar(OvCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "far"));
	SetClearColor(OvCore::Helpers::Serializer::DeserializeVec3(p_doc, p_node, "clear_color"));
}

void OvCore::ECS::Components::CCamera::OnInspector(OvUI::Internal::WidgetContainer& p_root)
{
	OvCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Field of view", std::bind(&CCamera::GetFov, this), std::bind(&CCamera::SetFov, this, std::placeholders::_1));
	OvCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Near", std::bind(&CCamera::GetNear, this), std::bind(&CCamera::SetNear, this, std::placeholders::_1));
	OvCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Far", std::bind(&CCamera::GetFar, this), std::bind(&CCamera::SetFar, this, std::placeholders::_1));
	OvCore::Helpers::GUIDrawer::DrawColor(p_root, "Clear color", [this]() {return reinterpret_cast<const OvUI::Types::Color&>(GetClearColor()); }, [this](OvUI::Types::Color p_color) { SetClearColor({ p_color.r, p_color.g, p_color.b }); }, false);
}
