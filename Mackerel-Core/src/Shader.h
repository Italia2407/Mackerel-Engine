#pragma once
#include <glad/glad.h>

#include <string>

namespace MCK::AssetType {
class Shader
{
public:
	Shader(std::string a_Name);
	~Shader();

	static Shader* k_DepthOnlyShader;

private:
	// Vertex Shader
	static GLuint k_ProjectionShaderID;
	static GLuint k_SkinnedProjectionShaderID;

	static bool loadShaderSource(std::string a_FilePath, GLuint a_ShaderType, GLuint& o_ShaderID);

public:
	static bool LoadDefaultShaders();
	static bool DeleteDefaultShaders();

private:
	std::string m_Name;

	GLuint m_ShaderProgramID;
	GLuint m_SkinnedShaderProgramID;

	void resetShader();
	
public:
	// Asset Management Functions
	bool LoadFromFile(std::string a_FilePath);

	GLint GetShaderUniformLocation(std::string a_UniformName, bool a_IsSkinned);

	// Shader Program Binders
	bool UseShaderProgram(bool a_IsSkinned);
};
}