#include "TextureLibrary.h"

using namespace MCK;

TextureLibrary* TextureLibrary::instance = nullptr;

TextureLibrary::TextureLibrary()
{
	// TODO: Add engine reserved textures to the data here

	/* Example:
		AssetType::Texture defaultTex(std::string filepath_to_default_texture);
		data.insert(std::pair<TextureEnum, AssetType::Texture>(TextureEnum::__MCK__DEFAULT, defaultTex));
	*/
}

TextureLibrary::~TextureLibrary()
{}

bool MCK::TextureLibrary::privGet(TextureEnum asset, AssetType::Texture* out)
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

bool MCK::TextureLibrary::privLoad(TextureEnum asset, std::string filepath)
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
			AssetType::Texture placeholderTex(std::string filepath);
			data.insert(std::pair<TextureEnum, AssetType::Texture>(asset, placeholderTex));
		*/
		success = true;
	}

	return success;
}

bool MCK::TextureLibrary::privFree(TextureEnum asset)
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

bool MCK::TextureLibrary::Release()
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

