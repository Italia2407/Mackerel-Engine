#include "Shader.h"

// Logging Headers
#include "LoggingSystem.h"
#include <format>

#include <iostream>
#include <fstream>
#include <string.h>

#include <vector>

namespace MCK::AssetType {
GLuint Shader::k_ProjectionShaderID = GL_ZERO;
GLuint Shader::k_PassthroughShaderID = GL_ZERO;

/**
 * Load & Compile OpenGL Shader from Source File.
 * 
 * \param a_FilePath: The Shader's File Path
 * \param a_ShaderType: The Shader's Type
 * \param o_ShaderID: The Output Shader's ID
 * \return Whether the Shader could be Loaded & Compiled
 */
bool Shader::loadShaderSource(std::string a_FilePath, GLuint a_ShaderType, GLuint& o_ShaderID)
{
	// Open Shader File
	std::ifstream shaderFile(a_FilePath.c_str(), std::ios::in);
	if (!shaderFile.good()) {
		shaderFile.close();

		Logger::log(std::format("Cannot Load Shader File", a_FilePath), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	std::vector<GLchar*> shaderFileLines;
	std::vector<GLint> shaderFileLineLengths;

	// Read each Line of the Shader File
	std::string shaderFileReadLine;
	while (std::getline(shaderFile, shaderFileReadLine))
	{
		// Length of Shader File Line Entry
		// 1 Extra Character needed for '\n'
		GLint shaderFileLineLength = (GLint)shaderFileReadLine.length() + 1;

		// Create new Line Entry for Shader Code
		GLchar* shaderFileLine = (GLchar*)std::calloc(shaderFileLineLength, sizeof(GLchar));
		strcpy_s(shaderFileLine, shaderFileLineLength * sizeof(GLchar), shaderFileReadLine.c_str());
		shaderFileLine[shaderFileLineLength - 1] = '\n';

		// Store Shader Line Data
		shaderFileLines.push_back(shaderFileLine);
		shaderFileLineLengths.push_back(shaderFileLineLength);
	}
	shaderFile.close();

	// Create & Compile Shader Object
	o_ShaderID = glCreateShader(a_ShaderType);

	glShaderSource(o_ShaderID, (GLsizei)shaderFileLines.size(), shaderFileLines.data(), shaderFileLineLengths.data());
	glCompileShader(o_ShaderID);

	// Free Memory for Shader File Lines
	for (auto shaderFileLine : shaderFileLines)
	{
		free(shaderFileLine);
	}

	// Ensure Shader has Compiled
	GLint hasCompiled = GL_FALSE;
	glGetShaderiv(o_ShaderID, GL_COMPILE_STATUS, &hasCompiled);
	if (hasCompiled == GL_FALSE)
	{
		// Get Shader Error Message
		GLint errorMessageMaxLength = 0;
		glGetShaderiv(o_ShaderID, GL_INFO_LOG_LENGTH, &errorMessageMaxLength);

		std::vector<GLchar> errorMsg(errorMessageMaxLength);
		glGetShaderInfoLog(o_ShaderID, errorMessageMaxLength, &errorMessageMaxLength, errorMsg.data());

		// Delete Shader & Output Error Message
		glDeleteShader(o_ShaderID);
		o_ShaderID = GL_ZERO;

		Logger::log(errorMsg.data(), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	return true;
}

bool Shader::LoadVertexShaders()
{
	// Ensure Vertex Shaders aren't already Loaded
	if (k_ProjectionShaderID != GL_ZERO || k_PassthroughShaderID != GL_ZERO) {
		Logger::log("Cannot Reload Vertex Shaders", Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}

	// Load Projection Vertex Shader
	if (!loadShaderSource("../Mackerel-Core/res/Shaders/vert/projection.vert", GL_VERTEX_SHADER, k_ProjectionShaderID)) {
		Logger::log("Could not Load Projection Vertex Shader", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}
	// Load Passthrough Vertex Shader
	if (!loadShaderSource("../Mackerel-Core/res/Shaders/vert/passthrough.vert", GL_VERTEX_SHADER, k_PassthroughShaderID)) {
		Logger::log("Could not Load Passthrough Vertex Shader", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	return true;
}
bool Shader::DeleteVertexShaders()
{
	if (k_ProjectionShaderID != GL_ZERO)
	{
		glDeleteShader(k_ProjectionShaderID);
		k_ProjectionShaderID = GL_ZERO;
	}
	if (k_PassthroughShaderID != GL_ZERO)
	{
		glDeleteShader(k_PassthroughShaderID);
		k_PassthroughShaderID = GL_ZERO;
	}

	return true;
}
}

namespace MCK::AssetType
{
Shader::Shader(std::string a_Name = "") :
	m_Name(a_Name),
	m_ProjectionShaderProgramID(GL_ZERO), m_PassthroughShaderProgramID(GL_ZERO)
{}
Shader::~Shader()
{}

void Shader::resetShader()
{
	// Delete Shader Programs
	if (m_ProjectionShaderProgramID != GL_ZERO)
	{
		glDeleteProgram(m_ProjectionShaderProgramID);
		m_ProjectionShaderProgramID = GL_ZERO;
	}
	if (m_PassthroughShaderProgramID != GL_ZERO)
	{
		glDeleteProgram(m_PassthroughShaderProgramID);
		m_PassthroughShaderProgramID = GL_ZERO;
	}
}

/**
 * Load Shader from a File Path.
 * 
 * \param a_FilePath: The Shader's File Path
 * \return Whether the Shader could be Loaded
 */
bool Shader::LoadFromFile(std::string a_FilePath)
{
	// Ensure Shader Programs weren't already Loaded
	if (m_ProjectionShaderProgramID != GL_ZERO || m_PassthroughShaderProgramID != GL_ZERO) {
		Logger::log(std::format("Cannot Reload {} Shader", m_Name), Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}

	// Load & Compile Shader from Source File
	GLuint shaderID = GL_ZERO;
	if (!loadShaderSource(a_FilePath, GL_FRAGMENT_SHADER, shaderID)) {
		Logger::log(std::format("Could not Load {} Shader", m_Name), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// Create Projection Shader Program
	m_ProjectionShaderProgramID = glCreateProgram(); {
		glAttachShader(m_ProjectionShaderProgramID, k_ProjectionShaderID);
		glAttachShader(m_ProjectionShaderProgramID, shaderID);

		glLinkProgram(m_ProjectionShaderProgramID);
	}
	// Create Passthrough Shader Program
	m_PassthroughShaderProgramID = glCreateProgram(); {
		glAttachShader(m_PassthroughShaderProgramID, k_PassthroughShaderID);
		glAttachShader(m_PassthroughShaderProgramID, shaderID);

		glLinkProgram(m_PassthroughShaderProgramID);
	}

	// Delete OpenGL Shader
	glDeleteShader(shaderID);

	// Validate Projection Shader Program
	glValidateProgram(m_ProjectionShaderProgramID); {
		GLint validationStatus = GL_FALSE;
		glGetProgramiv(m_ProjectionShaderProgramID, GL_VALIDATE_STATUS, &validationStatus);
		if (validationStatus == GL_FALSE) {
			// Delete Shader Programs
			resetShader();

			Logger::log(std::format("Could not Validate {} Shader Projection Program", m_Name), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			return false;
		}
	}
	glValidateProgram(m_PassthroughShaderProgramID); {
		GLint validationStatus = GL_FALSE;
		glGetProgramiv(m_PassthroughShaderProgramID, GL_VALIDATE_STATUS, &validationStatus);
		if (validationStatus == GL_FALSE) {
			// Delete Shader Programs
			resetShader();

			Logger::log(std::format("Could not Validate {} Shader Passthrough Program", m_Name), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			return false;
		}
	}

	return true;
}

bool Shader::UseProjectionProgram()
{
	if (m_ProjectionShaderProgramID == GL_ZERO)
	{
		Logger::log(std::format("Cannot Use Unexistant {} Projection Shader Program", m_Name), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	glUseProgram(m_ProjectionShaderProgramID);
	return true;
}
bool Shader::UsePassthroughProgram()
{
	if (m_PassthroughShaderProgramID == GL_ZERO)
	{
		Logger::log(std::format("Cannot Use Unexistant {} Passthrough Shader Program", m_Name), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	glUseProgram(m_PassthroughShaderProgramID);
	return true;
}
}
