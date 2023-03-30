#include "JsonHelpers.h"
#include "LoggingSystem.h"

json MCK::Helpers::getJsonObject(std::string path)
{
	json data;

	// check the path is valid
	std::ifstream test(path);
	if (!test)
	{
		std::string message = "path not valid: " + path + ". Current directory:" + _getcwd(0, 0);
		MCK::Logger::log(message, MCK::Logger::LogLevel::Debug, std::source_location::current());
		std::cerr << message << std::endl;
	}
	else
	{
		// load the file
		std::ifstream file(path);

		// try to parse the json file
		try {
			data = json::parse(file);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	return data;
}