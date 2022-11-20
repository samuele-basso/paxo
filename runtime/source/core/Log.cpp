#include "Log.h"

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace rt
{

    namespace log
    {
        void initialize(logger& logger, std::string& name)
        {
            std::vector<spdlog::sink_ptr> sinks;

            sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
            sinks[0]->set_pattern("%^[%T] [%n]: %v%$");

            sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(".log", true));
            sinks[1]->set_pattern("[%T] [%l] [%n]: %v");

            logger = std::make_shared<spdlog::logger>(name.c_str(), begin(sinks), end(sinks));
            spdlog::register_logger(logger);
            logger->set_level(spdlog::level::trace);
            logger->flush_on(spdlog::level::trace);
        }

        logger& runtime_logger::get()
        {
            static runtime_logger instance;
            return instance.p_logger;
        }

        runtime_logger::runtime_logger()
        {
            initialize(p_logger, std::string("RUNTIME"));
        }
    }

}