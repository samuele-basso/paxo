#pragma once

#include "Definitions.h"

#include <functional>
#include <vector>

namespace rt
{
    namespace evnt
    {
        enum class rt_event_type
        {
            app_tick_event,
            char_event,
            cursor_enter_event,
            cursor_exit_event,
            key_press_event,
            key_release_event,
            key_type_event,
            mouse_button_press_event,
            mouse_button_release_event,
            mouse_move_event,
            mouse_scroll_event,
            viewport_resize_event,
            window_close_event,
            window_focus_event,
            window_lost_focus_event,
            window_minimize_event,
            window_move_event,
            window_resize_event,
            window_unminimize_event,
            rt_event
        };

        class evnt_callback
        {
        public:
            evnt_callback(std::function<void(rt_event*)> action);
            void operator()(rt_event* payload);

            inline const uint64_t unique() const { return p_unique; }

        private:
            std::function<void(rt_event*)> p_action;
            uint64_t p_unique;

        private:
            static uint64_t p_unique_next;
        };

        bool operator==(const evnt_callback& lhs, const evnt_callback& rhs);

        class evnt
        {
        public:
            static void subscribe(rt_event_type type, evnt_callback action);
            static void unsubscribe(rt_event_type type, evnt_callback action);

            static void fire(rt_event_type type, rt_event* evnt);

        private:
            static std::vector<evnt_callback> p_subscriptions[19];
        };
    }
}