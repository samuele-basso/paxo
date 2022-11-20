#include "PipelineWindow.h"

#include <imgui.h>

namespace editor
{
    namespace gui
    {

        pipeline_control::pipeline_control(rt::root& root, gui_state& state)
            : gui_window(root, state, "Pipeline")
        {
            p_pipeline = root.get_system<pipeline>();
        }

        void pipeline_control::update()
        {
            if (ImGui::Begin(p_title.c_str()))
            {
                if (ImGui::CollapsingHeader("Draw"))
                {
                    ImGui::Checkbox("Enabled", p_pipeline->draw_pipe.enabled());
                    ImGui::Checkbox("Blend", p_pipeline->draw_pipe.blend());
                    ImGui::Checkbox("Clear", p_pipeline->draw_pipe.clearonprocess());
                }

                if (ImGui::CollapsingHeader("Multisample"))
                {
                    ImGui::PushID("Multisample");
                    ImGui::Checkbox("Enabled", p_pipeline->msaa_pipe.enabled());
                    ImGui::PopID();
                }
            }
            ImGui::End();
        }

    }
}