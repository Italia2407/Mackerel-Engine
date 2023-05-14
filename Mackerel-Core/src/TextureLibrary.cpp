#include "LoggingSystem.h"

#include "TextureLibrary.h"

#include "Texture.h"

#include <format>

#include "CacheReader.h"

#include "Paths.h"

const std::string TEXTURE_CACHE_NAME = "textureCache.csv";

// Static/Singleton Functions
namespace MCK {
TextureLibrary* TextureLibrary::k_Instance = nullptr;

/**
 * Getter for the TextureLibrary Singleton.
 *
 * \return The Singleton Instance
 */
TextureLibrary* TextureLibrary::Instance()
{
	// Create TextureLibrary Instance if it Doesn't Exist Yet
	if (!k_Instance)
	{
		k_Instance = new TextureLibrary();
	}

	return k_Instance;
}

/**
 * Delete the Texture Library Singleton Instance.
 * Should only be Called at End of Application Lifetime.
 * 
 * \return Whether the Texture Library was Released
 */
bool TextureLibrary::ReleaseLibrary()
{
	if (!k_Instance)
	{// Cannot Release Non-Initialised Texture Library
		Logger::log("Cannot Release Non-Initialised Texture Library", Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}

	// Delete Texture Library Singleton Instance
	delete k_Instance;
	k_Instance = nullptr;

	return true;
}

/**
* Attempt to Retrieve Specified Texture from Memory.
*
* \param a_Asset: Enum Identifier of Desired Texture
* \param o_Texture: Output Reference to Retrieved Texture
* \return Whether the Texture could be Retrieved
*/
bool TextureLibrary::GetTexture(TextureEnum a_Asset, AssetType::Texture*& o_Texture)
{
	return Instance()->getTexture(a_Asset, o_Texture);
}
/**
* Attempt to Load Specified Texture from Disk into Memory.
*
* \param a_Asset: Enum Identifier of Desired Texture
* \param a_FilePath: File Path to the Texture
* \return Whether the Texture could be Loaded
*/
bool TextureLibrary::LoadTexture(TextureEnum a_Asset, std::string a_FilePath)
{
	if (static_cast<int>(a_Asset) < 0)
	{// Engine Reserved Values
		return false;
	}

	return Instance()->loadTexture(a_Asset, a_FilePath);
}

/**
* Attempt to Load Specified Texture from Disk into Memory.
*
* \param a_Asset: Enum Identifier of Desired Texture
* \return Whether the Texture could be Loaded
*/
bool TextureLibrary::LoadTexture(TextureEnum a_Asset)
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

	return Instance()->loadTexture(a_Asset, path);
}
/**
* Attempt to Relase Specified Texture from Memory.
*
* \param a_Asset: Enum Identifier of Desired Texture
* \return Whether the Texture could be Deleted
*/
bool TextureLibrary::FreeTexture(TextureEnum a_Asset)
{
	if (static_cast<int>(a_Asset) < 0)
	{// Engine Reserved Values
		return false;
	}

	return Instance()->freeTexture(a_Asset);
}

bool TextureLibrary::getPath(TextureEnum a_Asset, std::string* stringOutput)
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
TextureLibrary::TextureLibrary()
{
	// Load map
	MCK::ResourceManagement::ReadCache((MCK::Paths::CACHES_PATH + TEXTURE_CACHE_NAME).c_str(), m_FileMap);

	// TODO: Load Engine Reserved Textures to the Data

	/* Example:
		AssetType::Texture defaultTex(std::string filepath_to_default_texture);
		data.insert(std::pair<TextureEnum, AssetType::Texture>(TextureEnum::__MCK__DEFAULT, defaultTex));
	*/
}
TextureLibrary::~TextureLibrary()
{
	// Free all Texture Assets Loaded in Memory
	for (auto [assetEnum, texture] : m_LibraryData)
	{
		freeTexture(assetEnum);
	}
}

/**
* Private Implementation of the TextureLibrary::GetTexture() Function.
* Attempt to Retrieve Specified Texture from Memory.
* 
* \param a_Asset: Enum Identifier of Desired Texture
* \param o_Texture: Output Reference to Retrieved Texture
* \return Whether the Texture could be Retrieved
*/
bool TextureLibrary::getTexture(TextureEnum a_Asset, AssetType::Texture*& o_Texture)
{
	if (!m_LibraryData.contains(a_Asset))
	{// Texture Asset does not Exist in Memory
		return false;
	}

	// Return Texture Asset
	o_Texture = m_LibraryData[a_Asset];

	return true;
}
/**
* Private Implementation of the TextureLibrary::LoadTexture() Function.
* Attempt to Load Specified Texture from Disk into Memory.
*
* \param a_Asset: Enum Identifier of Desired Texture
* \param a_FilePath: File Path to the Texture
* \return Whether the Texture could be Loaded
*/
bool TextureLibrary::loadTexture(TextureEnum a_Asset, std::string a_FilePath)
{
	if (m_LibraryData.contains(a_Asset))
	{// Texture Asset already Exists in Memory
		//Logger::log("Texture ")
		return false;
	}

	// Load Texture Asset
	AssetType::Texture* loadTexture = new AssetType::Texture();
	if (!loadTexture->LoadFromFile(a_FilePath, false))
	{// Texture Failed to Load

		return false;
	}

	// Add Texture Asset to Memory
	m_LibraryData[a_Asset] = loadTexture;

	return true;
}
/**
* Private Implementation of the TextureLibrary::FreeTexture() Function.
* Attempt to Release Specified Texture from Memory.
*
* \param a_Asset: Enum Identifier of Desired Texture
* \return Whether the Texture could be Deleted
*/
bool TextureLibrary::freeTexture(TextureEnum a_Asset)
{
	if (!m_LibraryData.contains(a_Asset))
	{// Texture Asset does not Exist in Memory
		//Logger::log("Texture ")
		return false;
	}

	// Unload the Texture Asset
	delete m_LibraryData[a_Asset];
	m_LibraryData.erase(a_Asset);

	return true;
}
}
