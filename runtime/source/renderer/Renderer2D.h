#pragma once
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"

#include "Texture.h"
#include "Buffer.h"
#include "Shader.h"
#include "Camera.h"

#include "memory/Memory.h"

#include "core/ISystem.h"
#include "core/Root.h"

#ifdef RT_DEBUG_ENABLED
#include "event/Event.h"
#include "event/Definitions.h"
#endif


namespace rt
{
    namespace gfx
    {

        struct quad
        {
            math::vector3 world_position;
            float texture_slot;
            math::vector4 color;
            math::vector2 texture_coordinates;
        };

        struct circle
        {
            math::vector3 world_position;
            float thickness;
            math::vector4 color;
            math::vector2 local_space;
            float fade;
        };

        struct line
        {
            math::vector3 world_position;
            math::vector4 color;
        };

        template <typename T> struct buffer
        {
            vertex_array va;
            vertex_buffer vb;
            index_buffer ib;

            T* vb_host_buffer;
            size_t vb_limit;
            size_t vb_top;

            unsigned int* ib_host_buffer;
            size_t ib_limit;
            size_t ib_top;
        };

        class renderer_2d : public isystem
        {
        public:
            renderer_2d(root& root) : isystem(root) { }
            renderer_2d(const renderer_2d&) = delete;

            void init();

            void draw_quad(const math::vector3& pos, const math::vector2& size, const math::vector3& rotation, const math::vector4& color);
            void draw_quad(const math::vector3& pos, const math::vector2& size, const math::vector3& rotation, const texture_2d& tex);
            void draw_quad(const math::vector3& pos, const math::vector2& size, const math::vector3& rotation, const texture_2d& tex, const math::vector4& color);

            void draw_line(const math::vector3& a, const math::vector3& b, const math::vector4& color); 
            void draw_line(const math::vector3& pos, const float len, const math::vector3& rotation, const math::vector4& color);

            void draw_circle(const math::vector3& pos, const math::vector2& size, const math::vector3& rotation, float thickness, float fade, const math::vector4& color);

            void direct_quad_draw();

            void flush();

            camera& cam() { return p_cam; }

        private:
            float get_texture_unit(texture_2d& tex);

            void shutdown();

            void flush_line();
            void flush_circle();
            void flush_quad();

            void check_quad();

            shader p_quad_shader { RT_SHADER_QUAD_VS, RT_SHADER_QUAD_FS };
            shader p_circle_shader { RT_SHADER_CIRCLE_VS, RT_SHADER_CIRCLE_FS };
            shader p_line_shader { RT_SHADER_LINE_VS, RT_SHADER_LINE_FS };

            buffer<quad> p_quad_buffer;
            buffer<circle> p_circle_buffer;
            buffer<line> p_line_buffer;

            size_t p_last_texture_slot;
            std::unique_ptr<texture_2d> p_white_default;
            int p_texture_slots[32];

            camera p_cam;

#ifdef RT_DEBUG_ENABLED
        public:
            class statistics
            {
            public:
                statistics() : quads(0), circles(0), lines(0), draws(0)
                {
                    rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::app_tick_event, rt::evnt::evnt_callback(
                        [&](rt::evnt::rt_event* e) {
                            quads = circles = lines = draws = 0;
                        })
                    );
                }

                static statistics& get()
                {
                    static statistics instance;
                    return instance;
                }

                int quads;
                int circles;
                int lines;
                int draws;
            };
#endif
        };

    }
}