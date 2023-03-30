#pragma once
#include "MaterialEnum.h"

#include <string>
#include <unordered_map>

namespace MCK::AssetType {
class Material;
}

namespace MCK {
/**
 * Resource Manager Responsible for Loading, Retrieving, & Releasing Materials in Memory.
 */
class MaterialLibrary
{
private:
	// Singleton Constructor/Destructor
	MaterialLibrary();
	~MaterialLibrary();

	// Ensure Inability to Copy
	MaterialLibrary(const MaterialLibrary&) = delete;
	MaterialLibrary& operator=(const MaterialLibrary&) = delete;

	// Singleton Instance Bookkeeping
	static MaterialLibrary* k_Instance;
	static MaterialLibrary* Instance();

	// Member Variables
private:
	std::unordered_map<MaterialEnum, AssetType::Material*> m_LibraryData;

	// Member Methods
private:
	bool getMaterial(MaterialEnum a_Asset, AssetType::Material*& o_Material);
	bool loadMaterial(MaterialEnum a_Asset, std::string a_FilePath);
	bool freeMaterial(MaterialEnum a_Asset);

public:
	static bool ReleaseLibrary();

	static bool GetMaterial(MaterialEnum a_Asset, AssetType::Material*& o_Material);
	static bool LoadMaterial(MaterialEnum a_Asset, std::string a_FilePath);
	static bool FreeMaterial(MaterialEnum a_Asset);
};
}
