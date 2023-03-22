#include "Shader.h"

#include "LoggingSystem.h"

#include <iostream>
#include <fstream>
#include <string.h>

#include <vector>

namespace MCK::AssetType
{
Shader::Shader() :
	m_ShaderID(GL_ZERO), m_IsLoaded(false), m_FilePath(""), m_ShaderType(GL_ZERO) {}
Shader::~Shader() {}

bool Shader::LoadShaderFromSource(std::string a_FilePath, GLuint a_ShaderType)
{
	if (m_IsLoaded)
	{// Loaded Shader Cannot be Reloaded
		Logger::log("ERROR: Loaded Shader cannot be Reloaded", Logger::LogLevel::Error, std::source_location::current());
		return false;
	}

	// TODO: Clean Up this Mess, Code Feels quite Hacky
	// Load Shader File Contents in List of Strings
	// NEED TO BE NULL TERMINATED
	std::ifstream inputFile;
	inputFile.open(a_FilePath.c_str(), std::ios::in);
	if (!inputFile.good())
	{
		Logger::log("ERROR: Shader File could not be Loaded!", Logger::LogLevel::Error, std::source_location::current());
		return false;
	}

	std::vector<GLchar*> shaderFileLines;

	std::string readLine;
	while (std::getline(inputFile, readLine))
	{
		// Length of Line Entry, 2 Extra Characters Needed for '\n' and '\0'
		size_t lineLength = readLine.length() + 2;

		// Create New Line Entry for Shader Code
		GLchar* shaderFileLine = (GLchar*)std::calloc(lineLength, sizeof(GLchar));
		shaderFileLines.push_back(shaderFileLine);

		strcpy_s(shaderFileLine, lineLength * sizeof(GLchar), readLine.c_str());
		shaderFileLine[lineLength - 1] = '\0';
		shaderFileLine[lineLength - 2] = '\n';
	}

	// Create & Compile Shader Object
	m_ShaderID = glCreateShader(a_ShaderType);

	glShaderSource(m_ShaderID, (GLsizei)shaderFileLines.size(), shaderFileLines.data(), nullptr);
	glCompileShader(m_ShaderID);

	// Free Memory of Shader File Lines
	for (auto line : shaderFileLines)
	{
		free(line);
	}

	// Ensure Shader Compiled
	GLint hasCompiled = 0;
	glGetShaderiv(m_ShaderID, GL_COMPILE_STATUS, &hasCompiled);
	if (hasCompiled == GL_FALSE)
	{
		// Get Shader Error Log Message
		GLint errorMsgMaxLength = 0;
		glGetShaderiv(m_ShaderID, GL_INFO_LOG_LENGTH, &errorMsgMaxLength);

		std::vector<GLchar> errorMsg(errorMsgMaxLength);
		glGetShaderInfoLog(m_ShaderID, errorMsgMaxLength, &errorMsgMaxLength, errorMsg.data());

		// Output Error Message & Delete Shader
		std::cout << "Shader Didn't Compile" << std::endl;
		std::cout << errorMsg.data() << std::endl;
		glDeleteShader(m_ShaderID);

		return false;
	}

	// Shader Compiled Successfully

	m_IsLoaded = true;
	return true;
}
}
