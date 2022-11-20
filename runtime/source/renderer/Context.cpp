#include "context.h"

#include "event/Event.h"
#include "core/Log.h"

#include "core/Window.h"

#include "core/Root.h"

#include <GLFW/glfw3.h>

namespace rt
{
    namespace gfx
    {

        void opengl_log_enable()
        {
            glDebugMessageCallback(opengl_log_message, nullptr);
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        }

        static opengl_log_level s_opengl_log_level = opengl_log_level::high_assert;

        void opengl_log_level_set(opengl_log_level level)
        {
            s_opengl_log_level = level;
        }

        void opengl_log_message(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* user_param)
        {
            switch (severity)
            {
            case GL_DEBUG_SEVERITY_HIGH:
                if ((int)s_opengl_log_level < 5)
                {
                    RT_ERROR("[OpenGL Debug HIGH] {0}", message);
                    if (s_opengl_log_level == opengl_log_level::high_assert)
                    {
                        RT_ASSERT(false, "OpenGL Debug HIGH");
                    }
                }
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                if ((int)s_opengl_log_level < 3)
                {
                    RT_WARN("[OpenGL Debug MEDIUM] {0}", message);
                }
                break;
            case GL_DEBUG_SEVERITY_LOW:
                if ((int)s_opengl_log_level < 2)
                {
                    RT_INFO("[OpenGL Debug LOW] {0}", message);
                }
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                if ((int)s_opengl_log_level < 1)
                {
                    RT_TRACE("[OpenGL Debug NOTIFICATION] {0}", message);
                }
                break;
            }
        }

        context::context(root& root) : isystem(root)
        {
        }

        void context::init()
        {
            std::shared_ptr<rt::window> win = p_root.get_system<window>();

            glfwMakeContextCurrent(win->get_GLFWwindow());
            int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

            RT_ASSERT(status);

            rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::viewport_resize_event, rt::evnt::evnt_callback([](rt::evnt::rt_event* payload) {
                    rt::evnt::viewport_resize_event* e = (rt::evnt::viewport_resize_event*)payload;
                    glViewport(0.0f, 0.0f, e->width, e->height);
                })
            );

#if RT_DEBUG_ENABLED
            opengl_log_enable();
#endif

            RT_TRACE("Context init:\n\tVersion: {}\n\tVendor: {}", glGetString(GL_VERSION), glGetString(GL_VENDOR));
        }

        void context::set_clear_color(math::vector4& color)
        {
            glClearColor(color.x, color.y, color.z, color.w);
        }

        void context::blend(bool state)
        {
            if (state)
            {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            else
            {
                glDisable(GL_BLEND);
            }
        }

        void context::backface_culling(bool state)
        {
            if (state)
            {
                glEnable(GL_CULL_FACE);
            }
            else
            {
                glDisable(GL_CULL_FACE);
            }
        }

        void context::depth(bool state)
        {
            if (state)
            {
                glEnable(GL_DEPTH_TEST);
            }
            else
            {
                glDisable(GL_DEPTH_TEST);
            }
        }

        void context::multisample(bool state)
        {
            if (state)
            {
                glEnable(GL_MULTISAMPLE);
            }
            else
            {
                glDisable(GL_MULTISAMPLE);
            }
        }

        void context::line_width(float value)
        {
            glLineWidth(value);
        }

        void context::clear()
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

    }
}