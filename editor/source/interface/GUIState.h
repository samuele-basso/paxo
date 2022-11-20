#pragma once
#include "scene/ECS.h"

namespace editor
{
    namespace gui
    {

        enum class gui_window_type
        {
            none,
            content_browser,
            hierarchy,
            pipeline,
            properties,
            renderer,
            statistics,
            viewport
        };

        struct gui_state
        {
            gui_state() : selected_entity(rt::ecs::entity_null()), selected(gui_window_type::none) { }

            rt::ecs::entity selected_entity;
            gui_window_type selected;
        };

    }
}