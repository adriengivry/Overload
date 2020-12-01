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
	};
}