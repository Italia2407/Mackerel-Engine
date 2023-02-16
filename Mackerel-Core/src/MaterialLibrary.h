#pragma once

#include <unordered_map>

#include "Material.h"
#include "MaterialEnum.h"

namespace MCK
{
	/// <summary>
	/// This class is the resource manager responsible for loading, retrieving, and
	///		releasing Materials in memory.
	/// </summary>
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
			std::unordered_map<MaterialEnum, AssetType::Material> data;

			// private implementation of Singleton functions
			
			/// <summary>
			/// Private implementation of the MaterialLibrary::Get() function.
			/// Attempts to retrieve the specified material from memory.
			/// </summary>
			/// <param name="asset">: enum identifier of the desired material</param>
			/// <param name="out">: output reference to return the retrieved material</param>
			/// <returns>true if the function succeeded or false if it fails</returns>
			bool privGet(MaterialEnum asset, AssetType::Material* out);

			/// <summary>
			/// Private implementation of the MaterialLibrary::Load() function.
			/// Attempts to load the specified material into memory from disk.
			/// </summary>
			/// <param name="asset">: enum identifier of the desired material</param>
			/// <returns>true if the function succeeded or false if it fails</returns>
			bool privLoad(MaterialEnum asset);

			/// <summary>
			/// Private implementation of the MaterialLibrary::Load() function.
			/// Attempts to release the specified material from memory. 
			/// </summary>
			/// <param name="asset">: enum identifier of the desired material</param>
			/// <returns>true if a material was released from memory or false if it wasn't
			///				loaded in the first place.</returns>
			bool privFree(MaterialEnum asset);

		public:

			/// <summary>
			/// <para>Releases all material in memory, and frees the MaterialLibrary instance.</para>
			/// <para>Should only be called at the end of the applications lifetime!</para>
			/// </summary>
			void Release();

			/// <summary>
			/// Attempts to retrieve the specified material from memory.
			/// </summary>
			/// <param name="asset">: enum identifier of the desired material</param>
			/// <param name="out">: output reference to return the retrieved material</param>
			/// <returns>true if the function succeeded or false if it fails</returns>
			bool Get(MaterialEnum asset, AssetType::Material* out)
			{
				return Instance()->privGet(asset, out);
			}

			/// <summary>
			/// Attempts to load the specified material into memory from disk.
			/// </summary>
			/// <param name="asset">: enum identifier of the desired material</param>
			/// <returns>true if the function succeeded or false if it fails</returns>
			bool Load(MaterialEnum asset)
			{
				return Instance()->privLoad(asset);
			}

			/// <summary>
			/// Attempts to release the specified material from memory. 
			/// </summary>
			/// <param name="asset">: enum identifier of the desired material</param>
			/// <returns>true if a material was released from memory or false if it wasn't
			///				loaded in the first place.</returns>
			bool Free(MaterialEnum asset)
			{
				return Instance()->privFree(asset);
			}
	};
}