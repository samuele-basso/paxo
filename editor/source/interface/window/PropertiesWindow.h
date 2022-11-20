#pragma once
#include "interface/GUIWindow.h"

#include "interface/widget/NameModifier.h"

#include "extension/CameraController.h"

#include "interface/widget/QuadControl.h"
#include "interface/widget/LineControl.h"
#include "interface/widget/CameraControl.h"
#include "interface/widget/CircleControl.h"

namespace editor
{

    namespace gui
    {

        class properties : public gui_window
        {
        public:
            properties(rt::root& root, gui_state& state);

            virtual void update() override;
            virtual void end() override;

            void component();

        private:
            name_modifier p_name_modifier;
            std::shared_ptr<rt::ecs::scene> p_scene;
            std::shared_ptr<camera_controller> p_cam;
            std::shared_ptr<rt::window> p_win;
            std::shared_ptr<pipeline> p_pipeline;

            bool p_has_quad, p_has_circle, p_has_line, p_has_camera;

            circle_control p_circle_control;
            line_control p_line_control;
            quad_control p_quad_control;
            camera_control p_camera_control;
        };

    }

}