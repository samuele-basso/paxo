#include "CircleControl.h"

#include <imgui.h>

namespace editor
{
    namespace gui
    {

        bool circle_control::update(circle* component)
        {
            bool or = false;

            ImGui::PushID("circlecontrol");

            if (ImGui::CollapsingHeader("Circle"))
            {
                or |= p_vec2mod.update("Position", &component->position.x);
                or |= p_vec1mod.update("Depth", &component->position.z);
                or |= p_vec2mod.update("Size", &component->size.x);
                or |= p_vec1mod.update("Rotation", &component->rotation.z);

                ImGui::Text("Color");
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                or |= ImGui::ColorEdit4("##Color", &component->color.x);
                ImGui::PopStyleVar();

                ImGui::PushID("Thickness");
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                or |= ImGui::DragFloat("Thickness", &component->thickness, 0.1f, 0.0f, 1.0f);
                ImGui::PopStyleVar();
                ImGui::PopID();
                
                ImGui::PushID("Fade");
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                or |= ImGui::DragFloat("Fade", &component->fade, 0.1f, 0.0f, 1.0f);
                ImGui::PopStyleVar();
                ImGui::PopID();
            }

            ImGui::PopID();

            return or ;
        }

    }
}