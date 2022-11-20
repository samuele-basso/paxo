#include "Editor.h"

#include "event/Event.h"

#ifndef PX_STANDALONE
#include <imgui.h>
#include "interface/Loader.h"
#include "interface/Interface.h"
#endif

#include <GLFW/glfw3.h>

#include <sstream>

#include "components/Components.h"
#include "extension/Pipe.h"
#include "extension/Pipeline.h"
#include "extension/CameraController.h"
#include "windows/Utils.h"

namespace editor
{

    const rt::window_state s_window_state { "Paxo - editor", 1920, 1080, true, false };
    rt::state s_state { s_window_state };

    editor::editor() : rt::application(s_state)
    {
        p_root.create_system<pipeline>();
        p_root.create_system<camera_controller>();
#ifndef PX_STANDALONE
        p_root.create_system<gui::gui>();
#endif
    }

    void editor::run()
    {
        auto t = p_root.get_system<rt::timing>();
        auto win = p_root.get_system<rt::window>();
        auto ppline = p_root.get_system<pipeline>();
        auto cam = p_root.get_system<camera_controller>();
#ifndef PX_STANDALONE
        auto gui = p_root.get_system<gui::gui>();
#endif

#ifdef PX_STANDALONE
        auto rend = p_root.get_system<rt::gfx::renderer_2d>();
        auto scene = p_root.get_system<rt::ecs::scene>();

        bool load = false;
        std::string path;

        rt::open_file_dialog(path, load, win);

        if (load)
        {
            for (rt::ecs::entity i = 1; i < scene->next(); i++)
            {
                scene->destroy_entity(i);
            }

            scene->begin_deserialize_session(path);
            scene->deserialize_component<name>();
            scene->deserialize_component<quad>();
            scene->deserialize_component<circle>();
            scene->deserialize_component<line>();
            scene->deserialize_component<camera>();
            scene->deserialize_component<texture_2d>();

            auto it = scene->entity_component_iterator<camera>();
            auto* o = it.next();
            while (o)
            {
                o->instance.component.cam = &rend->cam();
                o = it.next();
            }

            scene->deserialize_system_state<pipeline>();
        }

        rt::evnt::viewport_resize_event er(win->get_GLFWwindow(), win->get_width(), win->get_height());
        rt::evnt::evnt::fire(rt::evnt::rt_event_type::viewport_resize_event, &er);
#endif

        t->begin();

        rt::evnt::app_tick_event e(win->get_GLFWwindow());

        rt::gfx::context::depth(true);
        rt::gfx::context::set_clear_color(rt::math::vector4{ 0.27f, 0.275f, 0.28f, 1.0f });
        rt::gfx::context::line_width(1.0f);

        while (!win->should_close())
        {
            rt::evnt::evnt::fire(rt::evnt::rt_event_type::app_tick_event, &e);

#ifndef PX_STANDALONE
            gui->begin();
#endif

            rt::gfx::context::clear();

            ppline->process();

#ifndef PX_STANDALONE
            gui->update();
#endif

            win->update();

#ifndef PX_STANDALONE
            gui->end();
#endif

#ifdef PX_STANDALONE
            double fps = 1.0 / t->get();
            std::stringstream ss;
            ss << "Paxo - Standalone [" << fps << " fps]";
            glfwSetWindowTitle(win->get_GLFWwindow(), ss.str().c_str());
#endif

            t->end();
        }
    }

    editor::~editor()
    {
#ifndef PX_STANDALONE
        gui::shutdown_api();
#endif

        std::shared_ptr<rt::window> win = p_root.get_system<rt::window>();
        win->shutdown();
    }

}

rt::application* rt::get()
{
    return new editor::editor();
}