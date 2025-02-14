/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <string>
#include <functional>


namespace OvTools::Utils
{
	/*
	* Handle random numbers generation
	*/
	class String
	{
	public:
		enum ETrimType
		{
			Leading = 1,
			Trailing = 2,
			TwoWays = 4 // Leading and Trailing
		};

		/**
		* Disabled constructor
		*/
		String() = delete;

		/**
		* Replace the first occurence of p_from by p_to in the given string
		* @param p_target (The string to modify)
		* @param p_from
		* @param p_to
		*/
		static bool Replace(std::string& p_target, const std::string& p_from, const std::string& p_to);

		/**
		* Replace every occurence of p_from by p_to in the given string
		* @param p_target (The string to modify)
		* @param p_from
		* @param p_to
		*/
		static void ReplaceAll(std::string& p_target, const std::string& p_from, const std::string& p_to);

        /**
        * Generate a unique string satisfying the availability predicate
        * @param p_source
        * @param p_isAvailable (A callback that must returning true if the input string is available)
        */
        static std::string GenerateUnique(const std::string& p_source, std::function<bool(std::string)> p_isAvailable);

		/**
		* Trim whitespaces from a string in a desired direction (Leading, Trailing or both as TwoWays).
		* @param p_str String to trim
		* @param p_trimType The desired trim direction, default is Trailing
		*/
		static void Trim(std::string& p_str, const ETrimType p_trimType = ETrimType::Trailing);

	private:
		/**
		* Leading trim of whitespaces from a string.
		* param p_str String to trim
		*/
		static void LeadingTrim(std::string& p_str);

		/**
		* Trailing trim of whitespaces from a string.
		* param p_str String to trim
		*/
		static void TrailingTrim(std::string& p_str);
	};
}