/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include <string>

#include "OvTools/API/Export.h"

namespace OvTools::Utils
{
	/*
	* Handle random numbers generation
	*/
	class API_OVTOOLS String
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
	};
}