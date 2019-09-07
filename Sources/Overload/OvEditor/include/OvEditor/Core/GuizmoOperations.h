/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <OvEditor/Core/EditorActions.h>

namespace OvEditor::Core
{
	/* Handle guizmo behaviours */
	class GuizmoOperations
	{
	public:
		/**
		* Starts the guizmo picking behaviour for the given target in the given direction
		* @param p_actor
		* @param p_direction
		* @param p_cameraPosition
		*/
		void StartPicking(OvCore::ECS::Actor& p_target, const OvMaths::FVector3& p_direction, const OvMaths::FVector3& p_cameraPosition);

		/**
		* Stops the guizmo picking behaviour
		*/
		void StopPicking();

		/**
		* Handle the translation behaviour
		* @param p_viewMatrix
		* @param p_projectionMatrix
		* @param p_viewSize
		*/
		void ApplyTranslation(const OvMaths::FMatrix4& p_viewMatrix, const OvMaths::FMatrix4& p_projectionMatrix, const OvMaths::FVector2& p_viewSize) const;

		/**
		* Set the given mouse position as the current mouse position and update the previous mouse position
		* @param p_mousePosition
		*/
		void SetCurrentMouse(const OvMaths::FVector2& p_mousePosition);

		/**
		* Returns true if the guizmo is currently picked
		*/
		bool IsPicking() const;

	private:
		bool m_firstMouse = true;
		float m_distanceToActor = 0.0f;
		OvCore::ECS::Actor* m_target;
		OvMaths::FVector3 m_direction;
		OvMaths::FVector2 m_previousMouse;
		OvMaths::FVector2 m_currentMouse;
	};
}
