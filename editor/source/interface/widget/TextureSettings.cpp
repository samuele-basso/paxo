#include "TextureSettings.h"
#include <imgui.h>

namespace editor
{
    namespace gui
    {

        texture_settings::texture_settings()
            : p_min_filter_combo("min filter", (const char**)p_str_filters, 2), p_mag_filter_combo("mag filter", (const char**)p_str_filters, 2),
            p_wrap_s_combo("wrap s", (const char**)p_wraps, 2), p_wrap_t_combo("wrap t", (const char**)p_wraps, 2)
        {
        }

        rt::gfx::texture_parameter_value* texture_settings::get_values()
        {
            p_values[0] = (rt::gfx::texture_parameter_value)(2 + p_min_filter_combo.selected());
            p_values[1] = (rt::gfx::texture_parameter_value)(2 + p_mag_filter_combo.selected());
            p_values[2] = (rt::gfx::texture_parameter_value)p_wrap_s_combo.selected();
            p_values[3] = (rt::gfx::texture_parameter_value)p_wrap_t_combo.selected();

            return p_values;
        }

        bool texture_settings::update()
        {
            bool or = false;

            or |= p_min_filter_combo.update();
            or |= p_mag_filter_combo.update();
            or |= p_wrap_s_combo.update();
            or |= p_wrap_t_combo.update();

            return or;
        }

    }
}