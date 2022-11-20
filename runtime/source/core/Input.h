#pragma once
#include "core/ISystem.h"
#include "core/Window.h"

#include <memory>

namespace rt
{

    class input : public isystem
    {
    public:
        input(root& root);
        virtual ~input() = default;

    public:
        void init();

        bool is_key_pressed(int key_code);
        bool is_mouse_key_pressed(int key_code);

    private:
        std::shared_ptr<window> p_window;
    };

}