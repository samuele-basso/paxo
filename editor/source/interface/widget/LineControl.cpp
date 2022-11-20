#include "LineControl.h"

#include <imgui.h>

namespace editor
{
    namespace gui
    {

        bool line_control::update(line* component)
        {
            bool or = false;

            ImGui::PushID("linecontrol");

            if (ImGui::CollapsingHeader("Line"))
            {
                or |= p_vec2mod.update("Position", &component->position.x);
                or |= p_vec1mod.update("Size", &component->size);
                or |= p_vec1mod.update("Rotation", &component->rotation.z);

                ImGui::Text("Color");
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
                or |= ImGui::ColorEdit4("##Color", &component->color.x);
                ImGui::PopStyleVar();
            }

            ImGui::PopID();

            return or;
        }

    }
}