#include "CameraControl.h"

#include "extension/CameraController.h"

#include <imgui.h>

namespace editor
{
    namespace gui
    {

        bool camera_control::update(camera* component, pipeline_state& pipeline_state, gui_state& gui_state)
        {
            bool modified = false;
            bool view_update = false;
            bool projection_update = false;

            ImGui::PushID("cameracontrol");

            if (ImGui::CollapsingHeader("Camera"))
            {
                if (ImGui::Checkbox("Active", &component->active))
                {
                    projection_update = view_update = component->active;
                    modified = true;
                    pipeline_state.active_camera = gui_state.selected_entity;
                }

                view_update |= p_vec3mod.update("Position", &component->translation.x);
                view_update |= p_vec1mod.update("Rotation", &component->rotation.z);
                projection_update |= p_vec1mod.update("Size", &component->size);
                projection_update |= p_vec1mod.update("Near plane", &component->plane_near);
                projection_update |= p_vec1mod.update("Far plane", &component->plane_far);

                if (view_update && component->active) camera_controller::recalculate_view_matrix(component);
                if (projection_update && component->active) camera_controller::recalculate_projection_matrix(component);
            }

            ImGui::PopID();

            return modified | view_update | projection_update;
        }

    }
}