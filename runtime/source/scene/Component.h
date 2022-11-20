#pragma once
#include <json.hpp>

namespace rt
{

    struct component
    {
    public:
        virtual nlohmann::json serialize() = 0;
        virtual void deserialize(nlohmann::json dt) = 0;
    };

}