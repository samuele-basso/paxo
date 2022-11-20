#include "QuadControl.h"

#include "windows/Utils.h"

#include <imgui.h>

namespace editor
{
    namespace gui
    {

        quad_control::quad_control()
            : texture_update(false), path(false)
        {
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

#define RT_BLANK_TEXTURE "resources/textures/blank.png"
            p_default = std::make_unique<rt::gfx::texture_2d>(parameters, values, 4, RT_BLANK_TEXTURE);
        }

        bool quad_control::update(quad* component, texture_2d* texture, std::shared_ptr<rt::window>& win)
        {
            bool or = false;

            ImGui::PushID("quadcontrol");

            if (ImGui::CollapsingHeader("Quad"))
            {
                or |= p_vec2mod.update("Position", &component->position.x);
                or |= p_vec1mod.update("Depth", &component->position.z);
                or |= p_vec2mod.update("Size", &component->size.x);
                or |= p_vec1mod.update("Rotation", &component->rotation.z);

                ImGui::Text("Color");
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                or |= ImGui::ColorEdit4("##Color", &component->color.x);
                ImGui::PopStyleVar();

                ImTextureID id = (ImTextureID)p_default->get_id();
                if (texture) id = (ImTextureID)texture->texture.get_id();

                if (ImGui::ImageButton(id, { 100.0f, 100.0f }, { 1, 1 }, { 0, 0 }, 2.0f))
                {
                    or |= true;
                    ImGui::OpenPopup("Texture settings");
                }

                if (ImGui::BeginPopupModal("Texture settings", 0, ImGuiWindowFlags_NoResize))
                {
                    ImGuiTreeNodeFlags flags =
                        ImGuiTreeNodeFlags_FramePadding |
                        ImGuiTreeNodeFlags_Framed |
                        ImGuiTreeNodeFlags_SpanAvailWidth |
                        ImGuiTreeNodeFlags_Leaf |
                        ImGuiTreeNodeFlags_Selected;

                    if (ImGui::TreeNodeEx((void*)0, flags, "Select texture"))
                    {
                        if (ImGui::IsItemClicked())
                        {
                            rt::open_file_dialog(texture_path, path, win);
                        }
                        ImGui::TreePop();
                    }

                    if (!path) ImGui::BeginDisabled();
                    if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_Framed))
                    {
                        p_texture_settings.update();
                    }
                    if (!path) ImGui::EndDisabled();

                    if (ImGui::TreeNodeEx((void*)0, flags, "Ok"))
                    {
                        if (ImGui::IsItemClicked())
                        {
                            texture_update = path & true;
                            path = false;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNodeEx((void*)0, flags, "Cancel"))
                    {
                        if (ImGui::IsItemClicked())
                        {
                            path = false;
                            texture_update = false;
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::TreePop();
                    }

                    ImGui::EndPopup();
                }

                if (!texture) ImGui::BeginDisabled();
                ImGui::Text("Active: ");
                ImGui::SameLine();
                or |= ImGui::Checkbox("##active", &component->active_texture);
                ImGui::SameLine();
                ImGui::Text("Tint: ");
                ImGui::SameLine();
                or |= ImGui::Checkbox("##tint", &component->tint);
                if (!texture) ImGui::EndDisabled();
            }

            ImGui::PopID();

            return or;
        }

    }
}