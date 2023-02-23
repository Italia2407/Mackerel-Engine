#pragma once

#include <string>
#include <unordered_map>

#include "MaterialEnum.h"

namespace MCK
{
	namespace AssetType
	{
		class Material;
	}

	/**
	 * This class is the resource manager responsible for loading, retrieving, and
	 *	releasing Materials in memory.
	 */
	class MaterialLibrary
	{
		private:
			// private constructor/destructor because singleton
			MaterialLibrary();
			~MaterialLibrary();

			// not to be copied!
			MaterialLibrary(const MaterialLibrary&) = delete;
			MaterialLibrary& operator=(const MaterialLibrary&) = delete;

			// singleton bookkeeping
			static MaterialLibrary* instance;
			static MaterialLibrary* Instance()
			{
				if (!instance)
					instance = new MaterialLibrary;
				return instance;
			}

			// private member variables
			std::unordered_map<MaterialEnum, AssetType::Material*> data;

			// private implementation of Singleton functions
			
			/**
			 * Private implementation of the MaterialLibrary::Get() function.
			 * Attempts to retrieve the specified material from memory.
			 * 
			 * \param asset: enum identifier of the desired material
			 * \param out: output reference to return the retrieved material
			 * \return true if the function succeeded or false if it fails
			 */
			bool privGet(MaterialEnum asset, AssetType::Material* out);

			/**
			 * Private implementation of the MaterialLibrary::Load() function.
			 * Attempts to load the specified material into memory from disk.
			 * 
			 * \param asset: enum identifier of the desired material
			 * \param filepath: filepath to the material
			 * \return true if the function succeeded or false if it fails
			 */
			bool privLoad(MaterialEnum asset, std::string filepath);

			/**
			 * Private implementation of the MaterialLibrary::Load() function.
			 * Attempts to release the specified material from memory.
			 * 
			 * \param asset: enum identifier of the desired material
			 * \return true if a material was released from memory or false if it wasn't
			 *			loaded in the first place.
			 */
			bool privFree(MaterialEnum asset);

		public:
			/**
			 * Releases all materials in memory, and frees the MaterialLibrary instance.
			 * Should only be called at the end of the applications lifetime!
			 * 
			 * \return true if the library was successfully released, false if it
			 *	wasn't initialised to begin with
			 */
			bool Release();

			/**
			 * Attempts to retrieve the specified material from memory.
			 * 
			 * \param asset: enum identifier of the desired material
			 * \param out: output reference to return the retrieved material
			 * \return true if the function succeeded or false if it fails
			 */
			bool Get(MaterialEnum asset, AssetType::Material* out)
			{
				return Instance()->privGet(asset, out);
			}

			/**
			 * Attempts to load the specified material into memory from disk.
			 * 
			 * \param asset: enum identifier of the desired material
			 * \param filepath: filepath to the material
			 * \return true if the function succeeded or false if it fails
			 */
			bool Load(MaterialEnum asset, std::string filepath)
			{
				return Instance()->privLoad(asset, filepath);
			}

			/**
			 * Attempts to release the specified material from memory.
			 * 
			 * \param asset: enum identifier of the desired material
			 * \return true if a material was released from memory or false if it wasn't
			 *		loaded in the first place.
			 */
			bool Free(MaterialEnum asset)
			{
				return Instance()->privFree(asset);
			}
	};
}