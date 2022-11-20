#include "HierarchyWindow.h"

#include "components/Components.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace editor
{
    namespace gui
    {

        hierarchy::hierarchy(rt::root& root, gui_state& state)
            : gui_window(root, state, "Hierarchy")
        {
            p_scene = root.get_system<rt::ecs::scene>();
        }

        void hierarchy::update()
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(250, 250));

            if (ImGui::Begin(p_title.c_str()))
            {
                if (ImGui::IsWindowFocused())
                {
                    p_state.selected = gui_window_type::hierarchy;
                }

                p_scene->each([&](auto entity)
                {
                    entity_node(entity);
                });

                if (ImGui::BeginPopupContextWindow(0, 1, false))
                {
                    if (ImGui::Button("Create entity"))
                    {
                        rt::ecs::entity e = p_scene->generate_entity();
                        p_scene->attach_component<name>(e);
                        auto n = p_scene->get_component<name>(e);
                        n->name = std::string("Entity");
                    }

                    ImGui::EndPopup();
                }
            }
            ImGui::End();

            ImGui::PopStyleVar();
        }

        void hierarchy::entity_node(rt::ecs::entity entity)
        {
            auto n = p_scene->get_component<name>(entity);

            ImGuiTreeNodeFlags flags =
                ImGuiTreeNodeFlags_OpenOnArrow |
                ImGuiTreeNodeFlags_FramePadding |
                ImGuiTreeNodeFlags_Framed |
                ImGuiTreeNodeFlags_SpanAvailWidth |
                ImGuiTreeNodeFlags_Leaf;

            bool opened = false;

            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.5f);
            if (p_state.selected_entity == entity)
            {
                opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags | ImGuiTreeNodeFlags_Selected, n->name.c_str());
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Header, { 0.0f, 0.0f, 0.0f, 0.0f });
                opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, n->name.c_str());
                ImGui::PopStyleColor();
            }
            ImGui::PopStyleVar();


            if (ImGui::IsItemClicked())
            {
                p_state.selected_entity = entity;
            }

            bool deleted = false;
            if (ImGui::BeginPopupContextItem())
            {
                deleted = ImGui::Button("Delete entity");

                ImGui::EndPopup();
            }

            if (opened)
            {
                ImGui::TreePop();
            }

            if (deleted)
            {
                p_scene->destroy_entity(entity);
                if (p_state.selected_entity == entity)
                    p_state.selected_entity = rt::ecs::entity_null();
            }
        }

    }
}