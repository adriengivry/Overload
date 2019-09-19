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

void OvCore::ECS::Components::CCamera::SetFrustumGeometryCulling(bool p_enable)
{
	m_camera.SetFrustumGeometryCulling(p_enable);
}

void OvCore::ECS::Components::CCamera::SetFrustumLightCulling(bool p_enable)
{
	m_camera.SetFrustumLightCulling(p_enable);
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

bool OvCore::ECS::Components::CCamera::HasFrustumGeometryCulling() const
{
	return m_camera.HasFrustumGeometryCulling();
}

void OvCore::ECS::Components::CCamera::SetClearColor(const OvMaths::FVector3 & p_clearColor)
{
	m_camera.SetClearColor(p_clearColor);
}

bool OvCore::ECS::Components::CCamera::HasFrustumLightCulling() const
{
	return m_camera.HasFrustumLightCulling();
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
	OvCore::Helpers::Serializer::SerializeBoolean(p_doc, p_node, "frustum_geometry_culling", m_camera.HasFrustumGeometryCulling());
	OvCore::Helpers::Serializer::SerializeBoolean(p_doc, p_node, "frustum_light_culling", m_camera.HasFrustumLightCulling());
}

void OvCore::ECS::Components::CCamera::OnDeserialize(tinyxml2::XMLDocument & p_doc, tinyxml2::XMLNode * p_node)
{
	m_camera.SetFov(OvCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "fov"));
	m_camera.SetNear(OvCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "near"));
	m_camera.SetFar(OvCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "far"));
	m_camera.SetClearColor(OvCore::Helpers::Serializer::DeserializeVec3(p_doc, p_node, "clear_color"));
	m_camera.SetFrustumGeometryCulling(OvCore::Helpers::Serializer::DeserializeBoolean(p_doc, p_node, "frustum_geometry_culling"));
	m_camera.SetFrustumLightCulling(OvCore::Helpers::Serializer::DeserializeBoolean(p_doc, p_node, "frustum_light_culling"));
}

void OvCore::ECS::Components::CCamera::OnInspector(OvUI::Internal::WidgetContainer& p_root)
{
	OvCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Field of view", std::bind(&CCamera::GetFov, this), std::bind(&CCamera::SetFov, this, std::placeholders::_1));
	OvCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Near", std::bind(&CCamera::GetNear, this), std::bind(&CCamera::SetNear, this, std::placeholders::_1));
	OvCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Far", std::bind(&CCamera::GetFar, this), std::bind(&CCamera::SetFar, this, std::placeholders::_1));
	OvCore::Helpers::GUIDrawer::DrawColor(p_root, "Clear color", [this]() {return reinterpret_cast<const OvUI::Types::Color&>(GetClearColor()); }, [this](OvUI::Types::Color p_color) { SetClearColor({ p_color.r, p_color.g, p_color.b }); }, false);
	OvCore::Helpers::GUIDrawer::DrawBoolean(p_root, "Frustum Geometry Culling", std::bind(&CCamera::HasFrustumGeometryCulling, this), std::bind(&CCamera::SetFrustumGeometryCulling, this, std::placeholders::_1));
	OvCore::Helpers::GUIDrawer::DrawBoolean(p_root, "Frustum Light Culling", std::bind(&CCamera::HasFrustumLightCulling, this), std::bind(&CCamera::SetFrustumLightCulling, this, std::placeholders::_1));
}
