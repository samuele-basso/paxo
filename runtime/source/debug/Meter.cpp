#include "Meter.h"

#include <assert.h>
#include <sstream>
#include <filesystem>

#include <core/Log.h>

namespace rt
{
    namespace meter
    {

        meter& meter::instance()
        {
            static meter instance;
            return instance;
        }

        meter::meter() : p_session(false), p_current(nullptr)
        {
        }

        void meter::reset(const std::string& name, const std::string& path)
        {
            if (p_session)
            {
                footer();
                close();
                RT_TRACE("{} meter session closed", p_current->name);
            }
            p_current = new session{ name };
            p_session = true;
            p_stream.open(path.c_str());
            RT_ASSERT(p_stream.is_open());
            RT_TRACE("{} meter session opened", p_current->name);
            header();
        }

        void meter::out(const result& result)
        {
            std::stringstream json;

            json << std::setprecision(3) << std::fixed;
            json << ",{";
            json << "\"cat\":\"function\",";
            json << "\"dur\":" << (result.elapsed.count()) << ',';
            json << "\"name\":\"" << result.name << "\",";
            json << "\"ph\":\"X\",";
            json << "\"pid\":0,";
            json << "\"tid\":" << result.thread_id << ",";
            json << "\"ts\":" << result.start.count();
            json << "}";

            if (p_session)
            {
                p_stream << json.str();
                p_stream.flush();
            }
        }

        meter::~meter()
        {
            if (p_session)
            {
                footer();
                close();
                RT_TRACE("{} meter session closed", p_current->name);
            }
        }

        void meter::header()
        {
            p_stream << "{\"otherData\": {},\"traceEvents\":[{}";
            p_stream.flush();
        }

        void meter::footer()
        {
            p_stream << "]}";
            p_stream.flush();
        }

        inline void meter::close()
        {
            p_stream.close();
            RT_ASSERT(!p_stream.is_open());
            delete p_current;
            p_current = nullptr;
            p_session = false;
        }

        timer::timer(const char* name) : p_name(name), p_stopped(false)
        {
            p_start = std::chrono::steady_clock::now();
        }

        timer::~timer()
        {
            if (!p_stopped)
            {
                stop();
            }
        }

        void timer::stop()
        {
            auto end = std::chrono::steady_clock::now();
            auto start = std::chrono::duration<double, std::micro>{ p_start.time_since_epoch() };
            auto elapsed = std::chrono::time_point_cast<std::chrono::microseconds>(end).time_since_epoch() -
                std::chrono::time_point_cast<std::chrono::microseconds>(p_start).time_since_epoch();

            meter::instance().out({ p_name, start, elapsed, std::this_thread::get_id() });

            p_stopped = true;
        }

    }
}