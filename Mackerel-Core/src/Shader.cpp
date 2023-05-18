#include "LoggingSystem.h"

#include "Shader.h"

#include <format>

#include <iostream>
#include <fstream>
#include <string.h>

#include <vector>

namespace MCK::AssetType {
GLuint Shader::k_ProjectionShaderID = GL_ZERO;
GLuint Shader::k_SkinnedProjectionShaderID = GL_ZERO;
Shader* Shader::k_DepthOnlyShader = nullptr;

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

bool Shader::LoadDefaultShaders()
{
	// Ensure Vertex Shaders aren't already Loaded
	if (k_ProjectionShaderID != GL_ZERO) {
		Logger::log("Cannot Reload Vertex Shader", Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}
	if (k_SkinnedProjectionShaderID != GL_ZERO) {
		Logger::log("Cannot Reload Skinned Vertex Shader", Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}


	// Load Projection Vertex Shaders
	if (!loadShaderSource("../Mackerel-Core/res/Shaders/vert/projection.vert", GL_VERTEX_SHADER, k_ProjectionShaderID)) {
		Logger::log("Could not Load Projection Vertex Shader", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}
	if (!loadShaderSource("../Mackerel-Core/res/Shaders/vert/skinned_projection.vert", GL_VERTEX_SHADER, k_SkinnedProjectionShaderID)) {
		Logger::log("Could not Load Skinned Projection Vertex Shader", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	// Load Depth Only Shader Program
	k_DepthOnlyShader = new Shader("Depth Only Shader");
	if (!k_DepthOnlyShader->LoadFromFile("../Mackerel-Core/res/Shaders/static/depthOnly.glsl")) {
		Logger::log("Could not Load Depth Only Shader", Logger::LogLevel::Fatal, std::source_location::current(), "ENGINE");
		return false;
	}

	return true;
}
bool Shader::DeleteDefaultShaders()
{
	if (k_ProjectionShaderID != GL_ZERO)
	{
		glDeleteShader(k_ProjectionShaderID);
		k_ProjectionShaderID = GL_ZERO;
	}
	if (k_SkinnedProjectionShaderID != GL_ZERO)
	{
		glDeleteShader(k_SkinnedProjectionShaderID);
		k_SkinnedProjectionShaderID = GL_ZERO;
	}

	if (k_DepthOnlyShader != nullptr)
		delete k_DepthOnlyShader;

	return true;
}
}

namespace MCK::AssetType
{
Shader::Shader(std::string a_Name = "") :
	m_Name(a_Name),
	m_ShaderProgramID(GL_ZERO),
	m_SkinnedShaderProgramID(GL_ZERO)
{}
Shader::~Shader()
{}

void Shader::resetShader()
{
	// Delete Shader Programs
	if (m_ShaderProgramID != GL_ZERO)
	{
		glDeleteProgram(m_ShaderProgramID);
		m_ShaderProgramID = GL_ZERO;
	}
	if (m_SkinnedShaderProgramID != GL_ZERO)
	{
		glDeleteProgram(m_SkinnedShaderProgramID);
		m_SkinnedShaderProgramID = GL_ZERO;
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
	// Ensure Shader Program isn't already Loaded
	if (m_ShaderProgramID != GL_ZERO) {
		Logger::log(std::format("Cannot Reload {} Shader", m_Name), Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}
	if (m_SkinnedShaderProgramID != GL_ZERO) {
		Logger::log(std::format("Cannot Reload {} Skinned Shader", m_Name), Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}

	// Load & Compile Shader from Source File
	GLuint shaderID = GL_ZERO;
	if (!loadShaderSource(a_FilePath, GL_FRAGMENT_SHADER, shaderID)) {
		Logger::log(std::format("Could not Load {} Shader", m_Name), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	// Create Shader Programs
	m_ShaderProgramID = glCreateProgram(); {
		glAttachShader(m_ShaderProgramID, k_ProjectionShaderID);
		glAttachShader(m_ShaderProgramID, shaderID);

		glLinkProgram(m_ShaderProgramID);
	}
	m_SkinnedShaderProgramID = glCreateProgram(); {
		glAttachShader(m_SkinnedShaderProgramID, k_SkinnedProjectionShaderID);
		glAttachShader(m_SkinnedShaderProgramID, shaderID);

		glLinkProgram(m_SkinnedShaderProgramID);
	}

	// Delete OpenGL Shader
	glDeleteShader(shaderID);

	// Validate Shader Programs
	glValidateProgram(m_ShaderProgramID); {
		GLint validationStatus = GL_FALSE;
		glGetProgramiv(m_ShaderProgramID, GL_VALIDATE_STATUS, &validationStatus);
		if (validationStatus == GL_FALSE) {
			// Delete Shader Programs
			resetShader();

			Logger::log(std::format("Could not Validate {} Shader Program", m_Name), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			return false;
		}
	}
	glValidateProgram(m_SkinnedShaderProgramID); {
		GLint validationStatus = GL_FALSE;
		glGetProgramiv(m_SkinnedShaderProgramID, GL_VALIDATE_STATUS, &validationStatus);
		if (validationStatus == GL_FALSE) {
			// Delete Shader Programs
			resetShader();

			Logger::log(std::format("Could not Validate {} Skinned Shader Program", m_Name), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
			return false;
		}
	}

	return true;
}

GLint Shader::GetShaderUniformLocation(std::string a_UniformName, bool a_IsSkinned)
{
	GLuint shaderProgramID = a_IsSkinned ? m_SkinnedShaderProgramID : m_ShaderProgramID;

	return glGetUniformLocation(shaderProgramID, a_UniformName.c_str());
}

bool Shader::UseShaderProgram(bool a_IsSkinned)
{
	GLuint shaderProgramID = a_IsSkinned ? m_SkinnedShaderProgramID : m_ShaderProgramID;

	if (shaderProgramID == GL_ZERO)
	{
		Logger::log(std::format("Cannot Use Unexistant {} Shader Program", m_Name), Logger::LogLevel::Error, std::source_location::current(), "ENGINE");
		return false;
	}

	glUseProgram(shaderProgramID);
	return true;
}
}
