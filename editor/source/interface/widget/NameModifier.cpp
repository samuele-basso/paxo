#include "NameModifier.h"

#include <imgui.h>

namespace editor
{
    namespace gui
    {

        bool name_modifier::update(name* component, int width)
        {
            bool modified = false;

            RT_ASSERT(!(component->name.length() > 256));
            std::strncpy(p_buffer, component->name.c_str(), sizeof(p_buffer));

            ImGui::Text("Entity name: ");
            ImGui::SameLine();

            ImGui::PushItemWidth(width);

            if (ImGui::InputText("##namemodifier", p_buffer, sizeof(p_buffer)))
            {
                if (strlen(p_buffer) > 0)
                {
                    component->name = std::string(p_buffer);
                }

                modified = true;
            }

            ImGui::PopItemWidth();

            return modified;
        }

    }
}