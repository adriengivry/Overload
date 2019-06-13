/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

#include "OvUI/Widgets/InputFields/InputSingleScalar.h"

namespace OvUI::Widgets::InputFields
{
	/**
	* Input widget of type double
	*/
	class API_OVUI InputDouble : public InputSingleScalar<double>
	{
	public:
		/**
		* Constructor
		* @param p_defaultValue
		* @param p_step
		* @param p_fastStep
		* @param p_label
		* @param p_format
		* @param p_selectAllOnClick
		*/
		InputDouble
		(
			double p_defaultValue = 0.0,
			double p_step = 0.1,
			double p_fastStep = 0.0,
			const std::string& p_label = "",
			const std::string& p_format = "%.6f",
			bool p_selectAllOnClick = true
		);
	};
}