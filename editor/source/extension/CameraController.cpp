#include "CameraController.h"
#include <GLFW/glfw3.h>

#include "event/Definitions.h"
#include "event/Event.h"

#include "components/Components.h"

namespace editor
{

    camera_controller::camera_controller(rt::root& root)
        : rt::isystem(root)
    {
        p_pipeline = p_root.get_system<pipeline>();
        p_scene = p_root.get_system<rt::ecs::scene>();
        p_input = p_root.get_system<rt::input>();

#if 0
        rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::app_tick_event, rt::evnt::evnt_callback(
            [&](rt::evnt::rt_event* e) {
                auto cam = p_scene->get_component<camera>(p_pipeline->pipeline_state.active_camera);
                if (!cam) return;

                cam->rotation += rt::math::vector3(
                    0.0f,
                    0.0f,
                    -0.01f*p_input->is_key_pressed(GLFW_KEY_E) + 0.01f*p_input->is_key_pressed(GLFW_KEY_Q)
                );

                cam->translation.x +=
                    p_input->is_key_pressed(GLFW_KEY_A) * -cos(cam->rotation.z) +
                    p_input->is_key_pressed(GLFW_KEY_D) * cos(cam->rotation.z) +
                    p_input->is_key_pressed(GLFW_KEY_S) * sin(cam->rotation.z) +
                    p_input->is_key_pressed(GLFW_KEY_W) * -sin(cam->rotation.z);


                cam->translation.y +=
                    p_input->is_key_pressed(GLFW_KEY_A) * -sin(cam->rotation.z) +
                    p_input->is_key_pressed(GLFW_KEY_D) * sin(cam->rotation.z) +
                    p_input->is_key_pressed(GLFW_KEY_S) * -cos(cam->rotation.z) +
                    p_input->is_key_pressed(GLFW_KEY_W) * cos(cam->rotation.z);

                recalculate_view_matrix(cam);
                recalculate_projection_matrix(cam);
            }
        ));

        rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::mouse_scroll_event, rt::evnt::evnt_callback(
            [&](rt::evnt::rt_event* e) {
                rt::evnt::mouse_scroll_event* ms = (rt::evnt::mouse_scroll_event*)e;
                auto cam = p_scene->get_component<camera>(p_pipeline->pipeline_state.active_camera);
                if (!cam) return;

                cam->size += ms->y_offset * 25.0f;
            }
        ));
#endif

        rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::viewport_resize_event, rt::evnt::evnt_callback(
            [&](rt::evnt::rt_event* e) {
                rt::evnt::viewport_resize_event* vr = (rt::evnt::viewport_resize_event*)e;
                auto cam = p_scene->get_component<camera>(p_pipeline->pipeline_state.active_camera);
                if (!cam) return;

                cam->aspect = ((float)vr->width / (float)vr->height);
            }
        ));
    }

    void camera_controller::build_camera(camera* cam)
    {
        auto rend = p_root.get_system<rt::gfx::renderer_2d>();

        *cam = camera{
            0.1f, 1000.0f, 1000.0f, 45.0f, p_pipeline->pipeline_state.viewport_size.x / p_pipeline->pipeline_state.viewport_size.y,
            camera_projection::orthographic, &rend->cam(),
            rt::math::vector3(0.0f, 0.0f, 3.0f), rt::math::vector3(0.0f), rt::math::vector3(0.0f, 1.0f, 0.0f)
        };

        if (cam->active)
        {
            recalculate_view_matrix(cam);
            recalculate_projection_matrix(cam);
        }
    }

    void camera_controller::recalculate_view_matrix(camera* cam)
    {
        rt::math::vector3 front = rt::math::vector3(0.0f, 0.0f, -1.0f);

        rt::math::vector3 right = rt::math::cross(front, cam->world_up);
        right.normalize();

        rt::math::vector3 up = rt::math::cross(right, front);
        up = rt::math::vector3(rt::math::matrix::rotate(rt::math::attitude_to_quaternion(0.0f, 0.0f, (cam->rotation.z))) * rt::math::vector4(up.x, up.y, up.z, 0.0f));
        up.normalize();

        cam->view = rt::math::look_at(cam->translation, cam->translation + front, up);

        cam->cam->view_projection = cam->projection * cam->view;
    }

    void camera_controller::recalculate_projection_matrix(camera* cam)
    {
        switch (cam->mode)
        {
        case camera_projection::orthographic:
        {
            cam->projection = rt::math::orthographic(
                -cam->aspect / 2 * cam->size, cam->aspect / 2 * cam->size,
                -0.5f * cam->size, 0.5f * cam->size,
                cam->plane_near, cam->plane_far
            );
            cam->cam->view_projection = cam->projection * cam->view;
            break;
        }
        case camera_projection::perspective:
        {
            cam->projection = rt::math::perspective(
                rt::math::radians(cam->fov), cam->aspect,
                cam->plane_near, cam->plane_far
            );
            cam->cam->view_projection = cam->projection * cam->view;
            break;
        }
        }
    }

}