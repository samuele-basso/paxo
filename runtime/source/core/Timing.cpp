#include "Timing.h"
#include <GLFW/glfw3.h>

namespace rt
{

    timing::timing(root& root)
        : isystem(root), p_step(0), p_time_constructor(0)
    {
    }

    void timing::begin()
    {
        p_time_constructor = 0.0;
    }

    void timing::end()
    {
        double time = glfwGetTime();
        p_step = time - p_time_constructor;
        p_time_constructor = time;
    }

    double timing::get() const
    {
        return p_step;
    }

}