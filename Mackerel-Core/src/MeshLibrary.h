#pragma once

#include <string>
#include <unordered_map>

#include "MeshEnum.h"

namespace MCK
{
	namespace AssetType
	{
		class Mesh;
	}

	/**
	 * This class is the resource manager responsible for loading, retrieving, and
	 * releasing Meshes in memory.
	 */
	class MeshLibrary
	{
	private:
		// private constructor/destructor because singleton
		MeshLibrary();
		~MeshLibrary();

		// not to be copied!
		MeshLibrary(const MeshLibrary&) = delete;
		MeshLibrary& operator=(const MeshLibrary&) = delete;

		// singleton bookkeeping
		static MeshLibrary* instance;
		static MeshLibrary* Instance()
		{
			if (!instance)
				instance = new MeshLibrary;
			return instance;
		}

		// private member variables
		std::unordered_map<MeshEnum, AssetType::Mesh*> data;

		// private implementation of Singleton functions

		/**
		 * Private implementation of the MeshLibrary::Get() function.
		 * Attempts to retrieve the specified mesh from memory.
		 * 
		 * \param asset: enum identifier of the desired mesh
		 * \param out: output reference to return the retrieved mesh
		 * \return true if the function succeeded or false if it fails
		 */
		bool privGet(MeshEnum asset, AssetType::Mesh* out);

		/**
		 * Private implementation of the MeshLibrary::Load() function.
		 * Attempts to load the specified mesh into memory from disk.
		 * 
		 * \param asset: enum identifier of the desired mesh
		 * \param filepath: filepath to the mesh
		 * \return true if the function succeeded or false if it fails
		 */
		bool privLoad(MeshEnum asset, std::string filepath);

		/**
		 * Private implementation of the MeshLibrary::Load() function.
		 * Attempts to release the specified mesh from memory. 
		 * 
		 * \param asset: enum identifier of the desired mesh
		 * \return true if a mesh was released from memory or false if it wasn't
		 *		loaded in the first place.
		 */
		bool privFree(MeshEnum asset);

	public:
		/**
		 * Releases all meshes in memory, and frees the MeshLibrary instance.
		 * Should only be called at the end of the applications lifetime!
		 * 
		 * \return true if the library was successfully released, false if it
		 *		wasn't initialised to begin with
		 */
		bool Release();

		/**
		 * Attempts to retrieve the specified mesh from memory.
		 * 
		 * \param asset: enum identifier of the desired mesh
		 * \param out: output reference to return the retrieved mesh
		 * \return true if the function succeeded or false if it fails
		 */
		bool Get(MeshEnum asset, AssetType::Mesh* out)
		{
			return Instance()->privGet(asset, out);
		}

		/**
		 * Attempts to load the specified mesh into memory from disk.
		 * 
		 * \param asset: enum identifier of the desired mesh
		 * \param filepath: filepath to the mesh
		 * \return true if the function succeeded or false if it fails
		 */
		bool Load(MeshEnum asset, std::string filepath)
		{
			return Instance()->privLoad(asset, filepath);
		}

		/// <summary>
		/// Attempts to release the specified mesh from memory. 
		/// </summary>
		/// <param name="asset">: enum identifier of the desired mesh</param>
		/// <returns>true if a mesh was released from memory or false if it wasn't
		///				loaded in the first place.</returns>
		/**
		 * Attempts to release the specified mesh from memory.
		 * 
		 * \param asset: enum identifier of the desired mesh
		 * \return true if a mesh was released from memory or false if it wasn't
		 *		loaded in the first place.
		 */
		bool Free(MeshEnum asset)
		{
			return Instance()->privFree(asset);
		}
	};
}

