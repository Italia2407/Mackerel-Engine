#pragma once

#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

namespace MCK::Helpers
{
	/**
	 * Helper function to get json object from .json file path.
	 * 
	 * \param path: string of the path to the .json file
	 * 
	 * \return json object, either empty or read from file
	 */
	json getJsonObject(std::string path);
}
