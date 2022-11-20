#pragma once

#include "core/ISystem.h"

#include <glad/glad.h>

#include "math/Vector4.h"

namespace rt
{
    namespace gfx
    {

        enum class opengl_log_level
        {
            notification = 0,
            low = 1,
            medium = 2,
            high = 3,
            high_assert = 4,
            none = 5
        };

        void opengl_log_message(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei lenght, const GLchar* message, const void* user_param);
        void opengl_log_enable();
        void opengl_log_level_set(opengl_log_level level);

        class context : public isystem
        {
        public:
            context(root& root);
            ~context() = default;
            context(const context&) = delete;

            virtual void init();

        public:
            static void set_clear_color(math::vector4& color);
            static void blend(bool state);
            static void backface_culling(bool state);
            static void depth(bool state);
            static void multisample(bool state);
            static void line_width(float value);

            static void clear();
        };

    }
}