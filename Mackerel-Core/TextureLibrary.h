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

	/// <summary>
	/// This class is the resource manager responsible for loading, retrieving, and
	///		releasing Textures in memory.
	/// </summary>
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

		/// <summary>
		/// Private implementation of the TextureLibrary::Get() function.
		/// Attempts to retrieve the specified texture from memory.
		/// </summary>
		/// <param name="asset">: enum identifier of the desired texture</param>
		/// <param name="out">: output reference to return the retrieved texture</param>
		/// <returns>true if the function succeeded or false if it fails</returns>
		bool privGet(TextureEnum asset, AssetType::Texture* out);

		/// <summary>
		/// Private implementation of the TextureLibrary::Load() function.
		/// Attempts to load the specified texture into memory from disk.
		/// </summary>
		/// <param name="asset">: enum identifier of the desired texture</param>
		/// <param name="filepath">: filepath to the texture</param>
		/// <returns>true if the function succeeded or false if it fails</returns>
		bool privLoad(TextureEnum asset, std::string filepath);

		/// <summary>
		/// Private implementation of the TextureLibrary::Load() function.
		/// Attempts to release the specified texture from memory. 
		/// </summary>
		/// <param name="asset">: enum identifier of the desired texture</param>
		/// <returns>true if a texture was released from memory or false if it wasn't
		///				loaded in the first place.</returns>
		bool privFree(TextureEnum asset);

	public:
		/// <summary>
		/// <para>Releases all textures in memory, and frees the TextureLibrary instance.</para>
		/// <para>Should only be called at the end of the applications lifetime!</para>
		/// </summary>
		/// <returns>true if the library was successfully released, false if it
		//				wasn't initialised to begin with</returns>
		bool Release();

		/// <summary>
		/// Attempts to retrieve the specified texture from memory.
		/// </summary>
		/// <param name="asset">: enum identifier of the desired texture</param>
		/// <param name="out">: output reference to return the retrieved texture</param>
		/// <returns>true if the function succeeded or false if it fails</returns>
		bool Get(TextureEnum asset, AssetType::Texture* out)
		{
			return Instance()->privGet(asset, out);
		}

		/// <summary>
		/// Attempts to load the specified texture into memory from disk.
		/// </summary>
		/// <param name="asset">: enum identifier of the desired texture</param>
		/// <returns>true if the function succeeded or false if it fails</returns>
		bool Load(TextureEnum asset, std::string filepath)
		{
			return Instance()->privLoad(asset, filepath);
		}

		/// <summary>
		/// Attempts to release the specified texture from memory. 
		/// </summary>
		/// <param name="asset">: enum identifier of the desired texture</param>
		/// <returns>true if a texture was released from memory or false if it wasn't
		///				loaded in the first place.</returns>
		bool Free(TextureEnum asset)
		{
			return Instance()->privFree(asset);
		}
	};
}
