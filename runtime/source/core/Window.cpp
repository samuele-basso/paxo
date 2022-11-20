#include "Window.h"

#include "core/Platform.h"
#include "event/Event.h"

#include "core/Log.h"

#include <GLFW/glfw3.h>

namespace rt
{

    window::window(root& root) : isystem(root), p_window(nullptr)
    {
    }

    window::~window()
    {
        if (p_window)
        {
            glfwDestroyWindow(p_window);
        }
    }

    void window::init(const window_state& state)
    {
        p_window = nullptr;
        glfwInit();

        p_state = state;

        show(state.title, state.width, state.height, state.decorated, state.vsync);
    }

    void window::shutdown()
    {
        glfwTerminate();
    }

    void window::show(const char* title, int width, int height, bool decorated, bool vsync)
    {
        glfwWindowHint(GLFW_DECORATED, decorated);

#ifdef RT_ACTIVE_API_DEFAULT_OPENGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

        p_window = glfwCreateWindow(width, height, title, NULL, NULL);

        if (p_window == NULL)
        {
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(p_window);

        set_vsync(vsync);

        init_callbacks();
    }

    void window::init_callbacks()
    {
        glfwSetWindowIconifyCallback(p_window, [](GLFWwindow* window, int icon) {
            if (icon)
            {
                evnt::window_minimize_event e(window);
                evnt::evnt::fire(evnt::rt_event_type::window_minimize_event, &e);
            }
            else
            {
                evnt::window_unminimize_event e(window);
                evnt::evnt::fire(evnt::rt_event_type::window_unminimize_event, &e);
            }
        });

        glfwSetWindowSizeCallback(p_window, [](GLFWwindow* window, int width, int height) {
            if (glfwGetWindowAttrib(window, GLFW_ICONIFIED))
                return;
            evnt::window_resize_event e(window, width, height);
            evnt::evnt::fire(evnt::rt_event_type::window_resize_event, &e);
        });

        glfwSetWindowCloseCallback(p_window, [](GLFWwindow* window) {
            evnt::window_close_event e(window);
            evnt::evnt::fire(evnt::rt_event_type::window_close_event, &e);
        });

        glfwSetWindowFocusCallback(p_window, [](GLFWwindow* window, int focused) {
            switch (focused)
            {
            case GLFW_TRUE:
            {
                evnt::window_focus_event e(window);
                evnt::evnt::fire(evnt::rt_event_type::window_focus_event, &e);
                break;
            }
            case GLFW_FALSE:
            {
                evnt::window_lost_focus_event e(window);
                evnt::evnt::fire(evnt::rt_event_type::window_lost_focus_event, &e);
                break;
            }
            }
        });

        glfwSetWindowPosCallback(p_window, [](GLFWwindow* window, int xpos, int ypos) {
            evnt::window_move_event e(window, xpos, ypos);
            evnt::evnt::fire(evnt::rt_event_type::window_move_event, &e);
        });

        glfwSetCursorEnterCallback(p_window, [](GLFWwindow* window, int entered) {
            if (entered)
            {
                evnt::cursor_enter_event e(window);
                evnt::evnt::fire(evnt::rt_event_type::cursor_enter_event, &e);
            }
            else
            {
                evnt::cursor_exit_event e(window);
                evnt::evnt::fire(evnt::rt_event_type::cursor_exit_event, &e);
            }
        });
    }

    bool window::should_close()
    {
        return glfwWindowShouldClose(p_window);
    }

    void window::update()
    {
        glfwSwapBuffers(p_window);
        glfwPollEvents();
    }

    void window::set_vsync(bool f_Enabled)
    {
        if (f_Enabled)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }
    }

    int window::is_iconified()
    {
        return glfwGetWindowAttrib(p_window, GLFW_ICONIFIED);
    }

    void window::capture_cursor(bool f_Enabled)
    {
        if (f_Enabled)
        {
            glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else
        {
            glfwSetInputMode(p_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

}
