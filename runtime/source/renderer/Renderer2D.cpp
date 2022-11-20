#include "Renderer2D.h"

#include "math/Matrix.h"

#include <glad/glad.h>

#define RT_LIMIT 10000

namespace rt
{
    namespace gfx
    {
        void renderer_2d::init()
        {
            p_quad_buffer.vb_host_buffer = (quad*)malloc(RT_LIMIT * sizeof(quad));
            p_quad_buffer.vb_limit = RT_LIMIT;
            p_quad_buffer.vb_top = 0;

            p_quad_buffer.ib_host_buffer = (unsigned int*)malloc(6 * RT_LIMIT * sizeof(unsigned int));
            p_quad_buffer.ib_limit = RT_LIMIT;
            p_quad_buffer.ib_top = 0;


            p_circle_buffer.vb_host_buffer = (circle*)malloc(RT_LIMIT * sizeof(circle));
            p_circle_buffer.vb_limit = RT_LIMIT;
            p_circle_buffer.vb_top = 0;

            p_circle_buffer.ib_host_buffer = (unsigned int*)malloc(6 * RT_LIMIT * sizeof(unsigned int));
            p_circle_buffer.ib_limit = RT_LIMIT;
            p_circle_buffer.ib_top = 0;


            p_line_buffer.vb_host_buffer = (line*)malloc(RT_LIMIT * sizeof(line));
            p_line_buffer.vb_limit = RT_LIMIT;
            p_line_buffer.vb_top = 0;

            p_line_buffer.ib_host_buffer = (unsigned int*)malloc(2 * RT_LIMIT * sizeof(unsigned int));
            p_line_buffer.ib_limit = RT_LIMIT;
            p_line_buffer.ib_top = 0;


            p_quad_shader.bind();

            for (size_t i = 0; i < 32; i++)
            {
                p_texture_slots[i] = i;
            }

            p_quad_shader.u_1iv("u_textures", 32, p_texture_slots);

            p_quad_buffer.va.bind_vertex_buffer(
                &p_quad_buffer.vb,
                buffer_layout({ shader_data_type::float_3, shader_data_type::float_1, shader_data_type::float_4, shader_data_type::float_2 })
            );
            p_quad_buffer.va.bind_index_buffer(
                &p_quad_buffer.ib
            );

            rt::gfx::texture_parameter parameters[] = {
                rt::gfx::texture_parameter::filter_min,
                rt::gfx::texture_parameter::filter_mag,
                rt::gfx::texture_parameter::wrap_s,
                rt::gfx::texture_parameter::wrap_t
            };

            rt::gfx::texture_parameter_value values[] = {
                rt::gfx::texture_parameter_value::nearest,
                rt::gfx::texture_parameter_value::nearest,
                rt::gfx::texture_parameter_value::clamp_to_edge,
                rt::gfx::texture_parameter_value::clamp_to_edge
            };

            p_white_default = std::make_unique<texture_2d>(parameters, values, 4, rt::math::vector4(1.0f));
            p_white_default->bind(0);

            p_last_texture_slot = 1;

            p_circle_buffer.va.bind_vertex_buffer(
                &p_circle_buffer.vb,
                buffer_layout({ shader_data_type::float_3, shader_data_type::float_1, shader_data_type::float_4, shader_data_type::float_2, shader_data_type::float_1 })
            );
            p_circle_buffer.va.bind_index_buffer(
                &p_circle_buffer.ib
            );


            p_line_buffer.va.bind_vertex_buffer(
                &p_line_buffer.vb,
                buffer_layout({ shader_data_type::float_3, shader_data_type::float_4 })
            );

            RT_TRACE("Renderer init:\n\tLimit: {}", RT_LIMIT);
        }

        float renderer_2d::get_texture_unit(texture_2d& tex)
        {
            for (int i = 1; i < p_last_texture_slot; i++)
            {
                if (tex.get_id() == p_texture_slots[i])
                {
                    return i;
                }
            }

            int slot = p_last_texture_slot;
            p_texture_slots[p_last_texture_slot] = tex.get_id();
            p_last_texture_slot++;
            tex.bind(slot);
            return slot;
        }

        void renderer_2d::draw_quad(const math::vector3& pos, const math::vector2& size, const math::vector3& rotation, const math::vector4& color)
        {
#ifdef RT_DEBUG_ENABLED
            statistics::get().quads++;
#endif

            rt::math::matrix scale = rt::math::matrix::scale({ size.x, size.y, 1 });
            rt::math::matrix rotate = rt::math::matrix::rotate(rt::math::attitude_to_quaternion(rotation.x, rotation.y, rt::math::radians(rotation.z)));
            rt::math::matrix translate = rt::math::matrix::translate(pos);

            rt::math::matrix transform = translate * rotate * scale;

            check_quad();

            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top;
            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top + 1;
            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top + 2;
            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top + 1;
            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top + 2;
            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top + 3;

            p_quad_buffer.vb_host_buffer[p_quad_buffer.vb_top++] = quad{ rt::math::vector3(transform * rt::math::vector4(-0.5f, -0.5f, 0.0f, 1.0f)), 0.0f, color, { 0.0f, 0.0f } };
            p_quad_buffer.vb_host_buffer[p_quad_buffer.vb_top++] = quad{ rt::math::vector3(transform * rt::math::vector4( 0.5f, -0.5f, 0.0f, 1.0f)), 0.0f, color, { 1.0f, 0.0f } };
            p_quad_buffer.vb_host_buffer[p_quad_buffer.vb_top++] = quad{ rt::math::vector3(transform * rt::math::vector4(-0.5f,  0.5f, 0.0f, 1.0f)), 0.0f, color, { 0.0f, 1.0f } };
            p_quad_buffer.vb_host_buffer[p_quad_buffer.vb_top++] = quad{ rt::math::vector3(transform * rt::math::vector4( 0.5f,  0.5f, 0.0f, 1.0f)), 0.0f, color, { 1.0f, 1.0f } };
        }

        void renderer_2d::draw_quad(const math::vector3& pos, const math::vector2& size, const math::vector3& rotation, const texture_2d& tex)
        {
#ifdef RT_DEBUG_ENABLED
            statistics::get().quads++;
#endif

            rt::math::matrix scale = rt::math::matrix::scale({ size.x, size.y, 0 });
            rt::math::matrix rotate = rt::math::matrix::rotate(rt::math::attitude_to_quaternion(rotation.x, rotation.y, rt::math::radians(rotation.z)));
            rt::math::matrix translate = rt::math::matrix::translate(pos);

            rt::math::matrix transform = translate * rotate * scale;

            check_quad();

            float unit = get_texture_unit((texture_2d&)tex);

            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top;
            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top + 1;
            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top + 2;
            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top + 1;
            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top + 2;
            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top + 3;

            p_quad_buffer.vb_host_buffer[p_quad_buffer.vb_top++] = quad{ rt::math::vector3(transform * rt::math::vector4(-0.5f, -0.5f, 0.0f, 1.0f)), unit, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } };
            p_quad_buffer.vb_host_buffer[p_quad_buffer.vb_top++] = quad{ rt::math::vector3(transform * rt::math::vector4( 0.5f, -0.5f, 0.0f, 1.0f)), unit, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } };
            p_quad_buffer.vb_host_buffer[p_quad_buffer.vb_top++] = quad{ rt::math::vector3(transform * rt::math::vector4(-0.5f,  0.5f, 0.0f, 1.0f)), unit, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } };
            p_quad_buffer.vb_host_buffer[p_quad_buffer.vb_top++] = quad{ rt::math::vector3(transform * rt::math::vector4( 0.5f,  0.5f, 0.0f, 1.0f)), unit, { 1.0f, 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } };
        }

        void renderer_2d::draw_quad(const math::vector3& pos, const math::vector2& size, const math::vector3& rotation, const texture_2d& tex, const math::vector4& color)
        {
#ifdef RT_DEBUG_ENABLED
            statistics::get().quads++;
#endif

            rt::math::matrix scale = rt::math::matrix::scale({ size.x, size.y, 0 });
            rt::math::matrix rotate = rt::math::matrix::rotate(rt::math::attitude_to_quaternion(rotation.x, rotation.y, rt::math::radians(rotation.z)));
            rt::math::matrix translate = rt::math::matrix::translate(pos);

            rt::math::matrix transform = translate * rotate * scale;

            check_quad();

            float unit = get_texture_unit((texture_2d&)tex);

            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top;
            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top + 1;
            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top + 2;
            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top + 1;
            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top + 2;
            p_quad_buffer.ib_host_buffer[p_quad_buffer.ib_top++] = p_quad_buffer.vb_top + 3;

            p_quad_buffer.vb_host_buffer[p_quad_buffer.vb_top++] = quad{ rt::math::vector3(transform * rt::math::vector4(-0.5f, -0.5f, 0.0f, 1.0f)), unit, color, { 0.0f, 0.0f } };
            p_quad_buffer.vb_host_buffer[p_quad_buffer.vb_top++] = quad{ rt::math::vector3(transform * rt::math::vector4( 0.5f, -0.5f, 0.0f, 1.0f)), unit, color, { 1.0f, 0.0f } };
            p_quad_buffer.vb_host_buffer[p_quad_buffer.vb_top++] = quad{ rt::math::vector3(transform * rt::math::vector4(-0.5f,  0.5f, 0.0f, 1.0f)), unit, color, { 0.0f, 1.0f } };
            p_quad_buffer.vb_host_buffer[p_quad_buffer.vb_top++] = quad{ rt::math::vector3(transform * rt::math::vector4( 0.5f,  0.5f, 0.0f, 1.0f)), unit, color, { 1.0f, 1.0f } };
        }

        void renderer_2d::draw_line(const math::vector3& a, const math::vector3& b, const math::vector4& color)
        {
            RT_WARN("this function must be removed: renderer_2d::draw_line(const math::vector3& a, const math::vector3& b, const math::vector4& color)");
#ifdef RT_DEBUG_ENABLED
            statistics::get().lines++;
#endif

            if (p_line_buffer.vb_top >= p_line_buffer.vb_limit)
            {
                flush_line();
            }

            p_line_buffer.vb_host_buffer[p_line_buffer.vb_top++] = line{ a, color };
            p_line_buffer.vb_host_buffer[p_line_buffer.vb_top++] = line{ b, color };
        }

        void renderer_2d::draw_line(const math::vector3& pos, const float len, const math::vector3& rotation, const math::vector4& color)
        {
#ifdef RT_DEBUG_ENABLED
            statistics::get().lines++;
#endif

            rt::math::matrix scale = rt::math::matrix::scale({ len, 0, 0 });
            rt::math::matrix rotate = rt::math::matrix::rotate(rt::math::attitude_to_quaternion(rotation.x, rotation.y, rt::math::radians(rotation.z)));
            rt::math::matrix translate = rt::math::matrix::translate(pos);

            rt::math::matrix transform = translate * rotate * scale;

            if (p_line_buffer.vb_top >= p_line_buffer.vb_limit)
            {
                flush_line();
            }

            p_line_buffer.vb_host_buffer[p_line_buffer.vb_top++] = line{ rt::math::vector3(transform * rt::math::vector4(-0.5f, 0.0f, 0.0f, 1.0f)), color };
            p_line_buffer.vb_host_buffer[p_line_buffer.vb_top++] = line{ rt::math::vector3(transform * rt::math::vector4(0.5f, 0.0f, 0.0f, 1.0f)), color };
        }

        void renderer_2d::draw_circle(const math::vector3& pos, const math::vector2& size, const math::vector3& rotation, float thickness, float fade, const math::vector4& color)
        {
#ifdef RT_DEBUG_ENABLED
            statistics::get().circles++;
#endif

            rt::math::matrix scale = rt::math::matrix::scale({ size.x, size.y, 0 });
            rt::math::matrix rotate = rt::math::matrix::rotate(rt::math::attitude_to_quaternion(rotation.x, rotation.y, rt::math::radians(rotation.z)));
            rt::math::matrix translate = rt::math::matrix::translate(pos);

            rt::math::matrix transform = translate * rotate * scale;

            if (p_circle_buffer.vb_top >= p_circle_buffer.vb_limit)
            {
                flush_circle();
            }

            p_circle_buffer.ib_host_buffer[p_circle_buffer.ib_top++] = p_circle_buffer.vb_top;
            p_circle_buffer.ib_host_buffer[p_circle_buffer.ib_top++] = p_circle_buffer.vb_top + 1;
            p_circle_buffer.ib_host_buffer[p_circle_buffer.ib_top++] = p_circle_buffer.vb_top + 2;
            p_circle_buffer.ib_host_buffer[p_circle_buffer.ib_top++] = p_circle_buffer.vb_top + 1;
            p_circle_buffer.ib_host_buffer[p_circle_buffer.ib_top++] = p_circle_buffer.vb_top + 2;
            p_circle_buffer.ib_host_buffer[p_circle_buffer.ib_top++] = p_circle_buffer.vb_top + 3;

            p_circle_buffer.vb_host_buffer[p_circle_buffer.vb_top++] = circle{ rt::math::vector3(transform * rt::math::vector4(-0.5f, -0.5f, 0.0f, 1.0f)), thickness, color, { -1.0f, -1.0f }, fade };
            p_circle_buffer.vb_host_buffer[p_circle_buffer.vb_top++] = circle{ rt::math::vector3(transform * rt::math::vector4( 0.5f, -0.5f, 0.0f, 1.0f)), thickness, color, {  1.0f, -1.0f }, fade };
            p_circle_buffer.vb_host_buffer[p_circle_buffer.vb_top++] = circle{ rt::math::vector3(transform * rt::math::vector4(-0.5f,  0.5f, 0.0f, 1.0f)), thickness, color, { -1.0f,  1.0f }, fade };
            p_circle_buffer.vb_host_buffer[p_circle_buffer.vb_top++] = circle{ rt::math::vector3(transform * rt::math::vector4( 0.5f,  0.5f, 0.0f, 1.0f)), thickness, color, {  1.0f,  1.0f }, fade };
        }

        void renderer_2d::direct_quad_draw()
        {
#ifdef RT_DEBUG_ENABLED
            statistics::get().draws++;
#endif

            static unsigned int vao = 0, vbo = 0;

            if (vao == 0)
            {
                float vertices[] = {
                    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                     1.0f, -1.0f, 0.0f, 1.0f, 0.0f
                };

                glGenVertexArrays(1, &vao);
                glGenBuffers(1, &vbo);

                glBindVertexArray(vao);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);

                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            }

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }

        void renderer_2d::flush()
        {
            flush_quad();
            flush_circle();
            flush_line();
        }

        void renderer_2d::shutdown()
        {
            if (p_quad_buffer.vb_host_buffer)
            {
                free(p_quad_buffer.vb_host_buffer);
                p_quad_buffer.vb_limit = 0;
                p_quad_buffer.vb_top = 0;
            }

            if (p_quad_buffer.ib_host_buffer)
            {
                free(p_quad_buffer.ib_host_buffer);
                p_quad_buffer.ib_limit = 0;
                p_quad_buffer.ib_top = 0;
            }


            if (p_circle_buffer.vb_host_buffer)
            {
                free(p_circle_buffer.vb_host_buffer);
                p_circle_buffer.vb_limit = 0;
                p_circle_buffer.vb_top = 0;
            }

            if (p_circle_buffer.ib_host_buffer)
            {
                free(p_circle_buffer.ib_host_buffer);
                p_circle_buffer.ib_limit = 0;
                p_circle_buffer.ib_top = 0;
            }


            if (p_line_buffer.vb_host_buffer)
            {
                free(p_line_buffer.vb_host_buffer);
                p_line_buffer.vb_limit = 0;
                p_line_buffer.vb_top = 0;
            }

            if (p_line_buffer.ib_host_buffer)
            {
                free(p_line_buffer.ib_host_buffer);
                p_line_buffer.ib_limit = 0;
                p_line_buffer.ib_top = 0;
            }
        }

        void renderer_2d::flush_line()
        {
#ifdef RT_DEBUG_ENABLED
            statistics::get().draws++;
#endif

            p_line_shader.bind();
            p_line_shader.u_mat_4fv("u_view_projection", 1, false, &p_cam.view_projection.c0_r0);

            p_line_buffer.vb.set_data(p_line_buffer.vb_host_buffer, p_line_buffer.vb_top * sizeof(line));

            p_line_buffer.va.bind();
            glDrawArrays(GL_LINES, 0, p_line_buffer.vb_top);

            p_line_buffer.vb_top = 0;
        }

        void renderer_2d::flush_circle()
        {
#ifdef RT_DEBUG_ENABLED
            statistics::get().draws++;
#endif

            p_circle_shader.bind();
            p_circle_shader.u_mat_4fv("u_view_projection", 1, false, &p_cam.view_projection.c0_r0);

            p_circle_buffer.vb.set_data(p_circle_buffer.vb_host_buffer, p_circle_buffer.vb_top * sizeof(circle));
            p_circle_buffer.ib.set_data(p_circle_buffer.ib_host_buffer, p_circle_buffer.ib_top);

            p_circle_buffer.va.bind();

            glDrawElements(GL_TRIANGLES, p_circle_buffer.ib_top, GL_UNSIGNED_INT, 0);

            p_circle_buffer.vb_top = 0;
            p_circle_buffer.ib_top = 0;
        }

        void renderer_2d::flush_quad()
        {
#ifdef RT_DEBUG_ENABLED
            statistics::get().draws++;
#endif

            p_quad_shader.bind();
            p_quad_shader.u_mat_4fv("u_view_projection", 1, false, &p_cam.view_projection.c0_r0);

            p_quad_buffer.vb.set_data(p_quad_buffer.vb_host_buffer, p_quad_buffer.vb_top * sizeof(quad));
            p_quad_buffer.ib.set_data(p_quad_buffer.ib_host_buffer, p_quad_buffer.ib_top);

            p_quad_buffer.va.bind();

            glDrawElements(GL_TRIANGLES, p_quad_buffer.ib_top, GL_UNSIGNED_INT, 0);

            p_quad_buffer.vb_top = 0;
            p_quad_buffer.ib_top = 0;
            p_last_texture_slot = 1;
        }

        void renderer_2d::check_quad()
        {
            if (p_quad_buffer.vb_top >= p_quad_buffer.vb_limit || p_last_texture_slot > 31)
            {
                flush_quad();
            }
        }

    }
} 