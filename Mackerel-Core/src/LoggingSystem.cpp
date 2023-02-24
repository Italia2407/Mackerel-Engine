#include "LoggingSystem.h"

#include <iostream>

INITIALIZE_EASYLOGGINGPP

namespace MCK {

    /**
    * Initialize logging system using EasyLogging++.
    *
    */
    void Logger::initialize() {
        // Load configuration from file, if it exists
        //el::Configurations conf("logging.conf");
        //el::Loggers::reconfigureAllLoggers(conf);
        el::Configurations defaultConf;
        defaultConf.setToDefault();
    }

    /**
    * Log a message at the specified level.
    *
    * \param message: The message to log.
    * \param level: The severity level to log the message at (defaults to Info).
    */
    void Logger::log(const std::string& message, el::Level level) {
        LOG(WARNING) << message;
    }

    /**
    * Set the minimum severity level to log.
    *
    * \param level: The minimum severity level to log.
    */
    void Logger::setLogLevel(el::Level level) {
        el::Loggers::setLoggingLevel(level);
    }

    /**
    * Set the log file path.
    *
    * \param filePath: The file path to save the log file.
    */
    void Logger::SetLogFilePath(const std::string& filePath) {
        el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
        el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
        el::Loggers::addFlag(el::LoggingFlag::CreateLoggerAutomatically);
        el::Loggers::reconfigureAllLoggers(el::ConfigurationType::Filename, filePath);
    }

    /**
    * Enable logging for a specific severity level.
    *
    * \param level: The severity level to enable logging for.
    */
    void Logger::EnableLogLevel(el::Level level) {
        //el::Loggers::addFlag(el::LevelHelper::castToInt(level), el::LoggingFlag::Enabled);
    }

    /**
    * Disable logging for a specific severity level.
    *
    * \param level: The severity level to disable logging for.
    */
    void Logger::DisableLogLevel(el::Level level) {
        //el::Loggers::removeFlag(el::LevelHelper::castToInt(level), el::LoggingFlag::Enabled);
    }

    /**
    * Enable logging for a specific source.
    *
    * \param source: The source to enable logging for.
    */
    void Logger::EnableSource(const std::string& source) {
        //el::Loggers::getLogger(source)->setEnabled(true);
    }

    /**
    * Disable logging for a specific source.
    *
    * \param source: The source to disable logging for.
    */
    void Logger::DisableSource(const std::string& source) {
        //el::Loggers::getLogger(source)->setEnabled(false);
    }

    /**
    * Choose to split levels into different files based on severity
    *
    * \param splitLogs: Whether or not to split logs into separate files.
    */
    void Logger::SetLogSplitting(bool splitLogs) {
        if (splitLogs) {
            el::Loggers::addFlag(el::LoggingFlag::MultiLoggerSupport);
        }
        else {
            el::Loggers::removeFlag(el::LoggingFlag::MultiLoggerSupport);
        }
    }

    /**
    * Enable interception cout and cerr to format those streams too.
    *
    */
    void Logger::InterceptCoutAndCerr() {

        // Create new logging streams for std::cout and std::cerr
        auto coutStream = el::Loggers::getLogger("cout");
        auto cerrStream = el::Loggers::getLogger("cerr");

        // Set log level for these streams
        //coutStream->setLevel(el::Level::Info);
        //cerrStream->setLevel(el::Level::Error);

        // Redirect std::cout and std::cerr to the logging streams
        //el::Helpers::installLogDispatchCallback<el::LogDispatchCallback>("CoutDispatchCallback", [](const el::LogMessage* message) {
            //if (message->logger()->id() == "cout") {
               // std::cout << message->message().c_str() << std::endl;
            //}
            //});
        //el::Helpers::installLogDispatchCallback<el::LogDispatchCallback>("CerrDispatchCallback", [](const el::LogMessage* message) {
            //if (message->logger()->id() == "cerr") {
                //std::cerr << message->message().c_str() << std::endl;
            //}
            //});
    }

    /**
    * Disable interception cout and cerr to format those streams too.
    *
    */
    //void Logger::DisableInterceptCoutAndCerr() {
        // Restore original rdbuf
        //std::cout.rdbuf(m_originalCoutBuffer);
        //std::cerr.rdbuf(m_originalCerrBuffer);
    //}

    /**
    * Change the colour of a specified severity level
    *
    * \param level: severity level to change the colour of
    * \param value: colour e.g. "red", "magenta"
    */
    //void Logger::SetColorForLevel(el::Level level, el::ConfigurationType cfgType, const std::string& value)
    //{
        // Get the logger's Configurations
        //el::Configurations* loggerConfigurations = el::Loggers::getLogger("default")->configurations();

        // Set the color for the specified level in the logger's Configurations
        //loggerConfigurations->set(level, el::ConfigurationType::Format, value);
    //}

    //Logger::Logger() {
        // Private constructor to prevent object instantiation
    //}

} // namespace MCK
