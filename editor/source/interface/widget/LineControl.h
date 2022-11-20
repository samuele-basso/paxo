#pragma once
#include "components/Components.h"
#include "VectorModifier.h"

namespace editor
{
    namespace gui
    {

        class line_control
        {
        public:
            bool update(line* component);

        private:
            vector1_modifier p_vec1mod;
            vector2_modifier p_vec2mod;
        };

    }
}