#pragma once
#include "core/Application.h"

namespace editor
{

    class editor : public rt::application
    {
    public:
        editor();
        virtual ~editor() override;

        virtual void run() override;
    };

}