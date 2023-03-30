#include "MaterialLibrary.h"

#include "Material.h"

// Logging Headers
#include "LoggingSystem.h"
#include <format>

// Static/Singleton Functions
namespace MCK {
MaterialLibrary* MaterialLibrary::k_Instance = nullptr;

/**
 * Getter for the MaterialLibrary Singleton.
 *
 * \return The Singleton Instance
 */
MaterialLibrary* MaterialLibrary::Instance()
{
	// Create MaterialLibrary Instance if it Doesn't Exist Yet
	if (!k_Instance)
	{
		k_Instance = new MaterialLibrary();
	}

	return k_Instance;
}

/**
 * Delete the Material Library Singleton Instance.
 * Should only be Called at End of Application Lifetime.
 *
 * \return Whether the Material Library was Released
 */
bool MaterialLibrary::ReleaseLibrary()
{
	if (!k_Instance)
	{// Cannot Release Non-Initialised Material Library
		Logger::log("Cannot Release Non-Initialised Material Library", Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}

	// Delete Material Library Singleton Instance
	delete k_Instance;
	k_Instance = nullptr;

	return true;
}

/**
* Attempt to Retrieve Specified Material from Memory.
*
* \param a_Asset: Enum Identifier of Desired Material
* \param o_Material: Output Reference to Retrieved Material
* \return Whether the Material could be Retrieved
*/
bool MaterialLibrary::GetMaterial(MaterialEnum a_Asset, AssetType::Material*& o_Material)
{
	return Instance()->getMaterial(a_Asset, o_Material);
}
/**
* Attempt to Load Specified Material from Disk into Memory.
*
* \param a_Asset: Enum Identifier of Desired Material
* \param a_FilePath: File Path to the Material
* \return Whether the Material could be Loaded
*/
bool MaterialLibrary::LoadMaterial(MaterialEnum a_Asset, std::string a_FilePath)
{
	if (static_cast<int>(a_Asset) < 0)
	{// Engine Reserved Values
		return false;
	}

	return Instance()->loadMaterial(a_Asset, a_FilePath);
}
/**
* Attempt to Relase Specified Material from Memory.
*
* \param a_Asset: Enum Identifier of Desired Material
* \return Whether the Material could be Deleted
*/
bool MaterialLibrary::FreeMaterial(MaterialEnum a_Asset)
{
	if (static_cast<int>(a_Asset) < 0)
	{// Engine Reserved Values
		return false;
	}

	return Instance()->freeMaterial(a_Asset);
}
}

namespace MCK {
MaterialLibrary::MaterialLibrary()
{
	// TODO: Load Engine Reserved Materials to the Data

	/* Example:
		AssetType::Material defaultTex(std::string filepath_to_default_texture);
		data.insert(std::pair<MaterialEnum, AssetType::Material>(MaterialEnum::__MCK__DEFAULT, defaultTex));
	*/
}
MaterialLibrary::~MaterialLibrary()
{
	// Free all Material Assets Loaded in Memory
	for (auto [assetEnum, texture] : m_LibraryData)
	{
		freeMaterial(assetEnum);
	}
}

/**
* Private Implementation of the MaterialLibrary::GetMaterial() Function.
* Attempt to Retrieve Specified Material from Memory.
*
* \param a_Asset: Enum Identifier of Desired Material
* \param o_Material: Output Reference to Retrieved Material
* \return Whether the Material could be Retrieved
*/
bool MaterialLibrary::getMaterial(MaterialEnum a_Asset, AssetType::Material*& o_Material)
{
	if (!m_LibraryData.contains(a_Asset))
	{// Material Asset does not Exist in Memory
		return false;
	}

	// Return Material Asset
	o_Material = m_LibraryData[a_Asset];

	return true;
}
/**
* Private Implementation of the MaterialLibrary::LoadMaterial() Function.
* Attempt to Load Specified Material from Disk into Memory.
*
* \param a_Asset: Enum Identifier of Desired Material
* \param a_FilePath: File Path to the Material
* \return Whether the Material could be Loaded
*/
bool MaterialLibrary::loadMaterial(MaterialEnum a_Asset, std::string a_FilePath)
{
	if (m_LibraryData.contains(a_Asset))
	{// Material Asset already Exists in Memory
		//Logger::log("Material ")
		return false;
	}

	// Load Material Asset
	AssetType::Material* loadMaterial = new AssetType::Material();
	if (!loadMaterial->LoadFromFile(a_FilePath))
	{// Material Failed to Load

		return false;
	}

	// Add Material Asset to Memory
	m_LibraryData[a_Asset] = loadMaterial;

	return true;
}
/**
* Private Implementation of the MaterialLibrary::FreeMaterial() Function.
* Attempt to Release Specified Material from Memory.
*
* \param a_Asset: Enum Identifier of Desired Material
* \return Whether the Material could be Deleted
*/
bool MaterialLibrary::freeMaterial(MaterialEnum a_Asset)
{
	if (!m_LibraryData.contains(a_Asset))
	{// Material Asset does not Exist in Memory
		//Logger::log("Material ")
		return false;
	}

	// Unload the Material Asset
	delete m_LibraryData[a_Asset];
	m_LibraryData.erase(a_Asset);

	return true;
}
}
