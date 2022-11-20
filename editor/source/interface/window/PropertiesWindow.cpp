#include "PropertiesWindow.h"

#include <imgui.h>

namespace editor
{
    namespace gui
    {

        properties::properties(rt::root& root, gui_state& state)
            : gui_window(root, state, "Properties"), p_has_quad(false), p_has_circle(false), p_has_line(false), p_has_camera(false)
        {
            p_scene = root.get_system<rt::ecs::scene>();
            p_cam = root.get_system<camera_controller>();
            p_win = root.get_system<rt::window>();
            p_pipeline = root.get_system<pipeline>();
        }

        void properties::update()
        {
            if (ImGui::Begin(p_title.c_str()))
            {
                if (ImGui::IsWindowFocused())
                {
                    p_state.selected = gui_window_type::properties;
                }

                if (p_state.selected_entity == rt::ecs::entity_null())
                {
                    ImGui::Text("No entity selected");
                    ImGui::End();
                    return;
                }

                p_name_modifier.update(p_scene->get_component<name>(p_state.selected_entity), 250.0f);

                ImGui::SameLine();

                p_has_quad = p_scene->has_component<quad>(p_state.selected_entity);
                p_has_circle = p_scene->has_component<circle>(p_state.selected_entity);
                p_has_line = p_scene->has_component<line>(p_state.selected_entity);
                p_has_camera = p_scene->has_component<camera>(p_state.selected_entity);

                component();


                if (p_has_quad)
                {
                    p_quad_control.update(
                        p_scene->get_component<quad>(p_state.selected_entity),
                        p_scene->get_component<texture_2d>(p_state.selected_entity),
                        p_win
                    );
                }

                if (p_has_circle)
                {
                    p_circle_control.update(
                        p_scene->get_component<circle>(p_state.selected_entity)
                    );
                }

                if (p_has_line)
                {
                    p_line_control.update(
                        p_scene->get_component<line>(p_state.selected_entity)
                    );
                }

                if (p_has_camera)
                {
                    p_camera_control.update(
                        p_scene->get_component<camera>(p_state.selected_entity),
                        p_pipeline->pipeline_state,
                        p_state
                    );
                }
            }
            ImGui::End();
        }

        void properties::end()
        {
            if (p_quad_control.texture_update)
            {
                rt::gfx::texture_parameter parameters[] = {
                    rt::gfx::texture_parameter::filter_min,
                    rt::gfx::texture_parameter::filter_mag,
                    rt::gfx::texture_parameter::wrap_s,
                    rt::gfx::texture_parameter::wrap_t
                };

                auto t = p_scene->get_component<texture_2d>(p_state.selected_entity);

                if (!t)
                {
                    p_scene->attach_component<texture_2d>(p_state.selected_entity);
                    t = p_scene->get_component<texture_2d>(p_state.selected_entity);
                }

                t->~texture_2d();
                new (t) texture_2d(parameters, p_quad_control.get_texture_value(), 4, p_quad_control.texture_path.c_str());

                p_quad_control.texture_update = p_quad_control.path = false;
            }
        }

        void properties::component()
        {
            if (p_has_quad && p_has_circle && p_has_line && p_has_camera)
            {
                ImGui::BeginDisabled();
                ImGui::Button("Add component");
                ImGui::EndDisabled();
            }
            else
            {
                if (ImGui::Button("Add component"))
                {
                    ImGui::OpenPopup("AddComponent");
                }

                if (ImGui::BeginPopup("AddComponent"))
                {
                    if (!p_has_quad)
                    {
                        if (ImGui::MenuItem("Quad"))
                        {
                            p_scene->attach_component<quad>(p_state.selected_entity);
                            new (p_scene->get_component<quad>(p_state.selected_entity)) quad();
                        }
                    }

                    if (!p_has_circle)
                    {
                        if (ImGui::MenuItem("Circle"))
                        {
                            p_scene->attach_component<circle>(p_state.selected_entity);
                            new (p_scene->get_component<circle>(p_state.selected_entity)) circle();
                        }
                    }

                    if (!p_has_line)
                    {
                        if (ImGui::MenuItem("Line"))
                        {
                            p_scene->attach_component<line>(p_state.selected_entity);
                            new (p_scene->get_component<line>(p_state.selected_entity)) line();
                        }
                    }

                    if (!p_has_camera)
                    {
                        if (ImGui::MenuItem("Camera"))
                        {
                            p_scene->attach_component<camera>(p_state.selected_entity);
                            p_cam->build_camera(p_scene->get_component<camera>(p_state.selected_entity));
                        }
                    }
                    ImGui::EndPopup();
                }
            }

            ImGui::SameLine();

            if (!(p_has_quad || p_has_circle || p_has_line || p_has_camera))
            {
                ImGui::BeginDisabled();
                ImGui::Button("Remove component");
                ImGui::EndDisabled();
            }
            else
            {
                if (ImGui::Button("Remove component"))
                {
                    ImGui::OpenPopup("RemoveComponent");
                }

                if (ImGui::BeginPopup("RemoveComponent"))
                {
                    if (p_has_quad)
                    {
                        if (ImGui::MenuItem("Quad"))
                        {
                            p_scene->remove_component<quad>(p_state.selected_entity);
                        }
                    }

                    if (p_has_circle)
                    {
                        if (ImGui::MenuItem("Circle"))
                        {
                            p_scene->remove_component<circle>(p_state.selected_entity);
                        }
                    }

                    if (p_has_line)
                    {
                        if (ImGui::MenuItem("Line"))
                        {
                            p_scene->remove_component<line>(p_state.selected_entity);
                        }
                    }

                    if (p_has_camera)
                    {
                        if (ImGui::MenuItem("Camera"))
                        {
                            p_scene->remove_component<camera>(p_state.selected_entity);
                        }
                    }
                    ImGui::EndPopup();
                }
            }

            p_has_quad = p_scene->has_component<quad>(p_state.selected_entity);
            p_has_circle = p_scene->has_component<circle>(p_state.selected_entity);
            p_has_line = p_scene->has_component<line>(p_state.selected_entity);
            p_has_camera = p_scene->has_component<camera>(p_state.selected_entity);
        }

    }
}