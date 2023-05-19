#include "LoggingSystem.h"
#include "ShaderLibrary.h"

#include "Shader.h"

#include "CacheReader.h"

#include <format>

#include "Paths.h"

const std::string SHADER_CACHE_NAME = "shaderCache.csv";

// Static/Singleton Functions
namespace MCK {
ShaderLibrary* ShaderLibrary::k_Instance = nullptr;

/**
 * Getter for the ShaderLibrary Singleton.
 *
 * \return The Singleton Instance
 */
ShaderLibrary* ShaderLibrary::Instance()
{
	// Create ShaderLibrary Instance if it Doesn't Exist Yet
	if (!k_Instance)
	{
		k_Instance = new ShaderLibrary();
	}

	return k_Instance;
}

/**
 * Delete the Shader Library Singleton Instance.
 * Should only be Called at End of Application Lifetime.
 *
 * \return Whether the Shader Library was Released
 */
bool ShaderLibrary::ReleaseLibrary()
{
	if (!k_Instance)
	{// Cannot Release Non-Initialised Shader Library
		Logger::log("Cannot Release Non-Initialised Shader Library", Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}

	// Delete Shader Library Singleton Instance
	delete k_Instance;
	k_Instance = nullptr;

	return true;
}

/**
* Attempt to Retrieve Specified Shader from Memory.
*
* \param a_Asset: Enum Identifier of Desired Shader
* \param o_Shader: Output Reference to Retrieved Shader
* \return Whether the Shader could be Retrieved
*/
bool ShaderLibrary::GetShader(ShaderEnum a_Asset, AssetType::Shader*& o_Shader)
{
	return Instance()->getShader(a_Asset, o_Shader);
}
/**
* Attempt to Load Specified Shader from Disk into Memory.
*
* \param a_Asset: Enum Identifier of Desired Shader
* \param a_FilePath: File Path to the Shader
* \return Whether the Shader could be Loaded
*/
bool ShaderLibrary::LoadShader(ShaderEnum a_Asset, std::string a_FilePath)
{
	if (static_cast<int>(a_Asset) < 0)
	{// Engine Reserved Values
		return false;
	}

	return Instance()->loadShader(a_Asset, a_FilePath);
}

/**
* Attempt to Load Specified Shader from Disk into Memory.
*
* \param a_Asset: Enum Identifier of Desired Shader
* \return Whether the Shader could be Loaded
*/
bool ShaderLibrary::LoadShader(ShaderEnum a_Asset)
{
	if (static_cast<int>(a_Asset) < 0)
	{// Engine Reserved Values
		return false;
	}

	std::string path;
	if (!GetPath(a_Asset, &path))
	{
		return false;
	}

	return Instance()->loadShader(a_Asset, path);
}

/**
* Attempt to Relase Specified Shader from Memory.
*
* \param a_Asset: Enum Identifier of Desired Shader
* \return Whether the Shader could be Deleted
*/
bool ShaderLibrary::FreeShader(ShaderEnum a_Asset)
{
	if (static_cast<int>(a_Asset) < 0)
	{// Engine Reserved Values
		return false;
	}

	return Instance()->freeShader(a_Asset);
}

bool ShaderLibrary::getPath(ShaderEnum a_Asset, std::string* stringOutput)
{
	auto search = m_FileMap.find(static_cast<int>(a_Asset));
	if (search != m_FileMap.end())
	{
		*stringOutput = search->second;
		return true;
	}

	return false;
}
}

namespace MCK {
ShaderLibrary::ShaderLibrary()
{
	MCK::ResourceManagement::ReadCache((MCK::Paths::CACHES_PATH + SHADER_CACHE_NAME).c_str(), m_FileMap);

	// Load Vertex Shaders for Shader Program Compilation
	AssetType::Shader::LoadDefaultShaders();
	
	// TODO: Load Engine Reserved Shaders to the Data
	loadShader(ShaderEnum::__MCK_FRAMEBUFFER_DISPLAY, "../Mackerel-Core/res/Shaders/static/FBDisplayer.glsl");
	loadShader(ShaderEnum::__LIGHT_UNLIT, "../Mackerel-Core/res/Shaders/light/unlit.glsl");
	loadShader(ShaderEnum::__FRAG_MONOCOLOUR, "../Mackerel-Core/res/Shaders/frag/monocolour.glsl");
	loadShader(ShaderEnum::__LIGHT_UNLIT_SHADOWS, "../Mackerel-Core/res/Shaders/frag/unlitShadows.glsl");

	/* Example:
		AssetType::Shader defaultTex(std::string filepath_to_default_texture);
		data.insert(std::pair<ShaderEnum, AssetType::Shader>(ShaderEnum::__MCK__DEFAULT, defaultTex));
	*/
}
ShaderLibrary::~ShaderLibrary()
{
	// Delete Vertex Shaders for Shader Program Compilation
	AssetType::Shader::DeleteDefaultShaders();

	// Free all Shader Assets Loaded in Memory
	for (auto itt = m_LibraryData.begin(); itt != m_LibraryData.end(); itt++)
	{
		freeShader(itt->first, false);
	}

	m_LibraryData.clear();
}

/**
* Private Implementation of the ShaderLibrary::GetShader() Function.
* Attempt to Retrieve Specified Shader from Memory.
*
* \param a_Asset: Enum Identifier of Desired Shader
* \param o_Shader: Output Reference to Retrieved Shader
* \return Whether the Shader could be Retrieved
*/
bool ShaderLibrary::getShader(ShaderEnum a_Asset, AssetType::Shader*& o_Shader)
{
	if (!m_LibraryData.contains(a_Asset))
	{// Shader Asset does not Exist in Memory
		return false;
	}

	// Return Shader Asset
	o_Shader = m_LibraryData[a_Asset];

	return true;
}
/**
* Private Implementation of the ShaderLibrary::LoadShader() Function.
* Attempt to Load Specified Shader from Disk into Memory.
*
* \param a_Asset: Enum Identifier of Desired Shader
* \param a_FilePath: File Path to the Shader
* \return Whether the Shader could be Loaded
*/
bool ShaderLibrary::loadShader(ShaderEnum a_Asset, std::string a_FilePath)
{
	if (m_LibraryData.contains(a_Asset))
	{// Shader Asset already Exists in Memory
		//Logger::log("Shader ")
		return false;
	}

	// TODO: Compute Shader File Name from File Path
	std::string fileName = a_FilePath;

	// Load Shader Asset
	AssetType::Shader* loadShader = new AssetType::Shader(fileName);
	if (!loadShader->LoadFromFile(a_FilePath))
	{// Shader Failed to Load

		return false;
	}

	// Add Shader Asset to Memory
	m_LibraryData[a_Asset] = loadShader;

	return true;
}
/**
* Private Implementation of the ShaderLibrary::FreeShader() Function.
* Attempt to Release Specified Shader from Memory.
*
* \param a_Asset: Enum Identifier of Desired Shader
* \return Whether the Shader could be Deleted
*/
bool ShaderLibrary::freeShader(ShaderEnum a_Asset, bool erase)
{
	if (!m_LibraryData.contains(a_Asset))
	{// Shader Asset does not Exist in Memory
		//Logger::log("Shader ")
		return false;
	}

	// Unload the Shader Asset
	delete m_LibraryData[a_Asset];
	if(erase)
		m_LibraryData.erase(a_Asset);

	return true;
}
}
