#pragma once
#include "components/Components.h"
#include "VectorModifier.h"
#include "TextureSettings.h"

#include "renderer/Texture.h"

#include "core/Window.h"

namespace editor
{
    namespace gui
    {

        class quad_control
        {
        public:
            quad_control();

            bool update(quad* component, texture_2d* texture, std::shared_ptr<rt::window>& win);

            rt::gfx::texture_parameter_value* get_texture_value() { return p_texture_settings.get_values(); }

            bool texture_update, path;
            std::string texture_path;

        private:
            vector1_modifier p_vec1mod;
            vector2_modifier p_vec2mod;

            texture_settings p_texture_settings;

            std::unique_ptr<rt::gfx::texture_2d> p_default;
        };

    }
}