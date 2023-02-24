#pragma once

#ifndef LOGGINGSYSTEM_H
#define LOGGINGSYSTEM_H

#include <easyloggingpp-9.97.0/src/easylogging++.h>

namespace MCK {

    class Logger {
    public:

        // Initialise logging system using EasyLogging++
        static void initialize();

        // log messages at the specified level, defaults to Info
        static void log(const std::string& message, el::Level level);

        // set the minimum severity level to log
        static void setLogLevel(el::Level level);

        // Set the log file path
        static void SetLogFilePath(const std::string& filePath);

        // Enable/disable severity levels
        static void EnableLogLevel(el::Level level);
        static void DisableLogLevel(el::Level level);

        // Enable/disable sources
        static void EnableSource(const std::string& source);
        static void DisableSource(const std::string& source);

        // Choose to split levels into different files
        static void SetLogSplitting(bool splitLogs);

        // Intercept cout and cerr to format those streams too
        static void InterceptCoutAndCerr();

        // Set colours etc, add more functions
        static void SetLevelColour(el::Level level, const std::string& value);

        // add more functions

    private:
        // Private constructor to prevent object instantiation
        Logger();
    };

} // namespace MCK

#endif // LOGGINGSYSTEM_H