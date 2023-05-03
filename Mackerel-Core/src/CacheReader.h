#pragma once
#include <string>
#include <map>

namespace MCK::ResourceManagement
{
	void ReadCache(const char* cachePath, std::map<int, std::string>& cacheOutput);
}


