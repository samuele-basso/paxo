#pragma once

#include <string>

namespace editor
{
    namespace gui
    {

        class vector1_modifier
        {
        public:
            bool update(const std::string& label, float* value, float reset_value = 0.0f, float speed = 1.0f, float column_width = 100.0f);
        };

        class vector2_modifier
        {
        public:
            bool update(const std::string& label, float* values, float reset_value = 0.0f, float speed = 1.0f, float column_width = 100.0f);
        };

        class vector3_modifier
        {
        public:
            bool update(const std::string& label, float* values, float reset_value = 0.0f, float speed = 1.0f, float column_width = 100.0f);
        };

    }
}