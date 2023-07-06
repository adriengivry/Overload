/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvMaths/FVector4.h>

namespace OvUI::Types
{
	/**
	* @brief Data structure that contains color information in a 0.f to 1.f float format
	*/
	struct Color
	{
		Color(float p_r = 1.0f, float p_g = 1.0f, float p_b = 1.0f, float p_a = 1.0f);

		float r;
		float g;
		float b;
		float a;

		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color White;
		static const Color Black;
		static const Color Grey;
		static const Color Yellow;
		static const Color Cyan;
		static const Color Magenta;

		/**
		* @brief Returns a representation of the color as a OvMaths::FVector4 having (r,g,b,a) order
		* @returns OvMaths::FVector4 representing the color
		*/
		OvMaths::FVector4 ToVector4();

		/**
		* @brief Compares two colors, returns true if they are identical
		* @param p_other
		* @returns true if equal, false otherwise
		*/
		bool operator==(const Color& p_other) const;

		/**
		* @brief Compares a color with a OvMaths::FVector4 representing a color, returns true if they are identical
		* @param p_other
		* @returns true if equal, false otherwise
		*/
		bool operator==(const OvMaths::FVector4& p_other) const;

		/**
		* @brief Compares two colors, returns true if they are different
		* @param p_other
		* @returns true if not equal, false otherwise
		*/
		bool operator!=(const Color& p_other) const;

		/**
		* @brief Set a color from a OvMaths::FVector4 representing a color.
		* @param p_other
		* @returns The new color represented by p_other, the same color otherwise.
		*/
		Color& operator=(const OvMaths::FVector4& p_other)
		{
			if (*this == p_other)
				return *this;

			r = p_other.x;
			g = p_other.y;
			b = p_other.z;
			a = p_other.w;

			return *this;
		}
	};
}