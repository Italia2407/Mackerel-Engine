#include "LoggingSystem.h"

#include <iostream>

INITIALIZE_EASYLOGGINGPP


std::unordered_map<MCK::Logger::LogLevel, int> MCK::Logger::levelForeColours;
std::unordered_map<MCK::Logger::LogLevel, int> MCK::Logger::levelBackColours;
std::streambuf* MCK::Logger::coutBuffer;
std::streambuf* MCK::Logger::cerrBuffer;
std::streambuf* MCK::Logger::clogBuffer;
MCK::Logger::logStreambuf MCK::Logger::logBuffer;


namespace MCK {

    /**
    * Initialize logging system using EasyLogging++.
    *
    */
    void Logger::initialize() {

        el::Loggers::getLogger("USER");
        el::Loggers::getLogger("ENGINE");

        // Load configuration from file, if it exists
        el::Configurations conf("../Mackerel-Core/src/LoggingSystem.conf");
        el::Loggers::reconfigureAllLoggers(conf);

        el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
        el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
        el::Loggers::addFlag(el::LoggingFlag::CreateLoggerAutomatically);

        // Set up custom format specifier for colors
        el::Helpers::installCustomFormatSpecifier(el::CustomFormatSpecifier("%coloured", [](const el::LogMessage* message) -> std::string {
            // Get the log level and choose the color code accordingly
            if (message->level() == el::Level::Trace) {
                return "\033[38;5;" + std::to_string(levelForeColours[LogLevel::Basic]) + ";48;5;" + std::to_string(levelBackColours[LogLevel::Basic]) + "m";
            }
            else if (message->level() == el::Level::Debug) {
                return "\033[38;5;" + std::to_string(levelForeColours[LogLevel::Debug]) + ";48;5;" + std::to_string(levelBackColours[LogLevel::Debug]) + "m";
            }
            else if (message->level() == el::Level::Info) {
                return "\033[38;5;" + std::to_string(levelForeColours[LogLevel::Info]) + ";48;5;" + std::to_string(levelBackColours[LogLevel::Info]) + "m";
            }
            else if (message->level() == el::Level::Warning) {
                return "\033[38;5;" + std::to_string(levelForeColours[LogLevel::Warning]) + ";48;5;" + std::to_string(levelBackColours[LogLevel::Warning]) + "m";
            }
            else if (message->level() == el::Level::Error) {
                return "\033[38;5;" + std::to_string(levelForeColours[LogLevel::Error]) + ";48;5;" + std::to_string(levelBackColours[LogLevel::Error]) + "m";
            }
            else if (message->level() == el::Level::Fatal) {
                return "\033[38;5;" + std::to_string(levelForeColours[LogLevel::Fatal]) + ";48;5;" + std::to_string(levelBackColours[LogLevel::Fatal]) + "m";
            }
            else {
                return "\033[0m";
            }
        }));

        // set intial level colours
        levelForeColours[LogLevel::Basic] = 15; // Set the color for Trace level to White
        levelForeColours[LogLevel::Debug] = 46; // Set the color for Debug level to green
        levelForeColours[LogLevel::Info] = 33; // Set the color for Info level to blue
        levelForeColours[LogLevel::Warning] = 226; // Set the color for Warning level to yellow
        levelForeColours[LogLevel::Error] = 196; // Set the color for Error level to red
        levelForeColours[LogLevel::Fatal] = 201; // Set the color for Fatal level to magenta

        levelBackColours[LogLevel::Basic] = 0;
        levelBackColours[LogLevel::Debug] = 0;
        levelBackColours[LogLevel::Info] = 0;
        levelBackColours[LogLevel::Warning] = 0;
        levelBackColours[LogLevel::Error] = 0;
        levelBackColours[LogLevel::Fatal] = 0;

        coutBuffer = std::cout.rdbuf();
        cerrBuffer = std::cerr.rdbuf();
        clogBuffer = std::clog.rdbuf();
    }

    /**
    * Log a message at the specified level.
    *
    * \param message: The message to log.
    * \param level: The severity level to log the message at (defaults to Info).
    * \param filesource: std::source_location::current(), source calling log function
    * \param source: "USER" or "ENGINE", defaults to "USER" if not supplied
    */
    void Logger::log(const std::string& m, LogLevel level, std::source_location filesource, std::string source) {
        std::cout.rdbuf(coutBuffer);
        std::cerr.rdbuf(cerrBuffer);
        std::clog.rdbuf(clogBuffer);

        std::string message = m;
        message.erase(std::remove(message.begin(), message.end(), '\n'), message.end());
        if ((el::Loggers::hasLogger(source)) && (source == "USER")){
            switch (level) {
            case LogLevel::Info: CLOG(INFO, source.c_str()) << std::filesystem::path(filesource.file_name()).filename().string() << " [" << filesource.line() << "]: " << message; break;
            case LogLevel::Debug: CLOG(DEBUG, source.c_str()) << std::filesystem::path(filesource.file_name()).filename().string() << " [" << filesource.line() << "]: " << message; break;
            case LogLevel::Fatal: CLOG(FATAL, source.c_str()) << std::filesystem::path(filesource.file_name()).filename().string() << " [" << filesource.line() << "]: " << message; break;
            case LogLevel::Error: CLOG(ERROR, source.c_str()) << std::filesystem::path(filesource.file_name()).filename().string() << " [" << filesource.line() << "]: " << message; break;
            case LogLevel::Warning: CLOG(WARNING, source.c_str()) << std::filesystem::path(filesource.file_name()).filename().string() << " [" << filesource.line() << "]: " << message; break;
            default: CLOG(TRACE, source.c_str()) << std::filesystem::path(filesource.file_name()).filename().string() << " [" << filesource.line() << "]: " << message; break;
            }
            std::cout << "\033[0m";
        }
        else if(el::Loggers::hasLogger(source)){
            switch (level) {
            case LogLevel::Info: CLOG(INFO, source.c_str()) << "[]: " << message; break;
            case LogLevel::Debug: CLOG(DEBUG, source.c_str()) << "[]: " << message; break;
            case LogLevel::Fatal: CLOG(FATAL, source.c_str()) << "[]: " << message; break;
            case LogLevel::Error: CLOG(ERROR, source.c_str()) << "[]: " << message; break;
            case LogLevel::Warning: CLOG(WARNING, source.c_str()) << "[]: " << message; break;
            default: CLOG(TRACE, source.c_str()) << "[]: " << message; break;
            }
            std::cout << "\033[0m";
        } 

        // Redirect the standard output and error streams to our custom stream buffer
        std::cout.rdbuf(&logBuffer);
        std::cerr.rdbuf(&logBuffer);
        std::clog.rdbuf(&logBuffer);
    }
 
    /**
    * Change the foreground colour of a specified severity level
    *
    * \param r: red value 0-255
    * \param g: green value 0-255
    * \param b: blue value 0-255
    */
    void Logger::SetLevelForeColour(LogLevel level, int r, int g, int b) {
        levelForeColours[level] = rgbToColourCode(r, g, b);
    }

    /**
    * Change the background colour of a specified severity level
    *
    * \param r: red value 0-255
    * \param g: green value 0-255
    * \param b: blue value 0-255
    */
    void Logger::SetLevelBackColour(LogLevel level, int r, int g, int b) {
        levelBackColours[level] = rgbToColourCode(r, g, b);
    }

    /**
    * Calculates colour palette code from rgb values
    *
    * \param r: red value 0-255
    * \param g: green value 0-255
    * \param b: blue value 0-255
    */
    int Logger::rgbToColourCode(int r, int g, int b) {
        return 16 + (36 * r / 255) + (6 * g / 255) + (b / 255);
    }

    /**
    * Enable logging for a specific severity level.
    *
    * \param level: The severity level to enable logging for.
    */
    void Logger::EnableLogLevel(LogLevel level) {
        el::Configurations conf;
        switch (level) {
        case LogLevel::Info: conf.set(el::Level::Info, el::ConfigurationType::Enabled, "true"); break;
        case LogLevel::Debug: conf.set(el::Level::Debug, el::ConfigurationType::Enabled, "true"); break;
        case LogLevel::Fatal: conf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "true"); break;
        case LogLevel::Error: conf.set(el::Level::Error, el::ConfigurationType::Enabled, "true"); break;
        case LogLevel::Warning: conf.set(el::Level::Warning, el::ConfigurationType::Enabled, "true"); break;
        case LogLevel::Basic: conf.set(el::Level::Trace, el::ConfigurationType::Enabled, "true"); break;
        }
        el::Loggers::reconfigureAllLoggers(conf);
    }

    /**
    * Disable logging for a specific severity level.
    *
    * \param level: The severity level to disable logging for.
    */
    void Logger::DisableLogLevel(LogLevel level) {
        el::Configurations conf;
        switch (level) {
        case LogLevel::Info: conf.set(el::Level::Info, el::ConfigurationType::Enabled, "false"); break;
        case LogLevel::Debug: conf.set(el::Level::Debug, el::ConfigurationType::Enabled, "false"); break;
        case LogLevel::Fatal: conf.set(el::Level::Fatal, el::ConfigurationType::Enabled, "false"); break;
        case LogLevel::Error: conf.set(el::Level::Error, el::ConfigurationType::Enabled, "false"); break;
        case LogLevel::Warning: conf.set(el::Level::Warning, el::ConfigurationType::Enabled, "false"); break;
        case LogLevel::Basic: conf.set(el::Level::Trace, el::ConfigurationType::Enabled, "false"); break;
        }
        el::Loggers::reconfigureAllLoggers(conf);
    }

    /**
    * Enable logging for a specific source.
    *
    * \param source: The source to enable logging for.
    */
    void Logger::EnableSource(const std::string& source) {
        if ((source == "USER") && !(el::Loggers::hasLogger(source))) {
            el::Loggers::getLogger("USER");
            el::Configurations conf("../Mackerel-Core/src/LoggingSystem.conf");
            el::Loggers::reconfigureAllLoggers(conf);
        }
        else if (source == "ENGINE"){
            el::Loggers::getLogger("ENGINE");
            el::Configurations conf("../Mackerel-Core/src/LoggingSystem.conf");
            el::Loggers::reconfigureAllLoggers(conf);
        }
    }

    /**
    * Disable logging for a specific source.
    *
    * \param source: The source to disable logging for.
    */
    void Logger::DisableSource(const std::string& source) {
        if ((source == "USER") && (el::Loggers::hasLogger(source))) {
            el::Loggers::unregisterLogger("USER");
        }
        else if (source == "ENGINE") {
            el::Loggers::unregisterLogger("ENGINE");
        }
    }

    /**
    * Sets a level to save to the given filename instead of the default
    *
    * \param level: level to seperate
    * \param filename: The filename to save the level to
    */
    void Logger::SetLevelFilePath(LogLevel level, const std::string& filename) {
        // Load configuration from file, if it exists
        el::Configurations conf("../Mackerel-Core/src/LoggingSystem.conf");

        switch (level) {
        case LogLevel::Info: conf.set(el::Level::Info, el::ConfigurationType::Filename, "logs\\" + filename + ".log"); break;
        case LogLevel::Debug: conf.set(el::Level::Debug, el::ConfigurationType::Filename, "logs\\" + filename + ".log"); break;
        case LogLevel::Fatal: conf.set(el::Level::Fatal, el::ConfigurationType::Filename, "logs\\" + filename + ".log"); break;
        case LogLevel::Error: conf.set(el::Level::Error, el::ConfigurationType::Filename, "logs\\" + filename + ".log"); break;
        case LogLevel::Warning: conf.set(el::Level::Warning, el::ConfigurationType::Filename, "logs\\" + filename + ".log"); break;
        case LogLevel::Basic: conf.set(el::Level::Trace, el::ConfigurationType::Filename, "logs\\" + filename + ".log"); break;
        }
        el::Loggers::reconfigureAllLoggers(conf);
    }

    /**
    * Splits the the Engine and User streams into seperate log files
    *
    */
    void Logger::SplitEngineAndUserLogs() {
        // Load configuration from file, if it exists
        el::Configurations uconf("../Mackerel-Core/src/LoggingSystem.conf");
        el::Configurations econf("../Mackerel-Core/src/LoggingSystem.conf");

        uconf.set(el::Level::Global, el::ConfigurationType::Filename, "logs\\USER.log");
        econf.set(el::Level::Global, el::ConfigurationType::Filename, "logs\\ENGINE.log");

        el::Loggers::reconfigureLogger("USER", uconf);
        el::Loggers::reconfigureLogger("ENGINE", econf);
    }

    Logger::Logger() {
        // Private constructor to prevent object instantiation
    }

} // namespace MCK
