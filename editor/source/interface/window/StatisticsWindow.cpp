#include "StatisticsWindow.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace editor
{
    namespace gui
    {

#ifdef RT_DEBUG_ENABLED
        statistics::statistics(rt::root& root, gui_state& state)
            : gui_window(root, state, "Statistics"), p_renderer_statistics(rt::gfx::renderer_2d::statistics::get())
        {
            p_timing = root.get_system<rt::timing>();
        }
#else
        statistics::statistics(rt::root& root, gui_state& state)
            : gui_window(root, state, "Statistics")
        {
            p_timing = root.get_system<rt::timing>();
        }
#endif

        void statistics::update()
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            float time = p_timing->get();
            ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
            float height = ImGui::GetFrameHeight();
            if (ImGui::BeginViewportSideBar("##MainStatusBar", viewport, ImGuiDir_Down, height, window_flags)) {
                if (ImGui::BeginMenuBar()) {
                    ImGui::Text("Frametime: %f", time);
                    ImGui::EndMenuBar();
                }
                ImGui::End();
            }
            ImGui::PopStyleVar();
        }

    }
}