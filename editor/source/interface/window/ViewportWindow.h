#pragma once
#include "interface/GUIWindow.h"
#include "extension/Pipeline.h"

#include "core/Window.h"

namespace editor
{
    namespace gui
    {

        class viewport : public gui_window
        {
        public:
            viewport(rt::root& root, gui_state& state);

            virtual void begin() override;
            virtual void update() override;

        private:
            bool p_valid;
            std::shared_ptr<rt::window> p_window;
            std::shared_ptr<pipeline> p_pipeline;
        };

    }
}