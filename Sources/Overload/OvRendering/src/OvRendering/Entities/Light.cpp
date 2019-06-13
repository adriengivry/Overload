/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvRendering/Entities/Light.h"

uint32_t Pack(uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3)
{
	return (c0 << 24) | (c1 << 16) | (c2 << 8) | c3;
}

uint32_t Pack(const OvMaths::FVector3& p_toPack)
{
	return Pack(static_cast<uint8_t>(p_toPack.x * 255.f), static_cast<uint8_t>(p_toPack.y * 255.f), static_cast<uint8_t>(p_toPack.z * 255.f), 0);
}

OvRendering::Entities::Light::Light(OvMaths::FTransform & p_tranform, Type p_type) : m_transform(p_tranform), type(static_cast<float>(p_type))
{
	
}

OvMaths::FMatrix4 OvRendering::Entities::Light::GenerateMatrix() const
{
	OvMaths::FMatrix4 result;

	auto position = m_transform.GetWorldPosition();
	result.data[0] = position.x;
	result.data[1] = position.y;
	result.data[2] = position.z;

	auto forward = m_transform.GetWorldForward();
	result.data[4] = forward.x;
	result.data[5] = forward.y;
	result.data[6] = forward.z;

	result.data[8] = static_cast<float>(Pack(color));

	result.data[12] = type;
	result.data[13] = cutoff;
	result.data[14] = outerCutoff;

	result.data[3] = constant;
	result.data[7] = linear;
	result.data[11] = quadratic;
	result.data[15] = intensity;

	return result;
}
