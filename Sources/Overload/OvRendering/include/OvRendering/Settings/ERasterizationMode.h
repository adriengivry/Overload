/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once



namespace OvRendering::Settings
{
	/**
	* OpenGL rasterization mode enum wrapper
	*/
	enum class ERasterizationMode
	{
		POINT	= 0x1B00, // Polygon vertices that are marked as the start of a boundary edge are drawn as points. Point attributes such as GL_POINT_SIZE and GL_POINT_SMOOTH control the rasterization of the points. Polygon rasterization attributes other than GL_POLYGON_MODE have no effect.
		LINE	= 0x1B01, // Boundary edges of the polygon are drawn as line segments. Line attributes such as GL_LINE_WIDTH and GL_LINE_SMOOTH control the rasterization of the lines. Polygon rasterization attributes other than GL_POLYGON_MODE have no effect.
		FILL	= 0x1B02  // The interior of the polygon is filled. Polygon attributes such as GL_POLYGON_SMOOTH control the rasterization of the polygon.
	};
}