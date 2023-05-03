# include "CacheReader.h"
#include "Paths.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>

namespace MCK::ResourceManagement
{
	bool TryReadNumber(std::string token, int* num)
	{
		// Used to check conversion
		char* p;
		// Convert
		long converted = strtol(token.c_str(), &p, 10);
		*num = (int)converted;
		
		return *p == 0;
	}

	std::string TrimPath(const std::string& path)
	{
		// Find the first charcater in the path
		auto pathBegin = path.find_first_not_of(' ');

		// If the path is just whitespace discard
		if (pathBegin == std::string::npos)
			return "";

		// Find the final character in the path
		auto pathEnd = path.find_last_not_of(' ');
		// Get range
		auto range = pathEnd - pathBegin + 1;

		return path.substr(pathBegin, range);
	}

	void ReadCache(const char* cachePath, std::map<int, std::string>& cacheOutput)
	{
		std::cout << "Current path is " << std::filesystem::current_path() << '\n'; 

		std::string line;
		std::ifstream cacheFile(cachePath);

		bool readEntry = false;
		int curId;

		while (std::getline(cacheFile, line))
		{
			std::stringstream tokenStream(line);
			std::string token;
			while (std::getline(tokenStream, token, ','))
			{
				if (readEntry)
				{
					cacheOutput[curId] = MCK::Paths::ROOT_PATH + TrimPath(token);
					readEntry = false;
				}

				if (TryReadNumber(token, &curId))
				{
					readEntry = true;
				}
			}


		}

		curId = 0;
	}
}
