#pragma once
#include <stdint.h>

#include "container/Vector.h"
#include "container/Array.h"
#include "core/Configuration.h"
#include "memory/Pool.h"
#include "core/ISystem.h"
#include "core/Root.h"

#include <json.hpp>
#include "Component.h"

#define RT_INITIAL_ENTITY_COUNT 1024
#define RT_COMPONENT_STORAGE_INITIAL_COUNT 1024

namespace rt
{
    namespace ecs
    {
        using entity = uint32_t;

        entity entity_null();

        template <typename T>
        struct entity_component
        {
            entity e;
            T component;
        };

        template <typename T>
        using component_pool = memory::pool<entity_component<T>>;

        template <typename T>
        using component_handle = memory::handle<memory::ops::aligned_object<entity_component<T>>>;

        template <typename T>
        struct entity_handle_beacon
        {
            entity e;
            component_handle<T> handle;
        };

        template <typename T>
        bool operator<(const entity_handle_beacon<T>& lhb, const entity_handle_beacon<T>& rhb)
        {
            return lhb.e < rhb.e;
        }

        template <typename T>
        struct component_storage
        {
            component_pool<T> pool;
            container::sorted_vector<entity_handle_beacon<T>, RT_COMPONENT_STORAGE_INITIAL_COUNT> beacons;
        };

        class scene : public isystem
        {
        public:
            scene(root& root);

            template <typename T>
            void attach_component(entity e)
            {
                RT_TRACE("Attaching {} component to entity: {}", typeid(T).name(), e);

                auto& storage = get_component_pool<T>();
                entity_handle_beacon empty_beacon = entity_handle_beacon<T>{ e, 0, 0 };

                size_t index = storage.beacons.find(empty_beacon);
                if (index < storage.beacons.count())
                {
                    entity_handle_beacon<T> beacon = storage.beacons.at(index);
                    if (beacon.e == e)
                    {
                        RT_WARN("Entity {} has {} component", e, typeid(T).name());
                        return;
                    }
                }

                component_handle<T> handle = storage.pool.malloc();
                storage.pool.dereference(handle)->instance.e = e;

                entity_handle_beacon<T> temp{ e, handle };
                storage.beacons.insert(temp);

                remove_components_on_e_destroy(
                    e,
                    [](entity e, scene& cs) {
                        cs.remove_component<T>(e);
                    }
                );
            }

            template <typename T>
            void remove_component(entity e)
            {
                RT_TRACE("Removing {} component from entity: {}", typeid(T).name(), e);
                auto& storage = get_component_pool<T>();

                entity_handle_beacon empty_beacon = entity_handle_beacon<T>{ e, 0, 0 };
                size_t index = storage.beacons.find(empty_beacon);

                if (index < storage.beacons.count())
                {
                    entity_handle_beacon<T> beacon = storage.beacons.at(index);
                    if (beacon.e == e)
                    {
                        storage.beacons.remove(index);
                        storage.pool.free(beacon.handle);
                    }
                }
            }

            template <typename T>
            bool has_component(entity e)
            {
                auto& storage = get_component_pool<T>();

                entity_handle_beacon empty_beacon = entity_handle_beacon<T>{ e, 0, 0 };
                size_t index = storage.beacons.find(empty_beacon);

                if (index < storage.beacons.count())
                {
                    entity_handle_beacon<T> beacon = storage.beacons.at(index);
                    return (beacon.e == e);
                }

                return false;
            }

            template <typename T>
            T* get_component(entity e)
            {
                component_handle<T> handle = get_component_handle<T>(e);
                return component_handle_to_pointer<T>(handle);
            }

            template <typename T>
            component_handle<T> get_component_handle(entity e)
            {
                auto& storage = get_component_pool<T>();

                entity_handle_beacon empty_beacon = entity_handle_beacon<T>{ e, 0, 0 };
                size_t index = storage.beacons.find(empty_beacon);

                if (index < storage.beacons.count())
                {
                    entity_handle_beacon<T> beacon = storage.beacons.at(index);
                    if (beacon.e == e)
                    {
                        return beacon.handle;
                    }
                }

                return component_handle<T>{ 0, 0 };
            }

            template <typename T>
            T* component_handle_to_pointer(component_handle<T>& handle)
            {
                auto& storage = get_component_pool<T>();
                auto ptr = storage.pool.dereference(handle);
                if (ptr)
                {
                    return &(ptr->instance.component);
                }
                else
                {
                    return nullptr;
                }
            }

            template <typename T>
            decltype(auto) entity_component_iterator()
            {
                auto& storage = get_component_pool<T>();
                return storage.pool.iterator();
            }

            void begin_serialize_session();

            template <typename T>
            void serialize_component()
            {
                RT_STATIC_ASSERT((std::is_base_of<component, T>::value));
                std::vector<nlohmann::json> j_sub;

                auto it = entity_component_iterator<T>();
                auto* o = it.next();
                while (o)
                {
                    j_sub.push_back(nlohmann::json{
                        o->instance.e,
                        o->instance.component.serialize()
                    });
                    o = it.next();
                }

                p_jserial[T::serialize_key()] = j_sub;
            }

            template <typename T>
            void serialize_system_state()
            {
                RT_STATIC_ASSERT((std::is_base_of<isystem, T>::value));

                auto sys = p_root.get_system<T>();

                p_jserial[T::serialize_key()] = sys->serialize();
            }

            void commit_serialize(bool finalize, const std::string& path);

            void begin_deserialize_session(std::string path);

            template <typename T>
            void deserialize_component()
            {
                RT_STATIC_ASSERT((std::is_base_of<component, T>::value));

                auto j_sub = p_jserial[T::serialize_key()];
                for (nlohmann::json::iterator j_it = j_sub.begin(); j_it != j_sub.end(); j_it++)
                {
                    auto val = j_it.value();

                    attach_component<T>(val[0]);
                    get_component<T>(val[0])->deserialize(val[1]);
                }
            }

            template <typename T>
            void deserialize_system_state()
            {
                RT_STATIC_ASSERT((std::is_base_of<isystem, T>::value));

                auto sys = p_root.get_system<T>();
                sys->deserialize(p_jserial[T::serialize_key()]);
            }

        private:
            template <typename T>
            static component_storage<T>& get_component_pool()
            {
                static component_storage<T> instance;
                return instance;
            }

            nlohmann::json p_jserial;
            bool p_serialize_running;
            bool p_deserialize_running;

        public:
            entity generate_entity();

            void destroy_entity(entity e);

            bool exists_entity(entity e);

            template <typename T>
            void each(T func)
            {
                for (size_t i = 0; i < p_entities.count(); i++)
                {
                    func(p_entities.at(i));
                }
            }

            container::sorted_vector<entity, RT_INITIAL_ENTITY_COUNT>& get_entities() { return p_entities; }
            container::vector<entity, RT_INITIAL_ENTITY_COUNT>& get_deleted_entities() { return p_deleted_entities; }
            entity& next() { return p_create_next; }

        private:
            void remove_components_on_e_destroy(entity e, std::function<void(entity, scene&)> func)
            {
                if (p_on_entity_delete.size() - 1 < e) p_on_entity_delete.expand(e + 16);
                auto& callbacks = p_on_entity_delete[e];
                callbacks.insert(func);
            }

            container::sorted_vector<entity, RT_INITIAL_ENTITY_COUNT> p_entities;
            container::vector<entity, RT_INITIAL_ENTITY_COUNT> p_deleted_entities;

            container::dynamic_array<
                container::vector<std::function<void(entity, scene&)>, 16>,
                RT_INITIAL_ENTITY_COUNT
            > p_on_entity_delete;

            entity p_create_next;
        };
    }
}