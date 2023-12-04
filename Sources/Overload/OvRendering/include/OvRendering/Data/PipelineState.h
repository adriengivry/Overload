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
		Settings::ERasterizationMode rasterizationMode = Settings::ERasterizationMode::FILL;
		float rasterizationLinesWidth = 1.0f;

		// Color writing mask
		std::bitset<4> colorWriting = ~0;

		// Capability state
		bool depthWriting = true;
		bool blending = false;
		bool culling = true;
		bool dither = false;
		bool polygonOffsetFill = false;
		bool sampleAlphaToCoverage = false;
		bool depthTest = true;
		bool scissorTest = false;
		bool stencilTest = false;
		bool multisample = true;

		// Clear
		float clearR = 0.0f;
		float clearG = 0.0f;
		float clearB = 0.0f;
		float clearA = 0.0f;

		// Stencil func state
		Settings::EComparaisonAlgorithm stencilAlgorithm = Settings::EComparaisonAlgorithm::ALWAYS;
		int32_t stencilAlgorithmReference = 0;
		uint32_t stencilAlgorithmMask = 0xFF;

		uint32_t stencilMask = 0xFF;

		Settings::EOperation stencilFailOp = Settings::EOperation::KEEP;
		Settings::EOperation depthFailOp = Settings::EOperation::KEEP;
		Settings::EOperation bothPassOp = Settings::EOperation::KEEP;

		// Depth state
		Settings::EComparaisonAlgorithm depthAlgorithm = Settings::EComparaisonAlgorithm::LESS;

		// Culling state
		Settings::ECullFace cullFace = Settings::ECullFace::BACK;

		// View state
		uint32_t viewportX = 0;
		uint32_t viewportY = 0;
		uint32_t viewportW = 0;
		uint32_t viewportH = 0;
	};
}
