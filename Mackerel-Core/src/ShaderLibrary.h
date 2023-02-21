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

	/// <summary>
	/// This class is the resource manager responsible for loading, retrieving, and
	///		releasing Shaders in memory.
	/// </summary>
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

		/// <summary>
		/// Private implementation of the ShaderLibrary::Get() function.
		/// Attempts to retrieve the specified shader from memory.
		/// </summary>
		/// <param name="asset">: enum identifier of the desired shader</param>
		/// <param name="out">: output reference to return the retrieved shader</param>
		/// <returns>true if the function succeeded or false if it fails</returns>
		bool privGet(ShaderEnum asset, AssetType::Shader* out);

		/// <summary>
		/// Private implementation of the ShaderLibrary::Load() function.
		/// Attempts to load the specified shader into memory from disk.
		/// </summary>
		/// <param name="asset">: enum identifier of the desired shader</param>
		/// <param name="filepath">: filepath to the shader</param>
		/// <returns>true if the function succeeded or false if it fails</returns>
		bool privLoad(ShaderEnum asset, std::string filepath);

		/// <summary>
		/// Private implementation of the ShaderLibrary::Load() function.
		/// Attempts to release the specified shader from memory. 
		/// </summary>
		/// <param name="asset">: enum identifier of the desired shader</param>
		/// <returns>true if a shader was released from memory or false if it wasn't
		///				loaded in the first place.</returns>
		bool privFree(ShaderEnum asset);

	public:
		/// <summary>
		/// <para>Releases all shaders in memory, and frees the ShaderLibrary instance.</para>
		/// <para>Should only be called at the end of the applications lifetime!</para>
		/// </summary>
		/// <returns>true if the library was successfully released, false if it
		//				wasn't initialised to begin with</returns>
		bool Release();

		/// <summary>
		/// Attempts to retrieve the specified shader from memory.
		/// </summary>
		/// <param name="asset">: enum identifier of the desired shader</param>
		/// <param name="out">: output reference to return the retrieved shader</param>
		/// <returns>true if the function succeeded or false if it fails</returns>
		bool Get(ShaderEnum asset, AssetType::Shader* out)
		{
			return Instance()->privGet(asset, out);
		}

		/// <summary>
		/// Attempts to load the specified shader into memory from disk.
		/// </summary>
		/// <param name="asset">: enum identifier of the desired shader</param>
		/// <returns>true if the function succeeded or false if it fails</returns>
		bool Load(ShaderEnum asset, std::string filepath)
		{
			return Instance()->privLoad(asset, filepath);
		}

		/// <summary>
		/// Attempts to release the specified shader from memory. 
		/// </summary>
		/// <param name="asset">: enum identifier of the desired shader</param>
		/// <returns>true if a shader was released from memory or false if it wasn't
		///				loaded in the first place.</returns>
		bool Free(ShaderEnum asset)
		{
			return Instance()->privFree(asset);
		}
	};
}
