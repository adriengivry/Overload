/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvRendering/Entities/Camera.h>
#include <OvRendering/Features/DebugShapeRenderFeature.h>

#include <OvCore/ECS/Actor.h>
#include <OvCore/SceneSystem/SceneManager.h>
#include <OvCore/ECS/Components/CModelRenderer.h>
#include <OvCore/Resources/Material.h>
#include <OvCore/ECS/Components/CAmbientBoxLight.h>
#include <OvCore/ECS/Components/CAmbientSphereLight.h>
#include <OvCore/Rendering/SceneRenderer.h>

#include "OvEditor/Core/Context.h"

namespace OvEditor::Rendering
{
	/**
	* Draw a gizmo
	*/
	class GizmoRenderFeature : public OvRendering::Features::ARenderFeature
	{
	public:
		/**
		* Render a gizmo at position
		* @param p_position
		* @param p_rotation
		* @param p_operation
		* @param p_pickable (Determine the shader to use to render the gizmo)
		* @param p_highlightedAxis (-1 to highlight no axis, 0 for X, 1 for Y, 2 for Z)
		*/
		void DrawGizmo(
			const OvMaths::FVector3& p_position,
			const OvMaths::FQuaternion& p_rotation,
			OvEditor::Core::EGizmoOperation p_operation,
			bool p_pickable,
			int p_highlightedAxis = -1
		);

	protected:
		GizmoRenderFeature(OvRendering::Core::CompositeRenderer& p_driver);

	private:
		OvCore::Resources::Material m_gizmoArrowMaterial;
		OvCore::Resources::Material m_gizmoBallMaterial;
		OvCore::Resources::Material m_gizmoPickingMaterial;
	};
}