#include "Input.h"
#include "core/Root.h"
#include "event/Event.h"

#include <GLFW/glfw3.h>

namespace rt
{

    input::input(root& root) : isystem(root)
    {
        p_window = p_root.get_system<window>();
    }

    bool input::is_key_pressed(int key_code)
    {
        int state = glfwGetKey(p_window->get_GLFWwindow(), key_code);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool input::is_mouse_key_pressed(int key_code)
    {
        int state = glfwGetMouseButton(p_window->get_GLFWwindow(), key_code);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    void input::init()
    {
        glfwSetKeyCallback(p_window->get_GLFWwindow(), [](GLFWwindow* window, int key_code, int scancode, int action, int mods) {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    evnt::key_press_event e(window, key_code, 0);
                    evnt::evnt::fire(evnt::rt_event_type::key_press_event, &e);
                    break;
                }
                case GLFW_REPEAT:
                {
                    evnt::key_press_event e(window, key_code, 1);
                    evnt::evnt::fire(evnt::rt_event_type::key_press_event, &e);
                    break;
                }
                case GLFW_RELEASE:
                {
                    evnt::key_release_event e(window, key_code);
                    evnt::evnt::fire(evnt::rt_event_type::key_release_event, &e);
                    break;
                }
            }
        });

        glfwSetCharCallback(p_window->get_GLFWwindow(), [](GLFWwindow* window, uint32_t key_code) {
            {
                evnt::key_type_event e(window, key_code);
                evnt::evnt::fire(evnt::rt_event_type::key_type_event, &e);
            }
            {
                evnt::char_event e(window, key_code);
                evnt::evnt::fire(evnt::rt_event_type::char_event, &e);
            }
        });

        glfwSetMouseButtonCallback(p_window->get_GLFWwindow(), [](GLFWwindow* window, int button, int action, int mods) {
            switch (action)
            {
                case GLFW_PRESS:
                {
                    evnt::mouse_button_press_event e(window, button);
                    evnt::evnt::fire(evnt::rt_event_type::mouse_button_press_event, &e);
                    break;
                }
                case GLFW_RELEASE:
                {
                    evnt::mouse_button_release_event e(window, button);
                    evnt::evnt::fire(evnt::rt_event_type::mouse_button_release_event, &e);
                    break;
                }
            }
        });

        glfwSetScrollCallback(p_window->get_GLFWwindow(), [](GLFWwindow* window, double x_offset, double y_offset) {
            evnt::mouse_scroll_event e(window, x_offset, y_offset);
            evnt::evnt::fire(evnt::rt_event_type::mouse_scroll_event, &e);
        });

        glfwSetCursorPosCallback(p_window->get_GLFWwindow(), [](GLFWwindow* window, double x, double y) {
            evnt::mouse_move_event e(window, x, y);
            evnt::evnt::fire(evnt::rt_event_type::mouse_move_event, &e);
        });
    }

}