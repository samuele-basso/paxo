#pragma once
#include "renderer/Framebuffer.h"
#include "math/Vector2.h"

#include "scene/ECS.h"

namespace editor
{

    struct pipeline_state
    {
        pipeline_state()
            : active_camera(0)
        {
        }

        rt::gfx::framebuffer<1> target;
#ifndef PX_STANDALONE
        rt::gfx::framebuffer<1> target_msaa;
#endif

        rt::ecs::entity active_camera;
        rt::math::vector2 viewport_size;
    };

}