#include "MeshLibrary.h"

#include "Mesh.h"

// Logging Headers
#include "LoggingSystem.h"
#include <format>

// Static/Singleton Functions
namespace MCK {
MeshLibrary* MeshLibrary::k_Instance = nullptr;

/**
 * Getter for the MeshLibrary Singleton.
 *
 * \return The Singleton Instance
 */
MeshLibrary* MeshLibrary::Instance()
{
	// Create MeshLibrary Instance if it Doesn't Exist Yet
	if (!k_Instance)
	{
		k_Instance = new MeshLibrary();
	}

	return k_Instance;
}

/**
 * Delete the Mesh Library Singleton Instance.
 * Should only be Called at End of Application Lifetime.
 *
 * \return Whether the Mesh Library was Released
 */
bool MeshLibrary::ReleaseLibrary()
{
	if (!k_Instance)
	{// Cannot Release Non-Initialised Mesh Library
		Logger::log("Cannot Release Non-Initialised Mesh Library", Logger::LogLevel::Warning, std::source_location::current(), "ENGINE");
		return false;
	}

	// Delete Mesh Library Singleton Instance
	delete k_Instance;
	k_Instance = nullptr;

	return true;
}

/**
* Attempt to Retrieve Specified Mesh from Memory.
*
* \param a_Asset: Enum Identifier of Desired Mesh
* \param o_Mesh: Output Reference to Retrieved Mesh
* \return Whether the Mesh could be Retrieved
*/
bool MeshLibrary::GetMesh(MeshEnum a_Asset, AssetType::Mesh*& o_Mesh)
{
	return Instance()->getMesh(a_Asset, o_Mesh);
}
/**
* Attempt to Load Specified Mesh from Disk into Memory.
*
* \param a_Asset: Enum Identifier of Desired Mesh
* \param a_FilePath: File Path to the Mesh
* \return Whether the Mesh could be Loaded
*/
bool MeshLibrary::LoadMesh(MeshEnum a_Asset, std::string a_FilePath)
{
	if (static_cast<int>(a_Asset) < 0)
	{// Engine Reserved Values
		return false;
	}

	return Instance()->loadMesh(a_Asset, a_FilePath);
}
/**
* Attempt to Relase Specified Mesh from Memory.
*
* \param a_Asset: Enum Identifier of Desired Mesh
* \return Whether the Mesh could be Deleted
*/
bool MeshLibrary::FreeMesh(MeshEnum a_Asset)
{
	if (static_cast<int>(a_Asset) < 0)
	{// Engine Reserved Values
		return false;
	}

	return Instance()->freeMesh(a_Asset);
}
}

namespace MCK {
MeshLibrary::MeshLibrary()
{
	// TODO: Load Engine Reserved Meshes to the Data
	loadMesh(MeshEnum::__MCK_DISPLAY_SCREEN, "../Mackerel-Core/res/Meshes/DisplayScreen.obj");

	/* Example:
		AssetType::Mesh defaultTex(std::string filepath_to_default_texture);
		data.insert(std::pair<MeshEnum, AssetType::Mesh>(MeshEnum::__MCK__DEFAULT, defaultTex));
	*/
}
MeshLibrary::~MeshLibrary()
{
	// Free all Mesh Assets Loaded in Memory
	for (auto [assetEnum, texture] : m_LibraryData)
	{
		freeMesh(assetEnum);
	}
}

/**
* Private Implementation of the MeshLibrary::GetMesh() Function.
* Attempt to Retrieve Specified Mesh from Memory.
*
* \param a_Asset: Enum Identifier of Desired Mesh
* \param o_Mesh: Output Reference to Retrieved Mesh
* \return Whether the Mesh could be Retrieved
*/
bool MeshLibrary::getMesh(MeshEnum a_Asset, AssetType::Mesh*& o_Mesh)
{
	if (!m_LibraryData.contains(a_Asset))
	{// Mesh Asset does not Exist in Memory
		return false;
	}

	// Return Mesh Asset
	o_Mesh = m_LibraryData[a_Asset];

	return true;
}
/**
* Private Implementation of the MeshLibrary::LoadMesh() Function.
* Attempt to Load Specified Mesh from Disk into Memory.
*
* \param a_Asset: Enum Identifier of Desired Mesh
* \param a_FilePath: File Path to the Mesh
* \return Whether the Mesh could be Loaded
*/
bool MeshLibrary::loadMesh(MeshEnum a_Asset, std::string a_FilePath)
{
	if (m_LibraryData.contains(a_Asset))
	{// Mesh Asset already Exists in Memory
		//Logger::log("Mesh ")
		return false;
	}

	// TODO: Compute Mesh File Name from File Path
	std::string fileName = a_FilePath;

	// Load Mesh Asset
	AssetType::Mesh* loadMesh = new AssetType::Mesh(fileName);
	if (!loadMesh->LoadFromFile(a_FilePath))
	{// Mesh Failed to Load

		return false;
	}

	// Add Mesh Asset to Memory
	m_LibraryData[a_Asset] = loadMesh;

	return true;
}
/**
* Private Implementation of the MeshLibrary::FreeMesh() Function.
* Attempt to Release Specified Mesh from Memory.
*
* \param a_Asset: Enum Identifier of Desired Mesh
* \return Whether the Mesh could be Deleted
*/
bool MeshLibrary::freeMesh(MeshEnum a_Asset)
{
	if (!m_LibraryData.contains(a_Asset))
	{// Mesh Asset does not Exist in Memory
		//Logger::log("Mesh ")
		return false;
	}

	// Unload the Mesh Asset
	delete m_LibraryData[a_Asset];
	m_LibraryData.erase(a_Asset);

	return true;
}
}
