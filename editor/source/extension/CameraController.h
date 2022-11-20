#pragma once
#include "renderer/Renderer2D.h"
#include "core/ISystem.h"

#include "core/Root.h"

#include "scene/ECS.h"
#include "core/Input.h"
#include "extension/Pipeline.h"


namespace editor
{

    struct camera;

    class camera_controller : public rt::isystem
    {
    public:
        camera_controller(rt::root& root);

        void build_camera(camera* cam);

        static void recalculate_view_matrix(camera* cam);
        static void recalculate_projection_matrix(camera* cam);


    private:
        std::shared_ptr<pipeline> p_pipeline;
        std::shared_ptr<rt::ecs::scene> p_scene;
        std::shared_ptr<rt::input> p_input;
    };

}