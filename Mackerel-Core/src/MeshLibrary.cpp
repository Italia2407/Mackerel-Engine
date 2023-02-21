#include "MeshLibrary.h"

using namespace MCK;

MeshLibrary* MeshLibrary::instance = nullptr;

MeshLibrary::MeshLibrary()
{
	// TODO: Add engine reserved meshes to the data here

	/* Example:
		AssetType::Mesh defaultMesh(std::string filepath_to_default_mesh);
		data.insert(std::pair<MeshEnum, AssetType::Mesh>(MeshEnum::__MCK__DEFAULT, defaultMesh));
	*/
}

MeshLibrary::~MeshLibrary()
{}

bool MCK::MeshLibrary::privGet(MeshEnum asset, AssetType::Mesh * out)
{
	bool success = false;

	if (static_cast<int>(asset) < 0)
	{
		// Engine reserved value
		success = false;
	}
	else if (data.contains(asset))
	{
		// Hit! (good)
		out = data[asset];
		success = true;
	}

	return success;
}

bool MCK::MeshLibrary::privLoad(MeshEnum asset, std::string filepath)
{
	(void*)&filepath; // currently unused variable, this line is here to avoid warnings

	bool success = false;

	if (static_cast<int>(asset) < 0)
	{
		// Engine reserved value
		success = false;
	}
	else if (data.contains(asset))
	{
		// Hit! (bad)
		success = false;
	}
	else
	{
		// Asset isn't loaded, load it

		/* TODO: replace this when a load function is added:
			AssetType::Mesh placeholderMesh(std::string filepath);
			data.insert(std::pair<MeshEnum, AssetType::Mesh>(asset, placeholderMesh));
		*/
		success = true;
	}

	return success;
}

bool MCK::MeshLibrary::privFree(MeshEnum asset)
{
	bool success = false;

	if (static_cast<int>(asset) < 0)
	{
		// Engine reserved value
		success = false;
	}
	else if (data.contains(asset))
	{
		// Hit! (good)
		data.erase(asset);
		success = true;
	}

	return success;
}

bool MCK::MeshLibrary::Release()
{
	bool success = false;

	if (instance)
	{
		data.clear();
		delete instance;
		success = true;
	}

	return success;
}
