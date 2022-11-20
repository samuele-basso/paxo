#pragma once

struct GLFWwindow;

namespace rt
{
    namespace evnt
    {

        struct rt_event
        {

        };

        struct cursor_enter_event : public rt_event
        {
            cursor_enter_event(GLFWwindow* window) : window(window)
            {
            }

            GLFWwindow* window;
        };

        struct cursor_exit_event : public rt_event
        {
            cursor_exit_event(GLFWwindow* window) : window(window)
            {
            }

            GLFWwindow* window;
        };

        struct window_close_event : public rt_event
        {
            window_close_event(GLFWwindow* window) : window(window)
            {
            }

            GLFWwindow* window;
        };

        struct window_focus_event : public rt_event
        {
            window_focus_event(GLFWwindow* window) : window(window)
            {
            }

            GLFWwindow* window;
        };

        struct window_lost_focus_event : public rt_event
        {
            window_lost_focus_event(GLFWwindow* window) : window(window)
            {
            }

            GLFWwindow* window;
        };

        struct window_minimize_event : public rt_event
        {
            window_minimize_event(GLFWwindow* window) : window(window)
            {
            }

            GLFWwindow* window;
        };

        struct window_unminimize_event : public rt_event
        {
            window_unminimize_event(GLFWwindow* window) : window(window)
            {
            }

            GLFWwindow* window;
        };

        struct window_resize_event : public rt_event
        {
            window_resize_event(GLFWwindow* window, unsigned int width, unsigned int height)
                : window(window), width(width), height(height)
            {
            }

            GLFWwindow* window;
            unsigned int width, height;
        };

        struct window_move_event : public rt_event
        {
            window_move_event(GLFWwindow* window, float x_pos, float y_pos) : window(window), x_pos(x_pos), y_pos(y_pos)
            {
            }

            GLFWwindow* window;
            float x_pos, y_pos;
        };

        struct viewport_resize_event : public rt_event
        {
            viewport_resize_event(GLFWwindow* window, unsigned int width, unsigned int height)
                : window(window), width(width), height(height)
            {
            }

            GLFWwindow* window;
            unsigned int width, height;
        };

        struct mouse_move_event : public rt_event
        {
            mouse_move_event(GLFWwindow* window, float x, float y) : window(window), x(x), y(y)
            {
            }

            GLFWwindow* window;
            float x, y;
        };

        struct mouse_scroll_event : public rt_event
        {
            mouse_scroll_event(GLFWwindow* window, float x_offset, float y_offset)
                : window(window), x_offset(x_offset), y_offset(y_offset)
            {
            }

            GLFWwindow* window;
            float x_offset, y_offset;
        };

        struct mouse_button_press_event : public rt_event
        {
            mouse_button_press_event(GLFWwindow* window, int button) : window(window), button(button)
            {
            }

            GLFWwindow* window;
            int button;
        };

        struct mouse_button_release_event : public rt_event
        {
            mouse_button_release_event(GLFWwindow* window, int button) : window(window), button(button)
            {
            }

            GLFWwindow* window;
            int button;
        };

        struct char_event : public rt_event
        {
            char_event(GLFWwindow* window, unsigned int code_point) : window(window), code_point(code_point)
            {
            }

            GLFWwindow* window;
            unsigned int code_point;
        };

        struct key_press_event : public rt_event
        {
            key_press_event(GLFWwindow* window, int key_code, int repeat) : window(window), key_code(key_code), repeat(repeat)
            {
            }

            GLFWwindow* window;
            int key_code;
            int repeat;
        };

        struct key_release_event : public rt_event
        {
            key_release_event(GLFWwindow* window, int key_code) : window(window), key_code(key_code)
            {
            }

            GLFWwindow* window;
            int key_code;
        };

        struct key_type_event : public rt_event
        {
            key_type_event(GLFWwindow* window, int key_code) : window(window), key_code(key_code)
            {
            }

            GLFWwindow* window;
            int key_code;
        };

        struct app_tick_event : public rt_event
        {
            app_tick_event(GLFWwindow* window) : window(window)
            {
            }

            GLFWwindow* window;
        };

    }
}