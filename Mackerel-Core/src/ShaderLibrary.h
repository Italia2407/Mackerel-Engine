#pragma once
#include "ShaderEnum.h"

#include <string>
#include <unordered_map>

namespace MCK::AssetType {
class Shader;
}

namespace MCK {
/**
 * Resource Manager Responsible for Loading, Retrieving, & Releasing Shaders in Memory.
 */
class ShaderLibrary
{
private:
	// Singleton Constructor/Destructor
	ShaderLibrary();
	~ShaderLibrary();

	// Ensure Inability to Copy
	ShaderLibrary(const ShaderLibrary&) = delete;
	ShaderLibrary& operator=(const ShaderLibrary&) = delete;

	// Singleton Instance Bookkeeping
	static ShaderLibrary* k_Instance;
	static ShaderLibrary* Instance();

	// Member Variables
private:
	std::unordered_map<ShaderEnum, AssetType::Shader*> m_LibraryData;
	std::map<int, std::string> m_FileMap;

	// Member Methods
private:
	bool getShader(ShaderEnum a_Asset, AssetType::Shader*& o_Shader);
	bool loadShader(ShaderEnum a_Asset, std::string a_FilePath);
	bool freeShader(ShaderEnum a_Asset, bool erase = true);
	bool getPath(ShaderEnum a_Asset, std::string* stringOutput);

public:
	static bool ReleaseLibrary();

	static bool GetShader(ShaderEnum a_Asset, AssetType::Shader*& o_Shader);
	static bool LoadShader(ShaderEnum a_Asset, std::string a_FilePath);
	static bool LoadShader(ShaderEnum a_Asset);
	static bool FreeShader(ShaderEnum a_Asset);

	static inline bool GetPath(ShaderEnum a_Asset, std::string* stringOutput)
	{
		return Instance()->getPath(a_Asset, stringOutput);
	}
};
}