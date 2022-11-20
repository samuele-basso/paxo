#pragma once
#include "renderer/Texture.h"
#include "ComboBox.h"

namespace editor
{
    namespace gui
    {

        class texture_settings
        {
        public:
            texture_settings();

            rt::gfx::texture_parameter_value* get_values();

            bool update();

        private:
            const char* p_str_filters[2] = { "Nearest", "Linear" };
            const char* p_wraps[2] = { "Clamp to edge", "Repeat" };

            combo_box p_min_filter_combo;
            combo_box p_mag_filter_combo;
            combo_box p_wrap_s_combo;
            combo_box p_wrap_t_combo;

            rt::gfx::texture_parameter_value p_values[4];
        };

    }
}