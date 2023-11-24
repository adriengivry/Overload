/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <iostream>
#include <sstream>

#include "OvTools/Utils/Format.h"

std::string OvTools::Utils::Format::ReadableNumber(const uint64_t number)
{
    std::stringstream ss;
    ss.imbue(std::locale(std::cout.getloc(), new DefaultNumberFormat));
    ss << number;

    return ss.str();
}