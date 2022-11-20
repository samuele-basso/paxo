#pragma once
#include "core/Root.h"

#include "GUIState.h"

#include <string>

namespace editor
{
    namespace gui
    {

        class gui_window
        {
        public:
            gui_window(rt::root& root, gui_state& state, const std::string& title);

            virtual void begin();
            virtual void update();
            virtual void end();

        protected:
            const std::string p_title;

            rt::root& p_root;
            gui_state& p_state;
        };

    }
}