/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <OvTools/Filesystem/tinyxml2.h>

#include "OvCore/API/Export.h"
#include "OvCore/Helpers/Serializer.h"

namespace OvCore::API
{
    /**
    * ISerializable is an interface for any class that can be serialized
    */
    class API_OVCORE ISerializable
    {
    public:
        /**
        * Called when the serialization is asked
        * @param p_doc
        * @param p_node
        */
        virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) = 0;

        /**
        * Called when the deserialization is asked
        * @param p_doc
        * @param p_node
        */
        virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) = 0;
    };
}