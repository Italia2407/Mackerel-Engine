#pragma once
#include "MeshEnum.h"

#include <string>
#include <unordered_map>

namespace MCK::AssetType {
class Mesh;
}

namespace MCK {
/**
 * Resource Manager Responsible for Loading, Retrieving, & Releasing Meshes in Memory.
 */
class MeshLibrary
{
private:
	// Singleton Constructor/Destructor
	MeshLibrary();
	~MeshLibrary();

	// Ensure Inability to Copy
	MeshLibrary(const MeshLibrary&) = delete;
	MeshLibrary& operator=(const MeshLibrary&) = delete;

	// Singleton Instance Bookkeeping
	static MeshLibrary* k_Instance;
	static MeshLibrary* Instance();

	// Member Variables
private:
	std::unordered_map<MeshEnum, AssetType::Mesh*> m_LibraryData;
	std::map<int, std::string> m_FileMap;

	// Member Methods
private:
	bool getMesh(MeshEnum a_Asset, AssetType::Mesh*& o_Mesh);
	bool loadMesh(MeshEnum a_Asset, std::string a_FilePath);
	bool freeMesh(MeshEnum a_Asset, bool erase = true);
	bool getPath(MeshEnum a_Asset, std::string* stringOutput);

public:
	static bool ReleaseLibrary();

	static bool GetMesh(MeshEnum a_Asset, AssetType::Mesh*& o_Mesh);
	static bool LoadMesh(MeshEnum a_Asset, std::string a_FilePath);
	static bool LoadMesh(MeshEnum a_Asset);
	static bool FreeMesh(MeshEnum a_Asset);

	static inline bool GetPath(MeshEnum a_Asset, std::string* stringOutput)
	{
		return Instance()->getPath(a_Asset, stringOutput);
	}
};
}
