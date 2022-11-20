#pragma once
#include <string>
#include <fstream>
#include <chrono>
#include <thread>

namespace rt
{
    namespace meter
    {

        struct result
        {
            std::string name;

            std::chrono::duration<double, std::micro> start;
            std::chrono::microseconds elapsed;
            std::thread::id thread_id;
        };

        struct session
        {
            std::string name;
        };

        class meter
        {
        public:
            static meter& instance();

            void reset(const std::string& name, const std::string& path);
            void out(const result& result);

        private:
            meter();
            ~meter();
            meter(const meter&) = delete;

            inline void close();
            inline void header();
            inline void footer();

            std::ofstream p_stream;

            bool p_session;
            session* p_current;
        };

        class timer
        {
        public:
            timer(const char* name);
            ~timer();

            void stop();

        private:
            const char* p_name;
            std::chrono::time_point<std::chrono::steady_clock> p_start;
            bool p_stopped;
        };

    }
}