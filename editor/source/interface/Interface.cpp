#include "Interface.h"

#include "core/Root.h"
#include "core/Window.h"
#include "windows/Utils.h"

#include "Loader.h"
#include "components/Components.h"

namespace editor
{
    namespace gui
    {

        gui::gui(rt::root& root)
            : isystem(root), properties(root, state), statistics(root, state), content_browser(root, state), pipeline_control(root, state),
            renderer(root, state), viewport(root, state), hierarchy(root, state)
        {
            state.selected_entity = rt::ecs::entity_null();

            auto win = p_root.get_system<rt::window>();
            init_api(win->get_GLFWwindow());
            install_callback();

            p_scene = root.get_system<rt::ecs::scene>();
        }

        void gui::begin()
        {
            properties.begin();
            statistics.begin();
            content_browser.begin();
            pipeline_control.begin();
            renderer.begin();
            viewport.begin();
            hierarchy.begin();
        }

        void gui::update()
        {
            new_frame();
            dock();

            properties.update();
            statistics.update();
            content_browser.update();
            pipeline_control.update();
            renderer.update();
            viewport.update();
            hierarchy.update();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

            if (ImGui::BeginMainMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Save scene"))
                    {
                        p_scene->begin_serialize_session();
                        p_scene->serialize_component<name>();
                        p_scene->serialize_component<quad>();
                        p_scene->serialize_component<circle>();
                        p_scene->serialize_component<line>();
                        p_scene->serialize_component<camera>();
                        p_scene->serialize_component<texture_2d>();
                        p_scene->serialize_system_state<pipeline>();
                        p_scene->commit_serialize(true, "scene.json");
                    }
                    if (ImGui::MenuItem("Open"))
                    {
                        auto win = p_root.get_system<rt::window>();
                        auto rend = p_root.get_system<rt::gfx::renderer_2d>();
                        auto ppline = p_root.get_system<pipeline>();

                        bool load = false;
                        std::string path;

                        rt::open_file_dialog(path, load, win);

                        if (load)
                        {
                            for (rt::ecs::entity i = 1; i < p_scene->next(); i++)
                            {
                                p_scene->destroy_entity(i);
                            }

                            p_scene->begin_deserialize_session(path);
                            p_scene->deserialize_component<name>();
                            p_scene->deserialize_component<quad>();
                            p_scene->deserialize_component<circle>();
                            p_scene->deserialize_component<line>();
                            p_scene->deserialize_component<camera>();
                            p_scene->deserialize_component<texture_2d>();

                            auto it = p_scene->entity_component_iterator<camera>();
                            auto* o = it.next();
                            while (o)
                            {
                                o->instance.component.cam = &rend->cam();
                                o->instance.component.aspect = (ppline->pipeline_state.viewport_size.x / ppline->pipeline_state.viewport_size.y);

                                o = it.next();
                            }

                            p_scene->deserialize_system_state<pipeline>();
                        }
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
            ImGui::PopStyleVar();

            end_frame();
        }

        void gui::end()
        {
            properties.end();
            statistics.end();
            content_browser.end();
            pipeline_control.end();
            renderer.end();
            viewport.end();
            hierarchy.end();
        }

    }
}