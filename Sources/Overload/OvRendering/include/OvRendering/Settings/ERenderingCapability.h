/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once



namespace OvRendering::Settings
{
	/**
	* OpenGL rendering capability enum wrapper
	*/
	enum class ERenderingCapability
	{
		BLEND						= 0x0BE2, // If enabled, blend the computed fragment color values with the values in the color buffers. See glBlendFunc.
		CULL_FACE					= 0x0B44, // If enabled, cull polygons based on their winding in window coordinates.
		DEPTH_TEST					= 0x0B71, // If enabled, do depth comparisons and update the depth buffer. Note that even if the depth buffer exists and the depth mask is non-zero, the depth buffer is not updated if the depth test is disabled.
		DITHER						= 0x0BD0, // If enabled, dither color components or indices before they are written to the color buffer.
		POLYGON_OFFSET_FILL			= 0x8037, // If enabled, an offset is added to depth values of a polygon's fragments produced by rasterization.
		SAMPLE_ALPHA_TO_COVERAGE	= 0x809E, // If enabled, compute a temporary coverage value where each bit is determined by the alpha value at the corresponding sample location. The temporary coverage value is then ANDed with the fragment coverage value.
		SAMPLE_COVERAGE				= 0x80A0, // If enabled, the fragment's coverage is ANDed with the temporary coverage value. If GL_SAMPLE_COVERAGE_INVERT is set to GL_TRUE, invert the coverage value.
		SCISSOR_TEST				= 0x0C11, // If enabled, discard fragments that are outside the scissor rectangle.
		STENCIL_TEST				= 0x0B90, // If enabled, do stencil testing and update the stencil buffer.
		MULTISAMPLE					= 0x809D  // If enabled, use multiple fragment samples in computing the final color of a pixel.	
	};
}