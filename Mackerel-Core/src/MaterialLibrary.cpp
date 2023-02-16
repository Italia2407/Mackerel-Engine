#include "MaterialLibrary.h"

using namespace MCK;

MaterialLibrary* MaterialLibrary::instance = nullptr;

MaterialLibrary::MaterialLibrary()
{
	// TODO: Add engine reserved materials to the data here

	/* Example:
		AssetType::Material defaultMat(std::string filepath_to_default_material);
		data.insert(std::pair<MaterialEnum, AssetType::Material>(MaterialEnum::__MCK__DEFAULT, defaultMat));
	*/
}

MaterialLibrary::~MaterialLibrary()
{}

bool MCK::MaterialLibrary::privGet(MaterialEnum asset, AssetType::Material * out)
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

bool MCK::MaterialLibrary::privLoad(MaterialEnum asset, std::string filepath)
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
			AssetType::Material placeholderMat(std::string filepath);
			data.insert(std::pair<MaterialEnum, AssetType::Material>(asset, placeholderMat));
		*/
		success = true;
	}

	return success;
}

bool MCK::MaterialLibrary::privFree(MaterialEnum asset)
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

bool MCK::MaterialLibrary::Release()
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
