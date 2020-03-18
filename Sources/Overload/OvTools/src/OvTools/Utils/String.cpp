/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvTools/Utils/String.h"

bool OvTools::Utils::String::Replace(std::string & p_target, const std::string & p_from, const std::string & p_to)
{
    size_t start_pos = p_target.find(p_from);

    if (start_pos != std::string::npos)
    {
        p_target.replace(start_pos, p_from.length(), p_to);
        return true;
    }

    return false;
}

void OvTools::Utils::String::ReplaceAll(std::string& p_target, const std::string& p_from, const std::string& p_to)
{
    if (p_from.empty()) return;

    size_t start_pos = 0;
    while ((start_pos = p_target.find(p_from, start_pos)) != std::string::npos)
    {
        p_target.replace(start_pos, p_from.length(), p_to);
        start_pos += p_to.length();
    }
}