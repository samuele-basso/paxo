#include "Pipe.h"

#include "components/Components.h"

namespace editor
{

    draw_pipe::draw_pipe(rt::root& root, pipeline_state& state)
        : pipe(root, state), p_blend(true), p_multisample(true), p_clear_on_process(true)
    {
        p_rend = root.get_system<rt::gfx::renderer_2d>();
    }

    void draw_pipe::process()
    {
        if (!p_enabled) return;

        p_state.target.bind();

        rt::gfx::context::blend(p_blend);
        rt::gfx::context::multisample(p_multisample);

        p_state.target.bind();

        if (p_clear_on_process) { rt::gfx::context::clear(); }

        auto quadit = p_scene->entity_component_iterator<quad>();
        auto* q = quadit.next();
        while (q)
        {
            auto t = p_scene->get_component<texture_2d>(q->instance.e);
            auto& quad = q->instance.component;
            if (t && quad.active_texture)
            {
                if (quad.tint)
                {
                    p_rend->draw_quad(quad.position, quad.size, quad.rotation, t->texture, quad.color);
                }
                else
                {
                    p_rend->draw_quad(quad.position, quad.size, quad.rotation, t->texture);
                }
            }
            else
            {
                p_rend->draw_quad(quad.position, quad.size, quad.rotation, quad.color);
            }

            q = quadit.next();
        }

        auto circleit = p_scene->entity_component_iterator<circle>();
        auto* c = circleit.next();
        while (c)
        {
            auto& circle = c->instance.component;
            p_rend->draw_circle(circle.position, circle.size, circle.rotation, circle.thickness, circle.fade, circle.color);
            c = circleit.next();
        }

        auto lineit = p_scene->entity_component_iterator<line>();
        auto* l = lineit.next();
        while (l)
        {
            auto& line = l->instance.component;
            p_rend->draw_line(line.position, line.size, line.rotation, line.color);
            l = lineit.next();
        }

        p_rend->flush();

        p_state.target.unbind();
    }

    void draw_pipe::update_samples()
    {
        if (p_multisample)
        {
            if (!p_state.target.is_multisample())
            {
                p_state.target.enable_multisampling(p_state.target.get_samples());
            }
        }
        else
        {
            if (p_state.target.is_multisample())
            {
                p_state.target.disable_multisampling();
            }
        }
    }

#ifndef PX_STANDALONE
    msaa_pipe::msaa_pipe(rt::root& root, pipeline_state& state)
        : pipe(root, state)
    {
        p_rend = root.get_system<rt::gfx::renderer_2d>();
    }

    void msaa_pipe::process()
    {
        if (!p_enabled) return;

        p_state.target_msaa.bind();

        rt::gfx::context::multisample(false);

        p_state.target.get_texture(0)->bind(0);

        rt::gfx::context::clear();

        p_msaa.bind();

        rt::math::vector2 size(p_state.target.get_width(), p_state.target.get_height());
        p_msaa.u_2fv("u_screen_size", 1, &size.x);
        p_msaa.u_1i("u_buffer", 0);
        p_msaa.u_1i("u_samples", p_state.target.get_samples());

        p_rend->direct_quad_draw();

        p_state.target_msaa.unbind();
    }

#else

    msaa_pipe_default::msaa_pipe_default(rt::root& root, pipeline_state& state)
        : pipe(root, state)
    {
        p_rend = root.get_system<rt::gfx::renderer_2d>();
    }

    void msaa_pipe_default::process()
    {
        if (!p_enabled) return;

        rt::gfx::context::multisample(false);

        p_state.target.get_texture(0)->bind(0);

        rt::gfx::context::clear();

        p_msaa.bind();

        rt::math::vector2 size(p_state.target.get_width(), p_state.target.get_height());
        p_msaa.u_2fv("u_screen_size", 1, &size.x);
        p_msaa.u_1i("u_buffer", 0);
        p_msaa.u_1i("u_samples", p_state.target.get_samples());

        p_rend->direct_quad_draw();
    }
#endif

}