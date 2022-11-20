#pragma once
#include <memory>

#include "interface/GUIWindow.h"

#include "renderer/Texture.h"

namespace editor
{

    namespace gui
    {

        class hierarchy : public gui_window
        {
        public:
            hierarchy(rt::root& root, gui_state& state);

            virtual void update() override;
            void entity_node(rt::ecs::entity entity);

        private:
            std::shared_ptr<rt::ecs::scene> p_scene;
        };

    }

}