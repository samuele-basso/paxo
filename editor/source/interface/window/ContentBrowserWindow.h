#pragma once
#include <filesystem>
#include <memory>

#include "interface/GUIWindow.h"

#include "renderer/Texture.h"

#include "interface/widget/TextureSettings.h"

namespace editor
{

    namespace gui
    {

        class content_browser : public gui_window
        {
        public:
            content_browser(rt::root& root, gui_state& state);

            virtual void update() override;
            bool button_tint(const char* label, ImTextureID user_texture_id, ImVec2& size, ImVec2& uv0, ImVec2& uv1, int frame_padding, ImVec4& bg_col, ImVec4& hover_tint, ImVec4& held_tint);

        private:
            std::unique_ptr<rt::gfx::texture_2d> p_file_icon;
            std::unique_ptr<rt::gfx::texture_2d> p_directory_icon;
            std::unique_ptr<rt::gfx::texture_2d> p_back_icon;

            std::vector<std::filesystem::path> p_path;
            int p_head;

            float p_padding, p_thumbnail_size;

            texture_settings p_texture_settings;
        };

    }

}