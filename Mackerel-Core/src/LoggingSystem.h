#pragma once

#ifndef LOGGINGSYSTEM_H
#define LOGGINGSYSTEM_H

#include <easyloggingpp-9.97.0/src/easylogging++.h>
#include <source_location>
#include <filesystem>
#include <iostream>

namespace MCK {

    class Logger {
    public:

        enum class LogLevel {
            Basic,
            Debug,
            Info,
            Warning,
            Error,
            Fatal
        };

        // Initialise logging system using EasyLogging++
        static void initialize();

        // log messages at the specified level, defaults to Info
        static void log(const std::string& message, LogLevel level, std::source_location filesource, std::string source = "USER");

        // set the desired foreground colour for the given level
        static void SetLevelForeColour(LogLevel level, int r, int g, int b);

        // set the desired background colour for the given level
        static void SetLevelBackColour(LogLevel level, int r, int g, int b);

        // converts rgb value to colour code
        static int rgbToColourCode(int r, int g, int b);

        // set the minimum severity level to log
        static void setLogLevel(el::Level level);

        // Sets a level to save to the given filename instead of the default
        static void SetLevelFilePath(LogLevel level, const std::string& filename);

        // Splits the the Engine and User streams into seperate log files
        static void SplitEngineAndUserLogs();

        // Enable/disable severity levels
        static void EnableLogLevel(LogLevel level);
        static void DisableLogLevel(LogLevel level);

        // Enable/disable sources
        static void EnableSource(const std::string& source);
        static void DisableSource(const std::string& source);

        // class to intercept engine streams
        class logStreambuf : public std::stringbuf {
        public:

            int sync() override {
                std::string message = str();
                std::transform(message.begin(), message.end(), message.begin(), ::tolower);
#
                // changes the level depending on the message
                if (message.find("error") != std::string::npos) {
                    setLevel(LogLevel::Error);
                }else if (message.find("debug") != std::string::npos) {
                    setLevel(LogLevel::Debug);
                }else if (message.find("fatal") != std::string::npos) {
                    setLevel(LogLevel::Fatal);
                }else if (message.find("info") != std::string::npos) {
                    setLevel(LogLevel::Info);
                }else if (message.find("warning") != std::string::npos) {
                    setLevel(LogLevel::Warning);
                }else {
                    setLevel(LogLevel::Basic);
                }

                // Call the custom logger function with the log message
                Logger::log(str(), level, std::source_location::current(), "ENGINE");

                // Clear the string buffer for the next log message
                str("");
                return 0;
            }

            void setLevel(LogLevel l) { level = l; }

        private:
            LogLevel level = LogLevel::Info;
        };

    private:
        // Private constructor to prevent object instantiation
        Logger();

        // map between levels and colours
        static std::unordered_map<LogLevel, int> levelForeColours;
        static std::unordered_map<LogLevel, int> levelBackColours;

        static std::streambuf* coutBuffer;
        static std::streambuf* cerrBuffer;
        static std::streambuf* clogBuffer;
        static logStreambuf logBuffer;
    };

} // namespace MCK

#endif // LOGGINGSYSTEM_H