#pragma once
#include "renderer/Context.h"
#include "renderer/Renderer2D.h"
#include "scene/ECS.h"
#include "core/Root.h"
#include "PipelineState.h"

namespace editor
{

    class pipe
    {
    public:
        pipe(rt::root& root, pipeline_state& state) : p_root(root), p_state(state), p_enabled(true) { }
        virtual void process() = 0;

        bool* enabled() { return &p_enabled; }

    protected:
        rt::root& p_root;
        pipeline_state& p_state;
        bool p_enabled;
    };

    class draw_pipe : public pipe
    {
    public:
        draw_pipe(rt::root& root, pipeline_state& state);

        virtual void process() override;

        void update_samples();

        bool* blend() { return &p_blend; }
        bool* multisample() { return &p_multisample; }
        bool* clearonprocess() { return &p_clear_on_process; }

    private:
        std::shared_ptr<rt::gfx::renderer_2d> p_rend;
        std::shared_ptr<rt::ecs::scene> p_scene;

        bool p_blend;
        bool p_multisample;

        bool p_clear_on_process;
    };

#ifndef PX_STANDALONE
    class msaa_pipe : public pipe
    {
    public:
        msaa_pipe(rt::root& root, pipeline_state& state);

        virtual void process() override;

    private:
        rt::gfx::shader p_msaa{ SHADER_MSAA_VS, SHADER_MSAA_FS };
        std::shared_ptr<rt::gfx::renderer_2d> p_rend;
    };
#else
    class msaa_pipe_default : public pipe
    {
    public:
        msaa_pipe_default(rt::root& root, pipeline_state& state);

        virtual void process() override;

    private:
        rt::gfx::shader p_msaa{ SHADER_MSAA_VS, SHADER_MSAA_FS };
        std::shared_ptr<rt::gfx::renderer_2d> p_rend;
    };
#endif

}