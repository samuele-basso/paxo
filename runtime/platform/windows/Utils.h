#pragma once
#include <string>
#include <memory>

#include "core/Window.h"

namespace rt
{

    void open_file_dialog(std::string& path, bool& status, std::shared_ptr<rt::window> win);

}