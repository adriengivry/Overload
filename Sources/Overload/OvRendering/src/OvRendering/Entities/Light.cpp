/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
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

float CalculateLuminosity(float p_constant, float p_linear, float p_quadratic, float p_intensity, float p_distance)
{
	auto attenuation = (p_constant + p_linear * p_distance + p_quadratic * (p_distance * p_distance));
	return (1.0f / attenuation) * std::abs(p_intensity);
}

float CalculatePointLightRadius(float p_constant, float p_linear, float p_quadratic, float p_intensity)
{
	constexpr float threshold = 1 / 255.0f;
	constexpr float step = 1.0f;

	float distance = 0.0f;

	#define TRY_GREATER(value)\
	else if (CalculateLuminosity(p_constant, p_linear, p_quadratic, p_intensity, value) > threshold)\
	{\
		distance = value;\
	}

	#define TRY_LESS(value, newValue)\
	else if (CalculateLuminosity(p_constant, p_linear, p_quadratic, p_intensity, value) < threshold)\
	{\
		distance = newValue;\
	}

	// Prevents infinite while true. If a light has a bigger radius than 10000 we ignore it and make it infinite
	if (CalculateLuminosity(p_constant, p_linear, p_quadratic, p_intensity, 1000.0f) > threshold)
	{
		return std::numeric_limits<float>::infinity();
	}
	TRY_LESS(20.0f, 0.0f)
	TRY_GREATER(750.0f)
	TRY_LESS(50.0f, 20.0f + step)
	TRY_LESS(100.0f, 50.0f + step)
	TRY_GREATER(500.0f)
	TRY_GREATER(250.0f)

	while (true)
	{
		if (CalculateLuminosity(p_constant, p_linear, p_quadratic, p_intensity, distance) < threshold) // If the light has a very low luminosity for the given distance, we consider the current distance as the light radius
		{
			return distance;
		}
		else
		{
			distance += step;
		}
	}
}

float CalculateAmbientBoxLightRadius(const OvMaths::FVector3& p_position, const OvMaths::FVector3& p_size)
{
	return OvMaths::FVector3::Distance(p_position, p_position + p_size);
}

float OvRendering::Entities::Light::GetEffectRange() const
{
	switch (static_cast<OvRendering::Entities::Light::Type>(static_cast<int>(type)))
	{
	case Type::POINT:
	case Type::SPOT:			return CalculatePointLightRadius(constant, linear, quadratic, intensity);
	case Type::AMBIENT_BOX:		return CalculateAmbientBoxLightRadius(m_transform.GetWorldPosition(), { constant, linear, quadratic });
	case Type::AMBIENT_SPHERE:	return constant;
	}

	return std::numeric_limits<float>::infinity();
}

const OvMaths::FTransform& OvRendering::Entities::Light::GetTransform() const
{
	return m_transform;
}
