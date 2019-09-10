/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <OvMaths/FMatrix4.h>
#include <OvMaths/FTransform.h>

#include "OvRendering/API/Export.h"
#include "OvRendering/Geometry/BoundingSphere.h"

namespace OvRendering::Data
{
	/**
	* Mathematic representation of a 3D frustum
	*/
	class API_OVRENDERING Frustum
	{
	public:
		/**
		* Update frustum values
		* @param p_viewProjection
		*/ 
		void CalculateFrustum(const OvMaths::FMatrix4& p_viewProjection);

		/**
		* Returns true if the given point is in frustum
		* @param p_x
		* @param p_y
		* @param p_z
		*/
		bool PointInFrustum(float p_x, float p_y, float p_z) const;

		/**
		* Returns true if the given sphere is in frustum
		* @param p_x
		* @param p_y
		* @param p_z
		* @param p_radius
		*/
		bool SphereInFrustum(float p_x, float p_y, float p_z, float p_radius) const;

		/**
		* Returns true if the given cube is in frustum
		* @param p_x
		* @param p_y
		* @param p_z
		* @param p_size
		*/
		bool CubeInFrustum(float p_x, float p_y, float p_z, float p_size) const;

		/**
		* Returns true if the given bouding sphere is in frustum
		* @param p_boundingSphere
		* @param p_transform
		*/
		bool BoundingSphereInFrustum(const OvRendering::Geometry::BoundingSphere& p_boundingSphere, const OvMaths::FTransform& p_transform) const;

	private:
		float m_frustum[6][4];
	};
}