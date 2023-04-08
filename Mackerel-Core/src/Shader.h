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
	// Vertex Shader
	static GLuint k_ProjectionShaderID;

	static bool loadShaderSource(std::string a_FilePath, GLuint a_ShaderType, GLuint& o_ShaderID);

public:
	static bool LoadVertexShaders();
	static bool DeleteVertexShaders();

private:
	std::string m_Name;

	GLuint m_ShaderProgramID;

	void resetShader();
	
public:
	// Asset Management Functions
	bool LoadFromFile(std::string a_FilePath);

	// Shader Program Binders
	bool UseShaderProgram();
};
}