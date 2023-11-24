/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <locale>
#include <string>

namespace OvTools::Utils
{
	struct DefaultNumberFormat : std::numpunct<char>
	{
		// Comma separator between groups
		char do_thousands_sep() const { return ','; }
		// Group every 3 digits
		std::string do_grouping() const { return "\03"; }
	};

	class Format
	{
		Format() = delete;
		
	public:
		/**
		* @brief Transfrom a number into a more readable version
		* by grouping every 3 digits occurences together seperated by commas.
		* @param number Number to make more readable
		* @return A std::string representation of the number, more readable.
		*/
		static std::string ReadableNumber(const uint64_t number);
	};
}