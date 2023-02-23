#pragma once

#include <string>
#include <unordered_map>

#include "ShaderEnum.h"

namespace MCK
{
	namespace AssetType
	{
		class Shader;
	}

	/**
	* This class is the resource manager responsible for loading, retrieving, and
	*		releasing Shaders in memory.
	*/
	class ShaderLibrary
	{
	private:
		// private constructor/destructor because singleton
		ShaderLibrary();
		~ShaderLibrary();

		// not to be copied!
		ShaderLibrary(const ShaderLibrary&) = delete;
		ShaderLibrary& operator=(const ShaderLibrary&) = delete;

		// singleton bookkeeping
		static ShaderLibrary* instance;
		static ShaderLibrary* Instance()
		{
			if (!instance)
				instance = new ShaderLibrary;
			return instance;
		}

		// private member variables
		std::unordered_map<ShaderEnum, AssetType::Shader*> data;

		// private implementation of Singleton functions

		/**
		 * Private implementation of the ShaderLibrary::Get() function.
		 * Attempts to retrieve the specified shader from memory.
		 * 
		 * \param asset: enum identifier of the desired shader
		 * \param out: output reference to return the retrieved shader
		 * \return true if the function succeeded or false if it fails
		 */
		bool privGet(ShaderEnum asset, AssetType::Shader* out);

		/**
		 * Private implementation of the ShaderLibrary::Load() function.
		 * Attempts to load the specified shader into memory from disk.
		 * 
		 * \param asset: enum identifier of the desired shader
		 * \param filepath: filepath to the shader
		 * \return true if the function succeeded or false if it fails
		 */
		bool privLoad(ShaderEnum asset, std::string filepath);

		/**
		 * Private implementation of the ShaderLibrary::Load() function.
		 * Attempts to release the specified shader from memory. 
		 * 
		 * \param asset: enum identifier of the desired shader
		 * \return true if a shader was released from memory or false if it wasn't
		 *		loaded in the first place.
		 */
		bool privFree(ShaderEnum asset);

	public:
		/**
		 * Releases all shaders in memory, and frees the ShaderLibrary instance.
		 * Should only be called at the end of the applications lifetime!
		 * 
		 * \return true if the library was successfully released, false if it
		 *		wasn't initialised to begin with
		 */
		bool Release();

		/**
		 * Attempts to retrieve the specified shader from memory.
		 * 
		 * \param asset: enum identifier of the desired shader
		 * \param out: output reference to return the retrieved shader
		 * \return true if the function succeeded or false if it fails
		 */
		bool Get(ShaderEnum asset, AssetType::Shader* out)
		{
			return Instance()->privGet(asset, out);
		}

		/**
		 * Attempts to load the specified shader into memory from disk.
		 * 
		 * \param asset: enum identifier of the desired shader
		 * \param filepath: filepath to the shader
		 * \return true if the function succeeded or false if it fails
		 */
		bool Load(ShaderEnum asset, std::string filepath)
		{
			return Instance()->privLoad(asset, filepath);
		}

		/**
		 * Attempts to release the specified shader from memory.
		 * 
		 * \param asset: enum identifier of the desired shader
		 * \return true if a shader was released from memory or false if it wasn't
		 *		loaded in the first place.
		 */
		bool Free(ShaderEnum asset)
		{
			return Instance()->privFree(asset);
		}
	};
}
