#pragma once
#include <vector>
#include <memory>

#include "core/Log.h"

namespace rt
{

    class isystem;

    class root
    {
    public:
        template <typename T>
        std::shared_ptr<T> get_system()
        {
            for (const auto& s : p_systems)
            {
                if (typeid(*s) == typeid(T)) return std::static_pointer_cast<T>(s);
            }

            RT_ASSERT(false);
        }

        template <typename T>
        void create_system()
        {
            RT_INFO("System creation - {}", typeid(T).name());
            RT_STATIC_ASSERT((std::is_base_of<isystem, T>::value));

            auto system = std::make_shared<T>(*this);
            p_systems.push_back(system);
        }

    private:
        std::vector<std::shared_ptr<isystem>> p_systems;
    };

}