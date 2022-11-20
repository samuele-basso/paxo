#pragma once
#include "components/Components.h"

namespace editor
{

    namespace gui
    {

        class name_modifier
        {
        public:
            bool update(name* name, int width = 100.0f);

        private:
            char p_buffer[256];
        };

    }


}