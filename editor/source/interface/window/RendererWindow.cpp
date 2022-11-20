#include "RendererWindow.h"
#include <string>

#include <imgui.h>

namespace editor
{
    namespace gui
    {

        renderer::renderer(rt::root& root, gui_state& state)
            : gui_window(root, state, "Renderer"), p_valid(true), p_combo_box("Samples", (const char**)p_selec, 3)
        {
            p_pipeline = root.get_system<pipeline>();
        }

        void renderer::end()
        {
            if (!p_valid)
            {
                *p_pipeline->msaa_pipe.enabled() = *p_pipeline->draw_pipe.multisample();

                if (!(*p_pipeline->draw_pipe.multisample()))
                    p_pipeline->pipeline_state.target.disable_multisampling();
                else
                    p_pipeline->pipeline_state.target.enable_multisampling();

                p_valid = true;
            }
        }

        void renderer::update()
        {
            if (ImGui::Begin(p_title.c_str()))
            {
                if (ImGui::CollapsingHeader("Post processing"))
                {
                    if (ImGui::Checkbox("##Multisample", p_pipeline->draw_pipe.multisample()))
                    {
                        p_valid = false;
                    }
                    ImGui::SameLine();
                    ImGui::Text("Multisample");

                    if (p_combo_box.update())
                    {
                        p_pipeline->pipeline_state.target.update_samples(p_values[p_combo_box.selected()]);
                    }
                }
            }

            ImGui::End();
        }

    }
}