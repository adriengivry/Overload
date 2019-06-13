/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <OvMaths/FVector3.h>
#include <OvMaths/FMatrix4.h>
#include <OvMaths/FQuaternion.h>

#include "OvRendering/API/Export.h"

namespace OvRendering::LowRenderer
{
	/**
	* Represents a camera. Wraps projection and view calculation based on applied rotation and the given positions
	*/
	class API_OVRENDERING Camera
	{
	public:
		/**
		* Constructor
		*/
		Camera();

		/**
		* Returns the projection matrix
		* @param p_windowWidth
		* @param p_windowHeight
		*/
		OvMaths::FMatrix4 GetProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight) const;

		/**
		* Returns the view matrix
		* @param p_position
		*/
		OvMaths::FMatrix4 GetViewMatrix(const OvMaths::FVector3& p_position) const;

		/**
		* Returns the forward vector of the camera
		*/
		const OvMaths::FVector3& GetForward() const;

		/**
		* Returns the up vector of the camera
		*/
		const OvMaths::FVector3& GetUp() const;

		/**
		* Returns the right vector of the camera
		*/
		const OvMaths::FVector3& GetRight() const;

		/**
		* Returns the yaw of the camera
		*/
		float GetYaw() const;

		/**
		* Returns the pitch of the camera
		*/
		float GetPitch() const;

		/**
		* Returns the roll of the camera
		*/
		float GetRoll() const;

		/**
		* Returns the fov of the camera
		*/
		float GetFov() const;

		/**
		* Returns the near of the camera
		*/
		float GetNear() const;

		/**
		* Returns the far of the camera
		*/
		float GetFar() const;

		/**
		* Returns the clear color of the camera
		*/
		const OvMaths::FVector3& GetClearColor() const;

		/**
		* Sets the yaw of the camera to the given value
		* @param p_value
		*/
		void SetYaw(float p_value);

		/**
		* Sets the pitch of the camera to the given value
		* @param p_value
		*/
		void SetPitch(float p_value);

		/**
		* Sets the roll of the camera to the given value
		* @param p_value
		*/
		void SetRoll(float p_value);

		/**
		* Sets the fov of the camera to the given value
		* @param p_value
		*/
		void SetFov(float p_value);

		/**
		* Sets the near of the camera to the given value
		* @param p_value
		*/
		void SetNear(float p_value);

		/**
		* Sets the far of the camera to the given value
		* @param p_value
		*/
		void SetFar(float p_value);

		/**
		* Sets the clear color of the camera to the given value
		* @param p_value
		*/
		void SetClearColor(const OvMaths::FVector3& p_clearColor);

		/**
		* Sets the rotation to the camera with a quaternion
		* @param p_rotation
		*/
		void SetRotation(const OvMaths::FQuaternion& p_rotation);

	private:
		void UpdateCameraVectors();

	private:
		OvMaths::FVector3 m_forward;
		OvMaths::FVector3 m_up;
		OvMaths::FVector3 m_right;

		float m_yaw;
		float m_pitch;
		float m_roll;

		float m_fov;
		float m_near;
		float m_far;

		OvMaths::FVector3 m_clearColor;
	};
}