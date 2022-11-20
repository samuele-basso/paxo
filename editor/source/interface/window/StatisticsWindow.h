#pragma once
#include <memory>
#include "interface/GUIWindow.h"
#include "core/Timing.h"
#include "renderer/Renderer2D.h"

namespace editor
{
    namespace gui
    {

        class statistics : public gui_window
        {
        public:
            statistics(rt::root& root, gui_state& state);

            virtual void update() override;

        private:
            std::shared_ptr<rt::timing> p_timing;
#ifdef RT_DEBUG_ENABLED
            rt::gfx::renderer_2d::statistics& p_renderer_statistics;
#endif
        };

    }
}