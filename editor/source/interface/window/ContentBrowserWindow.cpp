#include "ContentBrowserWindow.h"

#include "components/Components.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace editor
{
    namespace gui
    {

        content_browser::content_browser(rt::root& root, gui_state& state)
            : gui_window(root, state, "Content browser"), p_padding(16.0f), p_thumbnail_size(100.0f)
        {
            rt::gfx::texture_parameter parameters[] = {
                rt::gfx::texture_parameter::filter_min,
                rt::gfx::texture_parameter::filter_mag,
                rt::gfx::texture_parameter::wrap_s,
                rt::gfx::texture_parameter::wrap_t
            };

            rt::gfx::texture_parameter_value values[] = {
                rt::gfx::texture_parameter_value::linear,
                rt::gfx::texture_parameter_value::linear,
                rt::gfx::texture_parameter_value::clamp_to_edge,
                rt::gfx::texture_parameter_value::clamp_to_edge
            };

            p_file_icon = std::make_unique<rt::gfx::texture_2d>(parameters, values, 4, "resources\\icons\\file.png");
            p_directory_icon = std::make_unique<rt::gfx::texture_2d>(parameters, values, 4, "resources\\icons\\folder.png");
            p_back_icon = std::make_unique<rt::gfx::texture_2d>(parameters, values, 4, "resources\\icons\\arrow.png");

            p_path.push_back(EDITOR_CONTENT_BROWSER_BASE_PATH);
            p_head = 0;
        }

        void content_browser::update()
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(250, 250));

            if (ImGui::Begin(p_title.c_str()))
            { 
                if (ImGui::IsWindowFocused())
                {
                    p_state.selected = gui_window_type::content_browser;
                }

                bool en = !p_head;
                if (en) ImGui::BeginDisabled();

                ImGui::ImageButton((ImTextureID)p_back_icon->get_id(), { 20, 20 }, { 1, 1 }, { 0, 0 });
                if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    if (p_head > 0) p_head--;
                }

                if (en) ImGui::EndDisabled();


                ImGui::SameLine();


                en = !(p_path.size() - 1 > p_head);
                if (en) ImGui::BeginDisabled();

                ImGui::ImageButton((ImTextureID)p_back_icon->get_id(), { 20, 20 });
                if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
                {
                    p_head++;
                }

                if (en) ImGui::EndDisabled();


                ImGui::SameLine();


                ImGui::Text(p_path[p_head].generic_string().c_str());

                int col = (int)(ImGui::GetContentRegionAvailWidth() / (p_padding + p_thumbnail_size));
                if (col < 1) col = 1;

                ImGui::PushStyleColor(ImGuiCol_Button, { 0.0f, 0.0f, 0.0f, 0.0f });
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { 0.0f, 0.0f, 0.0f, 0.0f });
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, { 0.0f, 0.0f, 0.0f, 0.0f });
                if (ImGui::BeginTable("##navigation", col))
                {
                    ImGui::TableNextRow();

                    auto scene = p_root.get_system<rt::ecs::scene>();

                    auto itpath = p_path[p_head];

                    for (auto& element : std::filesystem::directory_iterator(itpath))
                    {
                        ImGui::TableNextColumn();

                        auto& path = element.path();
                        std::string element_name = std::filesystem::relative(path, itpath).filename().string();

                        bool action = false;

                        if (element.is_directory())
                        {
                            button_tint(element_name.c_str(), (ImTextureID)p_directory_icon->get_id(), ImVec2{p_thumbnail_size, p_thumbnail_size}, ImVec2{0, 1}, ImVec2{1, 0}, 0, ImVec4{0, 0, 0, 0}, ImVec4{0.7, 0.7, 0.7, 1.0}, ImVec4{0.3, 0.3, 0.3, 1.0});
                            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
                            {
                                p_path.resize(p_head + 1);
                                p_path.push_back(itpath /= path.filename());
                                itpath = p_path[p_head];
                                p_head++;
                            }
                        }
                        else
                        {
                            button_tint(element_name.c_str(), (ImTextureID)p_file_icon->get_id(), ImVec2{ p_thumbnail_size, p_thumbnail_size }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 }, 0, ImVec4{ 0, 0, 0, 0 }, ImVec4{ 0.7, 0.7, 0.7, 1.0 }, ImVec4{ 0.3, 0.3, 0.3, 1.0 });
                            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right) && p_state.selected_entity)
                            {
                                ImGui::OpenPopup(element_name.c_str());
                            }
                        }

                        ImGui::Text(element_name.c_str());
                        ImGui::NextColumn();

                        if (ImGui::BeginPopup(element_name.c_str()))
                        {
                            if (ImGui::Button("Attach as texture"))
                            {
                                scene->attach_component<texture_2d>(p_state.selected_entity);
                                ImGui::OpenPopup("Texture settings");
                            }

                            if (ImGui::BeginPopupModal("Texture settings", 0, ImGuiWindowFlags_NoResize))
                            {
                                if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_Framed))
                                {
                                    p_texture_settings.update();
                                }

                                ImGuiTreeNodeFlags flags =
                                    ImGuiTreeNodeFlags_FramePadding |
                                    ImGuiTreeNodeFlags_Framed |
                                    ImGuiTreeNodeFlags_SpanAvailWidth |
                                    ImGuiTreeNodeFlags_Leaf |
                                    ImGuiTreeNodeFlags_Selected;

                                if (ImGui::TreeNodeEx((void*)0, flags, "Ok"))
                                {
                                    if (ImGui::IsItemClicked())
                                    {
                                        rt::gfx::texture_parameter parameters[] = {
                                            rt::gfx::texture_parameter::filter_min,
                                            rt::gfx::texture_parameter::filter_mag,
                                            rt::gfx::texture_parameter::wrap_s,
                                            rt::gfx::texture_parameter::wrap_t
                                        };

                                        auto tx = scene->get_component<texture_2d>(p_state.selected_entity);
                                        if (tx) tx->~texture_2d();
                                        new (tx) texture_2d(parameters, p_texture_settings.get_values(), 4, path.string());

                                        ImGui::CloseCurrentPopup();
                                    }
                                    ImGui::TreePop();
                                }

                                if (ImGui::TreeNodeEx((void*)0, flags, "Cancel"))
                                {
                                    if (ImGui::IsItemClicked())
                                    {
                                        ImGui::CloseCurrentPopup();
                                    }
                                    ImGui::TreePop();
                                }

                                ImGui::EndPopup();
                            }

                            ImGui::EndPopup();
                        }
                    }

                    ImGui::EndTable();
                }
                ImGui::PopStyleColor(3);
            }

            ImGui::End();
            ImGui::PopStyleVar();
        }

        static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
        static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
        static inline ImVec2 operator*(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x * rhs, lhs.y * rhs); }

        bool content_browser::button_tint(const char* label, ImTextureID user_texture_id, ImVec2& size, ImVec2& uv0, ImVec2& uv1, int frame_padding, ImVec4& bg_col, ImVec4& hover_tint, ImVec4& held_tint)
        {
            ImGuiContext& g = *GImGui;
            ImGuiWindow* window = g.CurrentWindow;
            if (window->SkipItems)
                return false;

            ImGui::PushID((void*)(intptr_t)user_texture_id);
            ImGuiID id = window->GetID(label);
            ImGui::PopID();

            ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : g.Style.FramePadding;


            ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
            ImGui::ItemSize(bb);
            if (!ImGui::ItemAdd(bb, id))
                return false;

            bool hovered, held;
            bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

            ImVec4 tint = { 1.0f, 1.0f, 1.0f, 1.0f };
            if (hovered) tint = hover_tint;
            if (held) tint = held_tint;

            // Render
            ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
            ImGui::RenderNavHighlight(bb, id);
            ImGui::RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g.Style.FrameRounding));
            if (bg_col.w > 0.0f)
                window->DrawList->AddRectFilled(bb.Min + padding, bb.Max - padding, ImGui::GetColorU32(bg_col));
            window->DrawList->AddImage(user_texture_id, bb.Min + padding, bb.Max - padding, uv0, uv1, ImGui::GetColorU32(tint));

            return pressed;
        }

    }
}