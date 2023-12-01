/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include <GL/glew.h>
#include <stdint.h>

#include <OvDebug/Logger.h>

#include "OvRendering/Context/Driver.h"

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

void InitGlew()
{
	const GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		std::string message = "Error Init GLEW: ";
		std::string glewError = reinterpret_cast<const char*>(glewGetErrorString(error));
		OVLOG_INFO(message + glewError);
	}
}

OvRendering::Context::Driver::Driver(const Settings::DriverSettings& p_driverSettings)
{
	InitGlew();
	
	m_isActive = true;

	if (p_driverSettings.debugMode)
	{
		GLint flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
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
}

bool OvRendering::Context::Driver::IsActive() const
{
	return m_isActive;
}

void OvRendering::Context::Driver::SetClearColor(float p_red, float p_green, float p_blue, float p_alpha) const
{
	glClearColor(p_red, p_green, p_blue, p_alpha);
}

void OvRendering::Context::Driver::Clear(bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer) const
{
	GLbitfield clearMask = 0;

	if (p_colorBuffer) clearMask |= GL_COLOR_BUFFER_BIT;
	if (p_depthBuffer) clearMask |= GL_DEPTH_BUFFER_BIT;
	if (p_stencilBuffer) clearMask |= GL_STENCIL_BUFFER_BIT;

	if (clearMask != 0)
	{
		if ((clearMask & GL_STENCIL_BUFFER_BIT) != 0)
		{
			glStencilMask(~0); // Allow writing operations, which effectively allow clearing the stencil buffer
		}

		glDisable(GL_SCISSOR_TEST); // Necessary to avoid scissor test to prevent us from properly clearing the buffer
		glClear(clearMask);
	}
}

void OvRendering::Context::Driver::SetRasterizationLinesWidth(float p_width) const
{
	glLineWidth(p_width);
}

void OvRendering::Context::Driver::SetRasterizationMode(Settings::ERasterizationMode p_rasterizationMode) const
{
	glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(p_rasterizationMode));
}

void OvRendering::Context::Driver::SetCapability(Settings::ERenderingCapability p_capability, bool p_value) const
{
	(p_value ? glEnable : glDisable)(static_cast<GLenum>(p_capability));
}

bool OvRendering::Context::Driver::GetCapability(Settings::ERenderingCapability p_capability) const
{
	return glIsEnabled(static_cast<GLenum>(p_capability));
}

void OvRendering::Context::Driver::SetStencilAlgorithm(Settings::EComparaisonAlgorithm p_algorithm, int32_t p_reference, uint32_t p_mask) const
{
	glStencilFunc(static_cast<GLenum>(p_algorithm), p_reference, p_mask);
}

void OvRendering::Context::Driver::SetDepthAlgorithm(Settings::EComparaisonAlgorithm p_algorithm) const
{
	glDepthFunc(static_cast<GLenum>(p_algorithm));
}

void OvRendering::Context::Driver::SetStencilMask(uint32_t p_mask) const
{
	glStencilMask(p_mask);
}

void OvRendering::Context::Driver::SetStencilOperations(Settings::EOperation p_stencilFail, Settings::EOperation p_depthFail, Settings::EOperation p_bothPass) const
{
	glStencilOp(static_cast<GLenum>(p_stencilFail), static_cast<GLenum>(p_depthFail), static_cast<GLenum>(p_bothPass));
}

void OvRendering::Context::Driver::SetCullFace(Settings::ECullFace p_cullFace) const
{
	glCullFace(static_cast<GLenum>(p_cullFace));
}

void OvRendering::Context::Driver::SetDepthWriting(bool p_enable) const
{
	glDepthMask(p_enable);
}

void OvRendering::Context::Driver::SetColorWriting(bool p_enableRed, bool p_enableGreen, bool p_enableBlue, bool p_enableAlpha) const
{
	glColorMask(p_enableRed, p_enableGreen, p_enableBlue, p_enableAlpha);
}

void OvRendering::Context::Driver::SetColorWriting(bool p_enable) const
{
	glColorMask(p_enable, p_enable, p_enable, p_enable);
}

void OvRendering::Context::Driver::SetViewPort(uint32_t x, uint32_t y, uint32_t width, uint32_t height) const
{
	glViewport(x, y, width, height);
}

void OvRendering::Context::Driver::ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, Settings::EPixelDataFormat format, Settings::EPixelDataType type, void* data) const
{
	glReadPixels(x, y, width, height, static_cast<GLenum>(format), static_cast<GLenum>(type), data);
}

bool OvRendering::Context::Driver::GetBool(uint32_t p_parameter) const
{
	GLboolean result;
	glGetBooleanv(p_parameter, &result);
	return static_cast<bool>(result);
}

bool OvRendering::Context::Driver::GetBool(uint32_t p_parameter, uint32_t p_index) const
{
	GLboolean result;
	glGetBooleani_v(p_parameter, p_index, &result);
	return static_cast<bool>(result);
}

int OvRendering::Context::Driver::GetInt(uint32_t p_parameter) const
{
	GLint result;
	glGetIntegerv(p_parameter, &result);
	return static_cast<int>(result);
}

int OvRendering::Context::Driver::GetInt(uint32_t p_parameter, uint32_t p_index) const
{
	GLint result;
	glGetIntegeri_v(p_parameter, p_index, &result);
	return static_cast<int>(result);
}

float OvRendering::Context::Driver::GetFloat(uint32_t p_parameter) const
{
	GLfloat result;
	glGetFloatv(p_parameter, &result);
	return static_cast<float>(result);
}

float OvRendering::Context::Driver::GetFloat(uint32_t p_parameter, uint32_t p_index) const
{
	GLfloat result;
	glGetFloati_v(p_parameter, p_index, &result);
	return static_cast<float>(result);
}

double OvRendering::Context::Driver::GetDouble(uint32_t p_parameter) const
{
	GLdouble result;
	glGetDoublev(p_parameter, &result);
	return static_cast<double>(result);
}

double OvRendering::Context::Driver::GetDouble(uint32_t p_parameter, uint32_t p_index) const
{
	GLdouble result;
	glGetDoublei_v(p_parameter, p_index, &result);
	return static_cast<double>(result);
}

int64_t OvRendering::Context::Driver::GetInt64(uint32_t p_parameter) const
{
	GLint64 result;
	glGetInteger64v(p_parameter, &result);
	return static_cast<int64_t>(result);
}

int64_t OvRendering::Context::Driver::GetInt64(uint32_t p_parameter, uint32_t p_index) const
{
	GLint64 result;
	glGetInteger64i_v(p_parameter, p_index, &result);
	return static_cast<int64_t>(result);
}

std::string OvRendering::Context::Driver::GetString(uint32_t p_parameter) const
{
	const GLubyte* result = glGetString(p_parameter);
	return result ? reinterpret_cast<const char*>(result) : std::string();
}

std::string OvRendering::Context::Driver::GetString(uint32_t p_parameter, uint32_t p_index) const
{
	const GLubyte* result = glGetStringi(p_parameter, p_index);
	return result ? reinterpret_cast<const char*>(result) : std::string();
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

// TODO: Rename to something that is more explicit
void OvRendering::Context::Driver::UpdateStateMask()
{
	using namespace OvRendering::Settings;

	GLboolean colorWriteMask[4];
	glGetBooleanv(GL_COLOR_WRITEMASK, colorWriteMask);

	m_stateMask.depthWriting = GetBool(GL_DEPTH_WRITEMASK);
	m_stateMask.colorWriting = colorWriteMask[0];
	m_stateMask.blendable = GetCapability(ERenderingCapability::BLEND);
	m_stateMask.culling = GetCapability(ERenderingCapability::CULL_FACE);
	m_stateMask.depthTest = GetCapability(ERenderingCapability::DEPTH_TEST);

	ECullFace cullFace = static_cast<ECullFace>(GetInt(GL_CULL_FACE));
	m_stateMask.backfaceCulling = cullFace == ECullFace::FRONT_AND_BACK || cullFace == ECullFace::BACK;
	m_stateMask.frontfaceCulling = cullFace == ECullFace::FRONT_AND_BACK || cullFace == ECullFace::FRONT;
}

const OvRendering::Data::StateMask OvRendering::Context::Driver::GetStateMask() const
{
	return m_stateMask;
}

void OvRendering::Context::Driver::ApplyStateMask(Data::StateMask p_mask)
{
	auto current = m_stateMask.mask;
	auto target = p_mask.mask;

	if (target != current)
	{
		using namespace OvRendering::Settings;
		if ((target & 0x01) != (current & 0x01))	SetDepthWriting(target & 0x01);
		if ((target & 0x02) != (current & 0x02))	SetColorWriting(target & 0x02);
		if ((target & 0x04) != (current & 0x04))	SetCapability(ERenderingCapability::BLEND, target & 0x04);
		if ((target & 0x08) != (current & 0x08))	SetCapability(ERenderingCapability::CULL_FACE, target & 0x8);
		if ((target & 0x10) != (current & 0x10))	SetCapability(ERenderingCapability::DEPTH_TEST, target & 0x10);
		if ((target & 0x08) && ((target & 0x20) != (current & 0x20) || (target & 0x40) != (current & 0x40)))
		{
			int backBit = target & 0x20;
			int frontBit = target & 0x40;
			SetCullFace(backBit && frontBit ? ECullFace::FRONT_AND_BACK : (backBit ? ECullFace::BACK : ECullFace::FRONT));
		}

		m_stateMask.mask = p_mask.mask;
	}
}
