#pragma once
#include "core/ISystem.h"

#include <memory>

#include "scene/ECS.h"

#include "window/PropertiesWindow.h"
#include "window/StatisticsWindow.h"
#include "window/ContentBrowserWindow.h"
#include "window/PipelineWindow.h"
#include "window/RendererWindow.h"
#include "window/ViewportWindow.h"
#include "window/HierarchyWindow.h"

#include "GUIState.h"

namespace editor
{
    namespace gui
    {

        class gui : public rt::isystem
        {
        public:
            gui(rt::root& root);

            void begin();
            void update();
            void end();

            gui_state state;

            properties properties;
            statistics statistics;
            content_browser content_browser;
            pipeline_control pipeline_control;
            renderer renderer;
            viewport viewport;
            hierarchy hierarchy;

        private:
            std::shared_ptr<rt::ecs::scene> p_scene;
        };

    }
}