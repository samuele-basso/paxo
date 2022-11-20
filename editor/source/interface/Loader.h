#pragma once

struct GLFWwindow;

namespace editor
{
    namespace gui
    {

        void init_api(GLFWwindow* window);
        void shutdown_api();
        void install_callback();
        void new_frame();
        void end_frame();
        void dock();

    }
}