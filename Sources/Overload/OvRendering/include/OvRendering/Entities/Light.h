/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <OvMaths/FVector3.h>
#include <OvMaths/FMatrix4.h>
#include <OvMaths/FTransform.h>

#include "OvRendering/API/Export.h"

namespace OvRendering::Entities
{
	/**
	* Data structure that can represent any type of light
	*/
	struct API_OVRENDERING Light
	{
		/**
		* Light types
		*/
		enum class Type { POINT, DIRECTIONAL, SPOT, AMBIENT_BOX, AMBIENT_SPHERE };

		/**
		* Create the light with the given transform and type
		* @param p_transform
		* @param p_type
		*/
		Light(OvMaths::FTransform& p_tranform, Type p_type);

		/**
		* Generate the light matrix, ready to send to the GPU
		*/
		OvMaths::FMatrix4 GenerateMatrix() const;

		OvMaths::FVector3	color		= { 1.f, 1.f, 1.f };
		float				intensity	= 1.f;
		float				constant	= 0.0f;
		float				linear		= 0.0f;
		float				quadratic	= 1.0f;
		float				cutoff		= 12.f;
		float				outerCutoff = 15.f;
		float				type		= 0.0f;

	protected:
		OvMaths::FTransform& m_transform;
	};
}