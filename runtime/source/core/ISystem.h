#pragma once

#include <json.hpp>

namespace rt
{

    class root;

    class isystem
    {
    public:
        isystem(root& sim) : p_root(sim) { }
        virtual ~isystem() = default;

        virtual nlohmann::json serialize() { return { }; }
        virtual void deserialize(nlohmann::json&) { }

    protected:
        root& p_root;
    };

}