#include "ViewportWindow.h"
#include <imgui.h>

#include "event/Event.h"

namespace editor
{
    namespace gui
    {

        viewport::viewport(rt::root& root, gui_state& state)
            : gui_window(root, state, "Viewport"), p_valid(true)
        {
            p_window = p_root.get_system<rt::window>();
            p_pipeline = p_root.get_system<pipeline>();
        }

        void viewport::begin()
        {
            if (!p_valid && p_pipeline->pipeline_state.viewport_size.x > 0 && p_pipeline->pipeline_state.viewport_size.y > 0)
            {
                rt::evnt::viewport_resize_event e(p_window->get_GLFWwindow(), p_pipeline->pipeline_state.viewport_size.x, p_pipeline->pipeline_state.viewport_size.y);
                rt::evnt::evnt::fire(rt::evnt::rt_event_type::viewport_resize_event, &e);
                p_valid = true;
            }
        }

        void viewport::update()
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(500.0f, 500.0f));
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(3.0f, 3.0f));

            if (ImGui::Begin(p_title.c_str()))
            {
                if (ImGui::IsWindowFocused())
                {
                    p_state.selected = gui_window_type::viewport;
                }

                auto& size = ImGui::GetContentRegionAvail();
                if (p_pipeline->pipeline_state.viewport_size.x != size.x || p_pipeline->pipeline_state.viewport_size.y != size.y)
                {
                    p_pipeline->pipeline_state.viewport_size.x = size.x;
                    p_pipeline->pipeline_state.viewport_size.y = size.y;
                    p_valid = false;
                }

                bool multisample = p_pipeline->pipeline_state.target.is_multisample();

                ImTextureID id = (ImTextureID)(multisample ? p_pipeline->pipeline_state.target_msaa.get_texture(0)->get_id() : p_pipeline->pipeline_state.target.get_texture(0)->get_id());

                ImGui::Image(id, size);
            }
            ImGui::End();

            ImGui::PopStyleVar(2);
        }

    }
}