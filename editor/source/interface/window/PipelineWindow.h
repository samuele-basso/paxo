#pragma once
#include "interface/GUIWindow.h"

#include "extension/Pipeline.h"

namespace editor
{

    namespace gui
    {

        class pipeline_control : public gui_window
        {
        public:
            pipeline_control(rt::root& root, gui_state& state);

            virtual void update() override;

        private:
            std::shared_ptr<pipeline> p_pipeline;
        };

    }

}