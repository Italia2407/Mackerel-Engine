#pragma once
#include <glad/glad.h>

#include <string>

namespace MCK::AssetType {
class Shader
{
public:
	Shader(std::string a_Name);
	~Shader();

private:
	// Vertex Shaders
	static GLuint k_ProjectionShaderID;
	static GLuint k_PassthroughShaderID;

	static bool loadShaderSource(std::string a_FilePath, GLuint a_ShaderType, GLuint& o_ShaderID);

public:
	static bool LoadVertexShaders();
	static bool DeleteVertexShaders();

private:
	std::string m_Name;

	// Shader Program for Each Possible Vertex Shader
	GLuint m_ProjectionShaderProgramID;
	GLuint m_PassthroughShaderProgramID;

	void resetShader();
	
public:
	// Asset Management Functions
	bool LoadFromFile(std::string a_FilePath);

	// Shader Program Binders
	bool UseProjectionProgram();
	bool UsePassthroughProgram();
};
}