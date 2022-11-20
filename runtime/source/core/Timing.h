#pragma once
#include "ISystem.h"

namespace rt
{

    class timing : public isystem
    {
    public:
        timing(root& root);

        virtual ~timing() = default;

        void begin();

        void end();

        double get() const;

    private:
        double p_time_constructor;
        double p_step;
    };

}
