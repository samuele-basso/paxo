#pragma once
#include "components/Components.h"
#include "VectorModifier.h"

namespace editor
{
    namespace gui
    {

        class circle_control
        {
        public:
            bool update(circle* component);

        private:
            vector1_modifier p_vec1mod;
            vector2_modifier p_vec2mod;
        };

    }
}