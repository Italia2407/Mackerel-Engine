#pragma once
#include <glad/glad.h>

#include <string>

namespace MCK::AssetType {
class Shader
{
public:
	Shader();
	~Shader();

private:
	GLuint m_ShaderID;

	std::string m_FilePath;
	GLuint m_ShaderType;

public:
	const GLuint& ShaderID() const { return m_ShaderID; }
	const GLuint& ShaderType() const { return m_ShaderType; }

public:
	bool LoadShaderFromSource(std::string a_FilePath, GLuint a_ShaderType);
};
}