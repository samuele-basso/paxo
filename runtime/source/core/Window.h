#pragma once
#include "core/Configuration.h"
#include "core/ISystem.h"

struct GLFWwindow;

namespace rt
{

    class window : public isystem
    {
    public:
        window(root& root);
        window(const window&) = delete;

        virtual ~window();

        void init(const window_state& state);
        void shutdown();

        void show(const char* title, int width, int height, bool decorated, bool vsync);

        bool should_close();
        void update();

        void set_vsync(bool enabled);

        int is_iconified();

        int get_width()
        {
            return p_state.width;
        }
        int get_height()
        {
            return p_state.height;
        }

        GLFWwindow* get_GLFWwindow()
        {
            return p_window;
        }

        void capture_cursor(bool enabled);

    private:
        void init_callbacks();

        window_state p_state;
        GLFWwindow* p_window;
    };

}