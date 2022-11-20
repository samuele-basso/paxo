#pragma once
#include "Root.h"
#include "Timing.h"
#include "core/Input.h"
#include "core/Window.h"
#include "renderer/Context.h"
#include "renderer/Renderer2D.h"
#include "scene/ECS.h"

namespace rt
{

    class application
    {
    public:
        application(state& application_state)
        {
            p_root.create_system<timing>();

            p_root.create_system<window>();
            std::shared_ptr<rt::window> win = p_root.get_system<rt::window>();
            win->init(application_state.window);

            p_root.create_system<input>();
            std::shared_ptr<rt::input> inp = p_root.get_system<rt::input>();
            inp->init();

            p_root.create_system<gfx::context>();
            std::shared_ptr<rt::gfx::context> context = p_root.get_system<rt::gfx::context>();
            context->init();

            p_root.create_system<gfx::renderer_2d>();
            std::shared_ptr<rt::gfx::renderer_2d> renderer_2d = p_root.get_system<rt::gfx::renderer_2d>();
            renderer_2d->init();

            p_root.create_system<rt::ecs::scene>();
        }

        virtual ~application() = default;

        virtual void run() { };

    protected:
        root p_root;
    };

    extern application* get();

}

int main()
{
    auto app = rt::get();
    app->run();
    return 0;
}