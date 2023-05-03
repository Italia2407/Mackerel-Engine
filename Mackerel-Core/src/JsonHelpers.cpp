#include "LoggingSystem.h"
#include "JsonHelpers.h"

json MCK::Helpers::ParseJson(std::string path)
{
	// check the path is valid
	std::ifstream test(path);
	if (!test)
	{
		std::string message = "path not valid: " + path + ". Current directory:" + _getcwd(0,0);
		MCK::Logger::log(message, MCK::Logger::LogLevel::Debug, std::source_location::current());
		std::cerr << message << std::endl;
	}
	else
	{
		// load the file
		std::ifstream file(path);

		// try to parse the json file
		try {
			json data = json::parse(file);
			return data;
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	json empty = {};
	return empty;
}
