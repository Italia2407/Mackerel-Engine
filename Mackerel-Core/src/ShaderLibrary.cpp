#include "ShaderLibrary.h"

using namespace MCK;

ShaderLibrary* ShaderLibrary::instance = nullptr;

ShaderLibrary::ShaderLibrary()
{
	// TODO: Add engine reserved shaders to the data here

	/* Example:
		AssetType::Shader defaultShdr(std::string filepath_to_default_shader);
		data.insert(std::pair<ShaderEnum, AssetType::Shader>(ShaderEnum::__MCK__DEFAULT, defaultShdr));
	*/
}

ShaderLibrary::~ShaderLibrary()
{}

bool MCK::ShaderLibrary::privGet(ShaderEnum asset, AssetType::Shader* out)
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

bool MCK::ShaderLibrary::privLoad(ShaderEnum asset, std::string filepath)
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
			AssetType::Shader placeholderShdr(std::string filepath);
			data.insert(std::pair<ShaderEnum, AssetType::Shader>(asset, placeholderShdr));
		*/
		success = true;
	}

	return success;
}

bool MCK::ShaderLibrary::privFree(ShaderEnum asset)
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

bool MCK::ShaderLibrary::Release()
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
