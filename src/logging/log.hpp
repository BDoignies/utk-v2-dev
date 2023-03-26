#pragma once

#ifdef UTK_LOG

#include <cstdarg>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace utk
{
    class Logger
    {
    private:
        Logger()
        {
            CreateConsoleLogger();
        }

        void CreateConsoleLogger()
        {
            auto default_sink = std::make_shared<spdlog::sinks::stderr_color_sink_st>();

            spdlog::sinks_init_list sink_list = { default_sink };
            logger = std::make_shared<spdlog::logger>("UTK_LOGGER", sink_list.begin(), sink_list.end());
            logger->set_pattern("[UTK] %l: %v");

            EnableLog();
        }

        void CreateFileLogger(const std::string& file)
        {
            auto default_sink = std::make_shared<spdlog::sinks::basic_file_sink_st>(file);

            spdlog::sinks_init_list sink_list = { default_sink };
            logger = std::make_shared<spdlog::logger>("UTK_LOGGER", sink_list.begin(), sink_list.end());
            logger->set_pattern("[UTK] %l: %v");

            EnableLog();
        }
    public:
        Logger(Logger const&)          = delete;
        void operator=(Logger const&)  = delete;
    
    public:
        std::shared_ptr<spdlog::logger> logger; // Public ... Why no idea, but why not ?
                                                // Not static just to be sure it is properly constructed 
                                                // but without cpp file :)

        static void DisableLog()
        {
            Logger& log = Logger::instance();
            log.logger->set_level(spdlog::level::off);
        }

        static void EnableLog()
        {
            Logger& log = Logger::instance();
            log.logger->set_level(spdlog::level::trace);
        }

        static void SetFileLogging(const std::string& file)
        {
            Logger& log = Logger::instance();
            log.CreateFileLogger(file);
        }

        static void SetConsoleLogging()
        {
            Logger& log = Logger::instance();
            log.CreateConsoleLogger();
        }
        
        static Logger& instance()
        {
            static Logger log;
            return log;
        }
    };
}
    #ifdef UTK_LOG_NO_DEBUG
        #define UTK_DEBUG(message, ...) ;
    #else
        #define UTK_DEBUG(message, ...) utk::Logger::instance().logger->debug(message, __VA_ARGS__);
    #endif
    #define UTK_LOG_DISABLE()       utk::Logger::DisableLog();
    #define UTK_LOG_ENABLE()        utk::Logger::EnableLog();
    #define UTK_LOG_FILE(file)      utk::Logger::SetFileLogging(file);
    #define UTK_LOG_CONSOLE()       utk::Logger::SetConsoleLogging();
    #define UTK_INFO(message, ...)  utk::Logger::instance().logger->info (message, __VA_ARGS__);
    #define UTK_WARN(message, ...)  utk::Logger::instance().logger->warn (message, __VA_ARGS__);
    #define UTK_ERROR(message, ...) utk::Logger::instance().logger->error(message, __VA_ARGS__);
#else
    #define UTK_LOG_DISABLE()  ;
    #define UTK_LOG_ENABLE()   ;
    #define UTK_LOG_FILE(file) ;
    #define UTK_LOG_CONSOLE()  ;
    #define UTK_DEBUG(message, ...) ;
    #define UTK_INFO(message, ...)  ;
    #define UTK_WARN(message, ...)  ;
    #define UTK_ERROR(message, ...) ;
#endif