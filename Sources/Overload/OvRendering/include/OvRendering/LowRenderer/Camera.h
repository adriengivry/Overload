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
#include "OvRendering/Data/Frustum.h"

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
		* Cache the projection, view and frustum matrices
		* @param p_windowWidth
		* @param p_windowHeight
		* @param p_position
		*/
		void CacheMatrices(uint16_t p_windowWidth, uint16_t p_windowHeight, const OvMaths::FVector3& p_position);

		/**
		* Calculate and cache the result projection matrix
		* @param p_windowWidth
		* @param p_windowHeight
		*/
		void CacheProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight);

		/**
		* Calculate and cache the result view matrix
		* @param p_position
		*/
		void CacheViewMatrix(const OvMaths::FVector3& p_position);

		/**
		* Calculate and cache the result frustum.
		* This method should be called after projection and view matrices are cached.
		* @param p_view
		* @param p_projection
		*/
		void CacheFrustum(const OvMaths::FMatrix4& p_view, const OvMaths::FMatrix4& p_projection);

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
		* Returns the cached projection matrix
		*/
		const OvMaths::FMatrix4& GetProjectionMatrix() const;

		/**
		* Returns the cached view matrix
		*/
		const OvMaths::FMatrix4& GetViewMatrix() const;

		/**
		* Retursn the cached frustum
		*/
		const OvRendering::Data::Frustum& GetFrustum() const;

		/**
		* Returns true if the frustum culling for geometry is enabled
		*/
		bool HasFrustumGeometryCulling() const;

		/**
		* Returns true if the frustum culling for lights is enabled
		*/
		bool HasFrustumLightCulling() const;

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
		* Defines if the camera should apply frustum culling to geometry while rendering
		* @param p_enable
		*/
		void SetFrustumGeometryCulling(bool p_enable);

		/**
		* Defines if the camera should apply frustum culling to lights while rendering
		* @param p_enable
		*/
		void SetFrustumLightCulling(bool p_enable);

		/**
		* Sets the rotation to the camera with a quaternion
		* @param p_rotation
		*/
		void SetRotation(const OvMaths::FQuaternion& p_rotation);

	private:
		OvMaths::FMatrix4 CalculateProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight) const;
		OvMaths::FMatrix4 CalculateViewMatrix(const OvMaths::FVector3& p_position) const;
		void UpdateCameraVectors();

	private:
		OvRendering::Data::Frustum m_frustum;
		OvMaths::FMatrix4 m_viewMatrix;
		OvMaths::FMatrix4 m_projectionMatrix;

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

		bool m_frustumGeometryCulling;
		bool m_frustumLightCulling;
	};
}