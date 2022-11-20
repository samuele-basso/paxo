#pragma once
#include "components/Components.h"
#include "VectorModifier.h"
#include "extension/PipelineState.h"
#include "interface/GUIState.h"


namespace editor
{
    namespace gui
    {

        class camera_control
        {
        public:
            bool update(camera* component, pipeline_state& pipeline_state, gui_state& gui_state);

        private:
            vector1_modifier p_vec1mod;
            vector3_modifier p_vec3mod;
        };

    }
}