#include "VectorModifier.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace editor
{
    namespace gui
    {

        bool vector1_modifier::update(const std::string& label, float* value, float reset_value, float speed, float column_width)
        {
            bool modified = false;

            ImGuiIO& io = ImGui::GetIO();
            auto bold = io.Fonts->Fonts[0];

            ImGui::PushID(label.c_str());

            ImGui::Text(label.c_str());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

            float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding[1] * 2.0f;
            ImVec2 button_size = { line_height + 3.0f, line_height };

            ImGui::PushItemWidth(column_width);

            ImGui::PushFont(bold);
            ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(75, 150, 250, 150));
            if (ImGui::Button("S", button_size))
            {
                *value = reset_value;
                modified = true;
            }
            ImGui::PopStyleColor();
            ImGui::PopFont();
            ImGui::SameLine();
            if (ImGui::DragFloat("##S", value, speed, 0.0f, 0.0f, "%.2f"))
            {
                modified = true;
            }

            ImGui::PopStyleVar(2);
            ImGui::PopItemWidth();

            ImGui::PopID();

            return modified;
        }

        bool vector2_modifier::update(const std::string& label, float* values, float reset_value, float speed, float column_width)
        {
            bool modified = false;

            ImGuiIO& io = ImGui::GetIO();
            auto bold = io.Fonts->Fonts[0];

            ImGui::PushID(label.c_str());

            ImGui::Text(label.c_str());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

            float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding[1] * 2.0f;
            ImVec2 button_size = { line_height + 3.0f, line_height };

            ImGui::PushItemWidth(column_width);

            ImGui::PushFont(bold);
            ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(100, 0, 0, 150));
            if (ImGui::Button("X", button_size))
            {
                values[0] = reset_value;
                modified = true;
            }
            ImGui::PopStyleColor();
            ImGui::PopFont();
            ImGui::SameLine();
            if (ImGui::DragFloat("##X", &values[0], speed, 0.0f, 0.0f, "%.2f"))
            {
                modified = true;
            }
            ImGui::SameLine();

            ImGui::PushFont(bold);
            ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 100, 0, 150));
            if (ImGui::Button("Y", button_size))
            {
                values[1] = reset_value;
                modified = true;
            }
            ImGui::PopStyleColor();
            ImGui::PopFont();
            ImGui::SameLine();
            if (ImGui::DragFloat("##Y", &values[1], speed, 0.0f, 0.0f, "%.2f"))
            {
                modified = true;
            }

            ImGui::PopStyleVar(2);
            ImGui::PopItemWidth();

            ImGui::PopID();

            return modified;
        }

        bool vector3_modifier::update(const std::string& label, float* values, float reset_value, float speed, float column_width)
        {
            bool modified = false;

            ImGuiIO& io = ImGui::GetIO();
            auto bold = io.Fonts->Fonts[0];

            ImGui::PushID(label.c_str());

            ImGui::Text(label.c_str());
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);

            float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding[1] * 2.0f;
            ImVec2 button_size = { line_height + 3.0f, line_height };

            ImGui::PushItemWidth(column_width);

            ImGui::PushFont(bold);
            ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(100, 0, 0, 150));
            if (ImGui::Button("X", button_size))
            {
                values[0] = reset_value;
                modified = true;
            }
            ImGui::PopStyleColor();
            ImGui::PopFont();
            ImGui::SameLine();
            if (ImGui::DragFloat("##X", &values[0], speed, 0.0f, 0.0f, "%.2f"))
            {
                modified = true;
            }
            ImGui::SameLine();

            ImGui::PushFont(bold);
            ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 100, 0, 150));
            if (ImGui::Button("Y", button_size))
            {
                values[1] = reset_value;
                modified = true;
            }
            ImGui::PopStyleColor();
            ImGui::PopFont();
            ImGui::SameLine();
            if (ImGui::DragFloat("##Y", &values[1], speed, 0.0f, 0.0f, "%.2f"))
            {
                modified = true;
            }
            ImGui::SameLine();

            ImGui::PushFont(bold);
            ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(75, 150, 250, 150));
            if (ImGui::Button("Z", button_size))
            {
                values[2] = reset_value;
                modified = true;
            }
            ImGui::PopStyleColor();
            ImGui::PopFont();
            ImGui::SameLine();
            if (ImGui::DragFloat("##Z", &values[2], speed, 0.0f, 0.0f, "%.2f"))
            {
                modified = true;
            }

            ImGui::PopStyleVar(2);
            ImGui::PopItemWidth();

            ImGui::PopID();

            return modified;
        }

    }
}