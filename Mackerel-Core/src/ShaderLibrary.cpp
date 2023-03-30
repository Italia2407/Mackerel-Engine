#include "ShaderLibrary.h"

#include "Shader.h"

// Logging Headers
#include "LoggingSystem.h"
#include <format>

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
}

namespace MCK {
ShaderLibrary::ShaderLibrary()
{
	// Load Vertex Shaders for Shader Program Compilation
	AssetType::Shader::LoadVertexShaders();
	
	// TODO: Load Engine Reserved Shaders to the Data
	loadShader(ShaderEnum::__MCK_FRAMEBUFFER_DISPLAY, "../Mackerel-Core/res/Shaders/static/FBDisplayer.glsl");
	loadShader(ShaderEnum::__LIGHT_UNLIT, "../Mackerel-Core/res/Shaders/light/unlit.glsl");
	loadShader(ShaderEnum::__FRAG_MONOCOLOUR, "../Mackerel-Core/res/Shaders/frag/monocolour.glsl");

	/* Example:
		AssetType::Shader defaultTex(std::string filepath_to_default_texture);
		data.insert(std::pair<ShaderEnum, AssetType::Shader>(ShaderEnum::__MCK__DEFAULT, defaultTex));
	*/
}
ShaderLibrary::~ShaderLibrary()
{
	// Delete Vertex Shaders for Shader Program Compilation
	AssetType::Shader::DeleteVertexShaders();

	// Free all Shader Assets Loaded in Memory
	for (auto [assetEnum, texture] : m_LibraryData)
	{
		freeShader(assetEnum);
	}
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
bool ShaderLibrary::freeShader(ShaderEnum a_Asset)
{
	if (!m_LibraryData.contains(a_Asset))
	{// Shader Asset does not Exist in Memory
		//Logger::log("Shader ")
		return false;
	}

	// Unload the Shader Asset
	delete m_LibraryData[a_Asset];
	m_LibraryData.erase(a_Asset);

	return true;
}
}
