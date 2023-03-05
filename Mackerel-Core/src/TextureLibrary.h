#pragma once

#include <string>
#include <unordered_map>

#include "TextureEnum.h"

namespace MCK
{
	namespace AssetType
	{
		class Texture;
	}

	/**
	 * This class is the resource manager responsible for loading, retrieving, and
	 * releasing Textures in memory.
	 */
	class TextureLibrary
	{
	private:
		// private constructor/destructor because singleton
		TextureLibrary();
		~TextureLibrary();

		// not to be copied!
		TextureLibrary(const TextureLibrary&) = delete;
		TextureLibrary& operator=(const TextureLibrary&) = delete;

		// singleton bookkeeping
		static TextureLibrary* instance;
		static TextureLibrary* Instance()
		{
			if (!instance)
				instance = new TextureLibrary;
			return instance;
		}

		// private member variables
		std::unordered_map<TextureEnum, AssetType::Texture*> data;

		// private implementation of Singleton functions

		/**
		 * Private implementation of the TextureLibrary::Get() function.
		 * Attempts to retrieve the specified texture from memory.
		 * 
		 * \param asset: enum identifier of the desired texture
		 * \param out: output reference to return the retrieved texture
		 * \return true if the function succeeded or false if it fails
		 */
		bool privGet(TextureEnum asset, AssetType::Texture* out);

		/**
		 * Private implementation of the TextureLibrary::Load() function.
		 * Attempts to load the specified texture into memory from disk.
		 * 
		 * \param asset: enum identifier of the desired texture
		 * \param filepath: filepath to the texture
		 * \return true if the function succeeded or false if it fails
		 */
		bool privLoad(TextureEnum asset, std::string filepath);

		/**
		 * Private implementation of the TextureLibrary::Load() function.
		 * Attempts to release the specified texture from memory. 
		 * 
		 * \param asset: enum identifier of the desired texture
		 * \return true if a texture was released from memory or false if it wasn't
		 *		loaded in the first place.
		 */
		bool privFree(TextureEnum asset);

	public:
		/**
		 * Releases all textures in memory, and frees the TextureLibrary instance.
		 * Should only be called at the end of the applications lifetime!
		 * 
		 * \return true if the library was successfully released, false if it
		 *		wasn't initialised to begin with
		 */
		bool Release();

		/**
		 * Attempts to retrieve the specified texture from memory.
		 * 
		 * \param asset: enum identifier of the desired texture
		 * \param out: output reference to return the retrieved texture
		 * \return true if the function succeeded or false if it fails
		 */
		bool Get(TextureEnum asset, AssetType::Texture* out)
		{
			return Instance()->privGet(asset, out);
		}

		/**
		 * Attempts to load the specified texture into memory from disk.
		 * 
		 * \param asset: enum identifier of the desired texture
		 * \param filepath: filepath to the texture
		 * \return true if the function succeeded or false if it fails
		 */
		bool Load(TextureEnum asset, std::string filepath)
		{
			return Instance()->privLoad(asset, filepath);
		}

		/**
		 * Attempts to release the specified texture from memory. 
		 * 
		 * \param asset: enum identifier of the desired texture
		 * \return true if a texture was released from memory or false if it wasn't
		 *		loaded in the first place.
		 */
		bool Free(TextureEnum asset)
		{
			return Instance()->privFree(asset);
		}
	};
}
