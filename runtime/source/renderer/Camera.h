#pragma once
#include "math/Matrix.h"
#include "math/Vector3.h"

namespace rt
{
    namespace gfx
    {

        class camera
        {
        public:
            rt::math::matrix view_projection;
        };

    }
}