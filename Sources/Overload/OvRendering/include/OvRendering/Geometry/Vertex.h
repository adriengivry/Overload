/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#pragma once

namespace OvRendering::Geometry
{
	/**
	* Data structure that defines the geometry of a vertex
	*/
	struct Vertex
	{
		float position[3];
		float texCoords[2];
		float normals[3];
		float tangent[3];
		float bitangent[3];
	};
}