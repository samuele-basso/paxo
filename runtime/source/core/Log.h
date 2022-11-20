#pragma once
#include <memory>
#include <spdlog/spdlog.h>

namespace rt
{

    namespace log
    {
        using logger = std::shared_ptr<spdlog::logger>;

        void initialize(logger& logger, std::string& name);

        class runtime_logger
        {
        public:
            static logger& get();

        private:
            runtime_logger();
            logger p_logger;
        };
    }

}

#define RT_ERROR(...) rt::log::runtime_logger::get()->error(__VA_ARGS__)
#define RT_CRITICAL(...) rt::log::runtime_logger::get()->critical(__VA_ARGS__)

#ifdef RT_DEBUG_ENABLED

#define RT_TRACE(...) rt::log::runtime_logger::get()->trace(__VA_ARGS__)
#define RT_INFO(...) rt::log::runtime_logger::get()->info(__VA_ARGS__)
#define RT_WARN(...) rt::log::runtime_logger::get()->warn(__VA_ARGS__)

#define RT_ASSERT(condition, ...) assert(condition)
#define RT_STATIC_ASSERT(condition, ...) static_assert(condition)

#else

#define RT_TRACE(...)
#define RT_INFO(...)
#define RT_WARN(...)

#define RT_ASSERT(condition, ...)
#define RT_STATIC_ASSERT(condition, ...)

#endif
