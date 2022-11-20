#include "GUIWindow.h"

#include <imgui.h>

namespace editor
{
    namespace gui
    {

        gui_window::gui_window(rt::root& root, gui_state& state, const std::string& title)
            : p_root(root), p_state(state), p_title(title)
        {
        }

        void gui_window::begin()
        {
        }

        void gui_window::update()
        {
            ImGui::Begin(p_title.c_str());
            ImGui::End();
        }

        void gui_window::end()
        {
        }

    }
}