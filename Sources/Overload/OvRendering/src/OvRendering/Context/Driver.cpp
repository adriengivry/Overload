/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <GL/glew.h>
#include <stdint.h>

#include <OvDebug/Logger.h>
#include <OvDebug/Assertion.h>

#include "OvRendering/Context/Driver.h"
#include "OvRendering/Utils/Conversions.h"

constexpr GLenum ToGLEnum(OvRendering::Settings::ERasterizationMode value)
{
	return GL_POINT + static_cast<GLint>(value);
}

constexpr GLenum ToGLEnum(OvRendering::Settings::EComparaisonAlgorithm value)
{
	return GL_NEVER + static_cast<GLint>(value);
}

constexpr GLenum ToGLEnum(OvRendering::Settings::ECullFace value)
{
	switch (value)
	{
	case OvRendering::Settings::ECullFace::FRONT: return GL_FRONT;
	case OvRendering::Settings::ECullFace::BACK: return GL_BACK;
	case OvRendering::Settings::ECullFace::FRONT_AND_BACK: return GL_FRONT_AND_BACK;
	}

	static_assert(true);
	return {};
}

constexpr GLenum ToGLEnum(OvRendering::Settings::EOperation value)
{
	switch (value)
	{
	case OvRendering::Settings::EOperation::KEEP: return GL_KEEP;
	case OvRendering::Settings::EOperation::ZERO: return GL_ZERO;
	case OvRendering::Settings::EOperation::REPLACE: return GL_REPLACE;
	case OvRendering::Settings::EOperation::INCREMENT: return GL_INCR;
	case OvRendering::Settings::EOperation::INCREMENT_WRAP: return GL_INCR_WRAP;
	case OvRendering::Settings::EOperation::DECREMENT: return GL_DECR;
	case OvRendering::Settings::EOperation::DECREMENT_WRAP: return GL_DECR_WRAP;
	case OvRendering::Settings::EOperation::INVERT: return GL_INVERT;
	}

	static_assert(true);
	return {};
}

constexpr GLenum ToGLEnum(OvRendering::Settings::ERenderingCapability value)
{
	switch (value)
	{
	case OvRendering::Settings::ERenderingCapability::BLEND: return GL_BLEND;
	case OvRendering::Settings::ERenderingCapability::CULL_FACE: return GL_CULL_FACE;
	case OvRendering::Settings::ERenderingCapability::DEPTH_TEST: return GL_DEPTH_TEST;
	case OvRendering::Settings::ERenderingCapability::DITHER: return GL_DITHER;
	case OvRendering::Settings::ERenderingCapability::POLYGON_OFFSET_FILL: return GL_POLYGON_OFFSET_FILL;
	case OvRendering::Settings::ERenderingCapability::SAMPLE_ALPHA_TO_COVERAGE: return GL_SAMPLE_ALPHA_TO_COVERAGE;
	case OvRendering::Settings::ERenderingCapability::SAMPLE_COVERAGE: return GL_SAMPLE_COVERAGE;
	case OvRendering::Settings::ERenderingCapability::SCISSOR_TEST: return GL_SCISSOR_TEST;
	case OvRendering::Settings::ERenderingCapability::STENCIL_TEST: return GL_STENCIL_TEST;
	case OvRendering::Settings::ERenderingCapability::MULTISAMPLE: return GL_MULTISAMPLE;
	}

	static_assert(true);
	return {};
}

void GLDebugMessageCallback(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::string output;

	output += "OpenGL Debug Message:\n";
	output += "Debug message (" + std::to_string(id) + "): " + message + "\n";

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:				output += "Source: API";				break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		output += "Source: Window System";		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:	output += "Source: Shader Compiler";	break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:		output += "Source: Third Party";		break;
	case GL_DEBUG_SOURCE_APPLICATION:		output += "Source: Application";		break;
	case GL_DEBUG_SOURCE_OTHER:				output += "Source: Other";				break;
	}

	output += "\n";

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               output += "Type: Error";				break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: output += "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  output += "Type: Undefined Behaviour";	break;
	case GL_DEBUG_TYPE_PORTABILITY:         output += "Type: Portability";			break;
	case GL_DEBUG_TYPE_PERFORMANCE:         output += "Type: Performance";			break;
	case GL_DEBUG_TYPE_MARKER:              output += "Type: Marker";				break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          output += "Type: Push Group";			break;
	case GL_DEBUG_TYPE_POP_GROUP:           output += "Type: Pop Group";			break;
	case GL_DEBUG_TYPE_OTHER:               output += "Type: Other";				break;
	}

	output += "\n";

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:			output += "Severity: High";				break;
	case GL_DEBUG_SEVERITY_MEDIUM:			output += "Severity: Medium";			break;
	case GL_DEBUG_SEVERITY_LOW:				output += "Severity: Low";				break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:	output += "Severity: Notification";		break;
	}

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:			OVLOG_ERROR(output);	break;
	case GL_DEBUG_SEVERITY_MEDIUM:			OVLOG_WARNING(output);	break;
	case GL_DEBUG_SEVERITY_LOW:				OVLOG_INFO(output);		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:	OVLOG_INFO(output);			break;
	}
}

bool InitGlew()
{
	const GLenum error = glewInit();

	if (error != GLEW_OK)
	{
		std::string message = "Error Init GLEW: ";
		std::string glewError = reinterpret_cast<const char*>(glewGetErrorString(error));
		OVLOG_INFO(message + glewError);
		return false;
	}

	return true;
}

void SetClearColor(float p_red, float p_green, float p_blue, float p_alpha)
{
	glClearColor(p_red, p_green, p_blue, p_alpha);
}

void SetRasterizationLinesWidth(float p_width)
{
	glLineWidth(p_width);
}

void SetRasterizationMode(OvRendering::Settings::ERasterizationMode p_rasterizationMode)
{
	glPolygonMode(GL_FRONT_AND_BACK, ToGLEnum(p_rasterizationMode));
}

void SetCapability(OvRendering::Settings::ERenderingCapability p_capability, bool p_value)
{
	(p_value ? glEnable : glDisable)(ToGLEnum(p_capability));
}

bool GetCapability(OvRendering::Settings::ERenderingCapability p_capability)
{
	return glIsEnabled(ToGLEnum(p_capability));
}

void SetStencilAlgorithm(OvRendering::Settings::EComparaisonAlgorithm p_algorithm, int32_t p_reference, uint32_t p_mask)
{
	glStencilFunc(ToGLEnum(p_algorithm), p_reference, p_mask);
}

void SetDepthAlgorithm(OvRendering::Settings::EComparaisonAlgorithm p_algorithm)
{
	glDepthFunc(ToGLEnum(p_algorithm));
}

void SetStencilMask(uint32_t p_mask)
{
	glStencilMask(p_mask);
}

void SetStencilOperations(
	OvRendering::Settings::EOperation p_stencilFail,
	OvRendering::Settings::EOperation p_depthFail,
	OvRendering::Settings::EOperation p_bothPass
)
{
	glStencilOp(ToGLEnum(p_stencilFail), ToGLEnum(p_depthFail), ToGLEnum(p_bothPass));
}

void SetCullFace(OvRendering::Settings::ECullFace p_cullFace)
{
	glCullFace(ToGLEnum(p_cullFace));
}

void SetDepthWriting(bool p_enable)
{
	glDepthMask(p_enable);
}

void SetColorWriting(bool p_enableRed, bool p_enableGreen, bool p_enableBlue, bool p_enableAlpha)
{
	glColorMask(p_enableRed, p_enableGreen, p_enableBlue, p_enableAlpha);
}

void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

bool GetBool(uint32_t p_parameter)
{
	GLboolean result;
	glGetBooleanv(p_parameter, &result);
	return static_cast<bool>(result);
}

bool GetBool(uint32_t p_parameter, uint32_t p_index)
{
	GLboolean result;
	glGetBooleani_v(p_parameter, p_index, &result);
	return static_cast<bool>(result);
}

int GetInt(uint32_t p_parameter)
{
	GLint result;
	glGetIntegerv(p_parameter, &result);
	return static_cast<int>(result);
}

int GetInt(uint32_t p_parameter, uint32_t p_index)
{
	GLint result;
	glGetIntegeri_v(p_parameter, p_index, &result);
	return static_cast<int>(result);
}

float GetFloat(uint32_t p_parameter)
{
	GLfloat result;
	glGetFloatv(p_parameter, &result);
	return static_cast<float>(result);
}

float GetFloat(uint32_t p_parameter, uint32_t p_index)
{
	GLfloat result;
	glGetFloati_v(p_parameter, p_index, &result);
	return static_cast<float>(result);
}

double GetDouble(uint32_t p_parameter)
{
	GLdouble result;
	glGetDoublev(p_parameter, &result);
	return static_cast<double>(result);
}

double GetDouble(uint32_t p_parameter, uint32_t p_index)
{
	GLdouble result;
	glGetDoublei_v(p_parameter, p_index, &result);
	return static_cast<double>(result);
}

int64_t GetInt64(uint32_t p_parameter)
{
	GLint64 result;
	glGetInteger64v(p_parameter, &result);
	return static_cast<int64_t>(result);
}

int64_t GetInt64(uint32_t p_parameter, uint32_t p_index)
{
	GLint64 result;
	glGetInteger64i_v(p_parameter, p_index, &result);
	return static_cast<int64_t>(result);
}

std::string GetString(uint32_t p_parameter)
{
	const GLubyte* result = glGetString(p_parameter);
	return result ? reinterpret_cast<const char*>(result) : std::string();
}

std::string GetString(uint32_t p_parameter, uint32_t p_index)
{
	const GLubyte* result = glGetStringi(p_parameter, p_index);
	return result ? reinterpret_cast<const char*>(result) : std::string();
}

/**
* Very expensive! Call it once, and make sure you always keep track of state changes
*/
OvRendering::Data::PipelineState RetrieveOpenGLPipelineState()
{
	using namespace OvRendering::Settings;

	OvRendering::Data::PipelineState pso;

	// Rasterization
	pso.rasterizationMode = static_cast<ERasterizationMode>(GetInt(GL_POLYGON_MODE));
	pso.lineWidthPow2 = OvRendering::Utils::Conversions::FloatToPow2(GetFloat(GL_LINE_WIDTH));

	// Color write mask
	GLboolean colorWriteMask[4];
	glGetBooleanv(GL_COLOR_WRITEMASK, colorWriteMask);
	pso.colorWriting.r = colorWriteMask[0];
	pso.colorWriting.g = colorWriteMask[1];
	pso.colorWriting.b = colorWriteMask[2];
	pso.colorWriting.a = colorWriteMask[3];

	// Capability
	pso.depthWriting = GetBool(GL_DEPTH_WRITEMASK);
	pso.blending = GetBool(GL_BLEND);
	pso.culling = GetBool(GL_CULL_FACE);
	pso.dither = GetBool(GL_DITHER);
	pso.polygonOffsetFill = GetBool(GL_POLYGON_OFFSET_FILL);
	pso.sampleAlphaToCoverage = GetBool(GL_SAMPLE_ALPHA_TO_COVERAGE);
	pso.depthTest = GetBool(GL_DEPTH_TEST);
	pso.scissorTest = GetBool(GL_SCISSOR_TEST);
	pso.stencilTest = GetBool(GL_STENCIL_TEST);
	pso.multisample = GetBool(GL_MULTISAMPLE);

	// Stencil
	pso.stencilFuncOp = static_cast<EComparaisonAlgorithm>(GetInt(GL_STENCIL_FUNC));
	pso.stencilFuncRef = GetInt(GL_STENCIL_REF);
	pso.stencilFuncMask = static_cast<uint32_t>(GetInt(GL_STENCIL_VALUE_MASK));

	pso.stencilWriteMask = static_cast<uint32_t>(GetInt(GL_STENCIL_WRITEMASK));

	pso.stencilOpFail = static_cast<EOperation>(GetInt(GL_STENCIL_FAIL));
	pso.depthOpFail = static_cast<EOperation>(GetInt(GL_STENCIL_PASS_DEPTH_FAIL));
	pso.bothOpFail = static_cast<EOperation>(GetInt(GL_STENCIL_PASS_DEPTH_PASS));

	// Depth
	pso.depthFunc = static_cast<EComparaisonAlgorithm>(GetInt(GL_DEPTH_FUNC));

	// Culling
	pso.cullFace = static_cast<ECullFace>(GetInt(GL_CULL_FACE_MODE));

	return pso;
}

OvRendering::Context::Driver::Driver(const OvRendering::Settings::DriverSettings& p_driverSettings)
{
	OVASSERT(InitGlew(), "Failed to initialized Glew!");

	if (p_driverSettings.defaultPipelineState)
	{
		m_defaultPipelineState = p_driverSettings.defaultPipelineState.value();
	}
	
	if (p_driverSettings.debugMode)
	{
		GLint flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags); //TODO: That doesn't seem right, we should set the integerv not get it?
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(GLDebugMessageCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);

	m_pipelineState = RetrieveOpenGLPipelineState();
	SetPipelineState(m_defaultPipelineState);

	m_vendor = GetString(GL_VENDOR);
	m_hardware = GetString(GL_RENDERER);
	m_version = GetString(GL_VERSION);
	m_shadingLanguageVersion = GetString(GL_SHADING_LANGUAGE_VERSION);
}

void OvRendering::Context::Driver::SetViewport(uint32_t p_x, uint32_t p_y, uint32_t p_width, uint32_t p_height)
{
	::SetViewport(p_x, p_y, p_width, p_height);
}

void OvRendering::Context::Driver::Clear(
	bool p_colorBuffer,
	bool p_depthBuffer,
	bool p_stencilBuffer,
	OvTools::Utils::OptRef<const OvMaths::FVector4> p_color
) const
{
	GLbitfield clearMask = 0;

	if (p_colorBuffer && p_color)
	{
		const auto& color = p_color.value();
		glClearColor(color.x, color.y, color.z, color.w);
	}

	// TODO: Add the stencil write mask logic here instead of in the renderer

	if (p_colorBuffer) clearMask |= GL_COLOR_BUFFER_BIT;
	if (p_depthBuffer) clearMask |= GL_DEPTH_BUFFER_BIT;
	if (p_stencilBuffer) clearMask |= GL_STENCIL_BUFFER_BIT;

	if (clearMask != 0)
	{
		glClear(clearMask);
	}
}

void OvRendering::Context::Driver::ReadPixels(
	uint32_t p_x,
	uint32_t p_y,
	uint32_t p_width,
	uint32_t p_height,
	OvRendering::Settings::EPixelDataFormat p_format,
	OvRendering::Settings::EPixelDataType p_type,
	void* p_data
) const
{
	glReadPixels(p_x, p_y, p_width, p_height, static_cast<GLenum>(p_format), static_cast<GLenum>(p_type), p_data);
}

void OvRendering::Context::Driver::DrawElements(Settings::EPrimitiveMode p_primitiveMode, uint32_t p_indexCount) const
{
	glDrawElements(static_cast<GLenum>(p_primitiveMode), p_indexCount, GL_UNSIGNED_INT, nullptr);
}

void OvRendering::Context::Driver::DrawElementsInstanced(Settings::EPrimitiveMode p_primitiveMode, uint32_t p_indexCount, uint32_t p_instances) const
{
	glDrawElementsInstanced(static_cast<GLenum>(p_primitiveMode), p_indexCount, GL_UNSIGNED_INT, nullptr, p_instances);
}

void OvRendering::Context::Driver::DrawArrays(Settings::EPrimitiveMode p_primitiveMode, uint32_t p_vertexCount) const
{
	glDrawArrays(static_cast<GLenum>(p_primitiveMode), 0, p_vertexCount);
}

void OvRendering::Context::Driver::DrawArraysInstanced(Settings::EPrimitiveMode p_primitiveMode, uint32_t p_vertexCount, uint32_t p_instances) const
{
	glDrawArraysInstanced(static_cast<GLenum>(p_primitiveMode), 0, p_vertexCount, p_instances);
}

void OvRendering::Context::Driver::SetPipelineState(OvRendering::Data::PipelineState p_state)
{
	using namespace OvRendering::Settings;

	if (p_state.bits != m_pipelineState.bits)
	{
		auto& i = p_state;
		auto& c = m_pipelineState;

		// Rasterization
		if (i.rasterizationMode != c.rasterizationMode) SetRasterizationMode(i.rasterizationMode);
		if (i.lineWidthPow2 != c.lineWidthPow2) SetRasterizationLinesWidth(Utils::Conversions::Pow2toFloat(i.lineWidthPow2));

		if (i.colorWriting.mask != c.colorWriting.mask) SetColorWriting(i.colorWriting.r, i.colorWriting.g, i.colorWriting.b, i.colorWriting.a);
		if (i.depthWriting != c.depthWriting) SetDepthWriting(i.depthWriting);

		if (i.blending != c.blending) SetCapability(ERenderingCapability::BLEND, i.blending);
		if (i.culling != c.culling) SetCapability(ERenderingCapability::CULL_FACE, i.culling);
		if (i.dither != c.dither) SetCapability(ERenderingCapability::DITHER, i.dither);
		if (i.polygonOffsetFill != c.polygonOffsetFill) SetCapability(ERenderingCapability::POLYGON_OFFSET_FILL, i.polygonOffsetFill);
		if (i.sampleAlphaToCoverage != c.sampleAlphaToCoverage) SetCapability(ERenderingCapability::SAMPLE_ALPHA_TO_COVERAGE, i.sampleAlphaToCoverage);
		if (i.depthTest != c.depthTest) SetCapability(ERenderingCapability::DEPTH_TEST, i.depthTest);
		if (i.scissorTest != c.scissorTest) SetCapability(ERenderingCapability::SCISSOR_TEST, i.scissorTest);
		if (i.stencilTest != c.stencilTest) SetCapability(ERenderingCapability::STENCIL_TEST, i.stencilTest);
		if (i.multisample != c.multisample) SetCapability(ERenderingCapability::MULTISAMPLE, i.multisample);

		// Stencil algorithm
		if (i.stencilFuncOp != c.stencilFuncOp ||
			i.stencilFuncRef != c.stencilFuncRef ||
			i.stencilFuncMask != c.stencilFuncMask)
			SetStencilAlgorithm(i.stencilFuncOp, i.stencilFuncRef, i.stencilFuncMask);

		if (i.stencilWriteMask != c.stencilWriteMask) SetStencilMask(i.stencilWriteMask);
		if (i.stencilOpFail != c.stencilOpFail || i.depthOpFail != c.depthOpFail || i.bothOpFail != c.bothOpFail) SetStencilOperations(i.stencilOpFail, i.depthOpFail, i.bothOpFail);

		// Depth
		if (i.depthFunc != c.depthFunc) SetDepthAlgorithm(i.depthFunc);

		// Culling
		if (i.cullFace != c.cullFace) SetCullFace(i.cullFace);

		m_pipelineState = p_state;
	}
}

void OvRendering::Context::Driver::ResetPipelineState()
{
	SetPipelineState(m_defaultPipelineState);
}

const OvRendering::Data::PipelineState& OvRendering::Context::Driver::GetPipelineState() const
{
	return m_pipelineState;
}

OvRendering::Data::PipelineState OvRendering::Context::Driver::CreatePipelineState() const
{
	return m_defaultPipelineState;
}

std::string_view OvRendering::Context::Driver::GetVendor() const
{
	return m_vendor;
}

std::string_view OvRendering::Context::Driver::GetHardware() const
{
	return m_hardware;
}

std::string_view OvRendering::Context::Driver::GetVersion() const
{
	return m_version;
}

std::string_view OvRendering::Context::Driver::GetShadingLanguageVersion() const
{
	return m_shadingLanguageVersion;
}
