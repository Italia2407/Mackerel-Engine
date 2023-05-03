#pragma once
#include "TextureEnum.h"

#include <string>
#include <map>
#include <unordered_map>

namespace MCK::AssetType {
class Texture;
}

namespace MCK {
/**
 * Resource Manager Responsible for Loading, Retrieving, & Releasing Textures in Memory.
 */
class TextureLibrary
{
private:
	// Singleton Constructor/Destructor
	TextureLibrary();
	~TextureLibrary();

	// Ensure Inability to Copy
	TextureLibrary(const TextureLibrary&) = delete;
	TextureLibrary& operator=(const TextureLibrary&) = delete;

	// Singleton Instance Bookkeeping
	static TextureLibrary* k_Instance;
	static TextureLibrary* Instance();

// Member Variables
private:
	std::unordered_map<TextureEnum, AssetType::Texture*> m_LibraryData;
	std::map<int, std::string> m_FileMap;

// Member Methods
private:
	bool getTexture(TextureEnum a_Asset, AssetType::Texture*& o_Texture);
	bool loadTexture(TextureEnum a_Asset, std::string a_FilePath);
	bool freeTexture(TextureEnum a_Asset);
	bool getPath(TextureEnum a_Asset, std::string* stringOutput);

public:
	static bool ReleaseLibrary();

	static bool GetTexture(TextureEnum a_Asset, AssetType::Texture*& o_Texture);
	static bool LoadTexture(TextureEnum a_Asset, std::string a_FilePath);
	static bool LoadTexture(TextureEnum a_Asset);
	static bool FreeTexture(TextureEnum a_Asset);

	static inline bool GetPath(TextureEnum a_Asset, std::string* stringOutput)
	{
		return Instance()->getPath(a_Asset, stringOutput);
	}
};
}
