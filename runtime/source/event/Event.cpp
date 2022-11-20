#include "Event.h"
#include "core/Log.h"
#include <algorithm>

namespace rt
{
    namespace evnt
    {

        uint64_t evnt_callback::p_unique_next = 0;

        evnt_callback::evnt_callback(std::function<void(rt_event*)> action)
            : p_action(action), p_unique(p_unique_next++) { }

        void evnt_callback::operator()(rt_event* payload)
        {
            return p_action(payload);
        }

        std::vector<evnt_callback> evnt::p_subscriptions[19];

        void evnt::subscribe(rt_event_type type, evnt_callback action)
        {
            RT_TRACE("Subscribing to event-type: {}", type);
            RT_ASSERT(type < rt_event_type::rt_event);
            RT_ASSERT(!(type < rt_event_type::app_tick_event));
            auto& s = p_subscriptions[static_cast<unsigned int>(type)];
            auto& it = std::find(s.begin(), s.end(), action);
            RT_ASSERT(it == s.end());
            s.push_back(action);
        }

        void evnt::unsubscribe(rt_event_type type, evnt_callback action)
        {
            RT_TRACE("Removing subscription from event-type: {}", type);
            RT_ASSERT(type < rt_event_type::rt_event);
            RT_ASSERT(!(type < rt_event_type::app_tick_event));
            auto& s = p_subscriptions[static_cast<unsigned int>(type)];
            auto& it = std::find(s.begin(), s.end(), action);
            if (it != s.end())
            {
                s.erase(it);
            }
            else
            {
                RT_WARN("Non existent event referenced");
            }
        }

        void evnt::fire(rt_event_type type, rt_event* evnt)
        {
            RT_ASSERT(type < rt_event_type::rt_event);
            RT_ASSERT(!(type < rt_event_type::app_tick_event));
            auto& s = p_subscriptions[static_cast<unsigned int>(type)];
            for (auto& action : s)
            {
                action(evnt);
            }
        }

        bool operator==(const evnt_callback& lhs, const evnt_callback& rhs)
        {
            return lhs.unique() == rhs.unique();
        }

    }
}
