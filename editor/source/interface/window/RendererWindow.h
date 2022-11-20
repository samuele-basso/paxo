#pragma once
#include "interface/GUIWindow.h"
#include "extension/Pipeline.h"

#include "interface/widget/ComboBox.h"

namespace editor
{
    namespace gui
    {

        class renderer : public gui_window
        {
        public:
            renderer(rt::root& root, gui_state& state);

            virtual void update() override;
            virtual void end() override;

        private:
            bool p_valid;
            std::shared_ptr<pipeline> p_pipeline;

            const char* p_selec[3] = { "4", "8", "16" };
            const int p_values[3] = { 4, 8, 16 };
            combo_box p_combo_box;
        };

    }
}