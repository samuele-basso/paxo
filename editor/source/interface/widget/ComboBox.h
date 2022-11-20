#pragma once
#include <string>
#include <imgui.h>

namespace editor
{
    namespace gui
    {

        class combo_box
        {
        public:
            combo_box(const std::string& label, const char** selec, size_t count);

            bool update();

            size_t selected() { return p_selected; }

        private:
            const char** p_selec;
            size_t p_count;
            size_t p_selected;

            const std::string p_label;
        };

    }
}