/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <cstdint>
#include <bitset>

#include "OvRendering/Settings/DriverSettings.h"
#include "OvRendering/Settings/ERenderingCapability.h"
#include "OvRendering/Settings/EPrimitiveMode.h"
#include "OvRendering/Settings/ERasterizationMode.h"
#include "OvRendering/Settings/EComparaisonAlgorithm.h"
#include "OvRendering/Settings/EOperation.h"
#include "OvRendering/Settings/ECullFace.h"
#include "OvRendering/Settings/ECullingOptions.h"
#include "OvRendering/Settings/EPixelDataFormat.h"
#include "OvRendering/Settings/EPixelDataType.h"

namespace OvRendering::Data
{
	/**
	* Represents the current state of the driver and allow for efficient context switches
	*/
	struct PipelineState
	{
		// Rasterization state
		Settings::ERasterizationMode rasterizationMode;
		float rasterizationLinesWidth;

		// Color writing mask
		std::bitset<4> colorWriting;

		// Capability state
		bool depthWriting;
		bool blending;
		bool culling;
		bool dither;
		bool polygonOffsetFill;
		bool sampleAlphaToCoverage;
		bool depthTest;
		bool scissorTest;
		bool stencilTest;
		bool multisample;

		// Clear
		float clearR;
		float clearG;
		float clearB;
		float clearA;

		// Stencil func state
		Settings::EComparaisonAlgorithm stencilAlgorithm;
		int32_t stencilAlgorithmReference;
		uint32_t stencilAlgorithmMask;

		uint32_t stencilMask;

		Settings::EOperation stencilFailOp;
		Settings::EOperation depthFailOp;
		Settings::EOperation bothPassOp;

		// Depth state
		Settings::EComparaisonAlgorithm depthAlgorithm;

		// Culling state
		Settings::ECullFace cullFace;

		// View state
		uint32_t viewportX;
		uint32_t viewportY;
		uint32_t viewportW;
		uint32_t viewportH;
	};
}
