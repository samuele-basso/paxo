#include "ECS.h"

#include <fstream>

namespace rt
{

    namespace ecs
    {

        scene::scene(root& root)
            : isystem(root), p_create_next(1), p_serialize_running(false), p_deserialize_running(false)
        {
        }

        void scene::begin_serialize_session()
        {
            p_jserial.clear();

            std::vector<rt::ecs::entity> entities;

            for (int i = 0; i < p_entities.count(); i++)
            {
                entities.push_back(p_entities.at(i));
            }
            p_jserial["entities"] = entities;
            RT_TRACE("{} existing entities serialized", entities.size());

            entities.clear();

            for (int i = 0; i < p_deleted_entities.count(); i++)
            {
                entities.push_back(p_deleted_entities.at(i));
            }
            p_jserial["deleted_entities"] = entities;
            RT_TRACE("{} deleted entities serialized", entities.size());

            p_jserial["create_next"] = p_create_next;
            RT_TRACE("{} next entity", p_create_next);
        }

        void scene::commit_serialize(bool finalize, const std::string& path)
        {
            if (finalize)
            {
                std::ofstream fout;
                fout.open(path.c_str(), std::ios::out);

                fout.write(p_jserial.dump().c_str(), strlen(p_jserial.dump().c_str()));

                fout.close();

                p_jserial.clear();
            }
            else
            {
                p_jserial.clear();
            }
        }

        void scene::begin_deserialize_session(std::string path)
        {
            for (rt::ecs::entity i = 1; i < p_create_next; i++)
            {
                destroy_entity(i);
            }

            while (p_deleted_entities.count())
            {
                p_deleted_entities.remove(0);
            }

            std::ifstream f(path);
            p_jserial = nlohmann::json::parse(f);

            auto entities = p_jserial["entities"];
            for (nlohmann::json::iterator j_it = entities.begin(); j_it != entities.end(); j_it++)
            {
                auto val = j_it.value();
                auto e = val.get<rt::ecs::entity>();
                p_entities.insert(e);
            }
            RT_TRACE("{} entities deserialized", p_entities.count());

            entities = p_jserial["deleted_entities"];
            for (nlohmann::json::iterator j_it = entities.begin(); j_it != entities.end(); j_it++)
            {
                auto val = j_it.value();
                auto e = val.get<rt::ecs::entity>();
                p_deleted_entities.insert(e);
            }
            RT_TRACE("{} deleted entities deserialized", p_deleted_entities.count());

            p_create_next = p_jserial["create_next"];
            RT_TRACE("{} next entity", p_create_next);
        }

        entity scene::generate_entity()
        {
            entity e;

            if (p_deleted_entities.count() > 0)
            {
                e = p_deleted_entities.at(0);
                p_deleted_entities.remove(0);
            }
            else
            {
                e = p_create_next;
                p_create_next++;
                RT_ASSERT(p_create_next > e);
            }

            if (!(p_entities.count() < p_entities.size())) p_entities.expand(p_entities.size() * 2);

            p_entities.insert(e);
            RT_TRACE("Entity {} was generated", e);
            return e;
        }

        void scene::destroy_entity(entity e)
        {
            RT_ASSERT(e);

            size_t e_index = p_entities.find(e);

            if (p_entities.count() > 0)
            {
                entity e_found = p_entities.at(e_index);

                if (e == e_found)
                {
                    if (!(p_deleted_entities.count() < p_deleted_entities.size())) p_deleted_entities.expand(p_deleted_entities.size() * 2);
                    p_deleted_entities.insert(e);
                    p_entities.remove(e_index);

                    if (e < p_on_entity_delete.size())
                    {
                        auto& callbacks = p_on_entity_delete[e];
                        while (callbacks.count() > 0)
                        {
                            callbacks.at(0)(e, *this);
                            callbacks.remove(0);
                        }
                    }

                    RT_TRACE("Entity {} was destroyed", e);
                }
            }
        }

        bool scene::exists_entity(entity e)
        {
            size_t e_index = p_entities.find(e);

            if (p_entities.count() > 0)
            {
                entity e_found = p_entities.at(e_index);
                return e == e_found;
            }

            return false;
        }

        entity entity_null()
        {
            return 0;
        }

    }
}