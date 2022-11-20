#pragma once
#include "math/Vector2.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "renderer/Texture.h"
#include "renderer/Camera.h"
#include "renderer/Renderer2D.h"
#include "scene/Component.h"

#include "extension/CameraController.h"

namespace editor
{
    struct name : public rt::component
    {
        std::string name;

        virtual nlohmann::json serialize() override
        {
            return nlohmann::json{
                name
            };
        }

        virtual void deserialize(nlohmann::json dt) override
        {
            name = dt[0];
        }

        static const char* serialize_key()
        {
            return "component_name";
        }
    };

    struct quad : public rt::component
    {
        quad()
            : position({ 0.0f, 0.0f, 0.0f }), size({ 100.0f, 100.0f }), rotation({ 0.0f, 0.0f, 0.0f }), color({ 0.8f, 0.8f, 0.8f, 1.0f }), active_texture(false), tint(false)
        {
        }

        rt::math::vector3 position;
        rt::math::vector2 size;
        rt::math::vector3 rotation;
        rt::math::vector4 color;
        bool active_texture;
        bool tint;

        virtual nlohmann::json serialize() override
        {
            return nlohmann::json{
                position.x, position.y, position.z,
                size.x, size.y,
                rotation.x, rotation.y, rotation.z,
                color.x, color.y, color.z, color.w,
                active_texture, tint
            };
        }

        virtual void deserialize(nlohmann::json dt) override
        {
            position = { dt[0], dt[1], dt[2] };
            size = { dt[3], dt[4] };
            rotation = { dt[5], dt[6], dt[7] };
            color = { dt[8], dt[9], dt[10], dt[11] };
            active_texture = dt[12];
            tint = dt[13];
        }

        static const char* serialize_key()
        {
            return "component_quad";
        }
    };

    struct circle : public rt::component
    {
        circle()
            : position({ 0.0f, 0.0f, 0.0f }), size({ 100.0f, 100.0f }), rotation({ 0.0f, 0.0f, 0.0f }), color({ 0.8f, 0.8f, 0.8f, 1.0f }), thickness(1.0f), fade(0.5f)
        {
        }

        rt::math::vector3 position;
        rt::math::vector2 size;
        rt::math::vector3 rotation;
        rt::math::vector4 color;
        float thickness;
        float fade;

        virtual nlohmann::json serialize() override
        {
            return nlohmann::json{
                position.x, position.y, position.z,
                size.x, size.y,
                rotation.x, rotation.y, rotation.z,
                color.x, color.y, color.z, color.w,
                thickness, fade
            };
        }

        virtual void deserialize(nlohmann::json dt) override
        {
            position = { dt[0], dt[1], dt[2] };
            size = { dt[3], dt[4] };
            rotation = { dt[5], dt[6], dt[7] };
            color = { dt[8], dt[9], dt[10], dt[11] };
            thickness = dt[12];
            fade = dt[13];
        }

        static const char* serialize_key()
        {
            return "component_circle";
        }
    };

    struct line : public rt::component
    {
        line()
            : position({ 0.0f, 0.0f, 0.0f }), size(100.0f), rotation({ 0.0f, 0.0f, 0.0f }), color({ 0.8f, 0.8f, 0.8f, 1.0f })
        {
        }

        rt::math::vector3 position;
        float size;
        rt::math::vector3 rotation;
        rt::math::vector4 color;

        virtual nlohmann::json serialize() override
        {
            return nlohmann::json{
                position.x, position.y, position.z,
                size,
                rotation.x, rotation.y, rotation.z,
                color.x, color.y, color.z, color.w
            };
        }

        virtual void deserialize(nlohmann::json dt) override
        {
            position = { dt[0], dt[1], dt[2] };
            size = dt[3];
            rotation = { dt[4], dt[5], dt[6] };
            color = { dt[7], dt[8], dt[9], dt[10] };
        }

        static const char* serialize_key()
        {
            return "component_line";
        }
    };

    enum class camera_projection
    {
        perspective,
        orthographic
    };

    struct camera : public rt::component
    {
        camera() = default;

        rt::evnt::evnt_callback ec{[&](rt::evnt::rt_event* e) {
            rt::evnt::viewport_resize_event* vr = (rt::evnt::viewport_resize_event*)e;
            this->aspect = ((float)vr->width / (float)vr->height);

            if (this->active)
            {
                camera_controller::recalculate_view_matrix(this);
                camera_controller::recalculate_projection_matrix(this);
            }
        }};

        camera(float pn, float pf, float size, float fov, float aspect,
               camera_projection mode, rt::gfx::camera* cam,
               rt::math::vector3& translation, rt::math::vector3& rotation, rt::math::vector3& world_up
        )
            : plane_near(pn), plane_far(pf), size(size), fov(fov), aspect(aspect), mode(mode), cam(cam),
              translation(translation), rotation(rotation), world_up(world_up), active(false)
        {
            rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::viewport_resize_event, ec);
        }

        ~camera()
        {
            rt::evnt::evnt::unsubscribe(rt::evnt::rt_event_type::viewport_resize_event, ec);
        }

        float plane_near;

        float plane_far;

        float size;
        float fov;

        float aspect;

        camera_projection mode;
        rt::gfx::camera* cam;

        rt::math::vector3 translation;
        rt::math::vector3 rotation;

        rt::math::vector3 world_up;

        rt::math::matrix view;
        rt::math::matrix projection;

        bool active;

        virtual nlohmann::json serialize() override
        {
            return nlohmann::json{
                plane_near, plane_far, size, fov, aspect, mode,
                translation.x, translation.y, translation.z,
                rotation.x, rotation.y, rotation.z,
                world_up.x, world_up.y, world_up.z,
                view.c0_r0, view.c0_r1, view.c0_r2, view.c0_r3,
                view.c1_r0, view.c1_r1, view.c1_r2, view.c1_r3,
                view.c2_r0, view.c2_r1, view.c2_r2, view.c2_r3,
                view.c3_r0, view.c3_r1, view.c3_r2, view.c3_r3,
                projection.c0_r0, projection.c0_r1, projection.c0_r2, projection.c0_r3,
                projection.c1_r0, projection.c1_r1, projection.c1_r2, projection.c1_r3,
                projection.c2_r0, projection.c2_r1, projection.c2_r2, projection.c2_r3,
                projection.c3_r0, projection.c3_r1, projection.c3_r2, projection.c3_r3
            };
        }

        virtual void deserialize(nlohmann::json dt) override
        {
            plane_near = dt[0];
            plane_far = dt[1];
            size = dt[2];
            fov = dt[3];
            aspect = dt[4];

            mode = dt[5];

            cam = nullptr;

            translation = { dt[6], dt[7], dt[8] };
            rotation = { dt[9], dt[10], dt[11] };
            world_up = { dt[12], dt[13], dt[14] };
            view = { dt[15], dt[16], dt[17], dt[18], dt[19], dt[20], dt[21], dt[22], dt[23], dt[24], dt[25], dt[26], dt[27], dt[28], dt[29], dt[30] };
            projection = { dt[31], dt[32], dt[33], dt[34], dt[35], dt[36], dt[37], dt[38], dt[39], dt[40], dt[41], dt[42], dt[43], dt[44], dt[45], dt[46] };

            active = false;

            rt::evnt::evnt::subscribe(rt::evnt::rt_event_type::viewport_resize_event, ec);
        }

        static const char* serialize_key()
        {
            return "component_camera";
        }
    };

    struct texture_2d : public rt::component
    {
        texture_2d() : from_file(false), path(""), color(0.0f) { }

        texture_2d(rt::gfx::texture_parameter* parameters, rt::gfx::texture_parameter_value* values, size_t count, const rt::math::vector4& color)
            : from_file(false), path(""), color(color)
        {
            parameters_values_setup(parameters, values, count);
            texture.~texture_2d();
            new (&texture) rt::gfx::texture_2d(parameters, values, count, color);
        }

        texture_2d(rt::gfx::texture_parameter* parameters, rt::gfx::texture_parameter_value* values, size_t count, const std::string& path)
            : from_file(true), path(path), color(0.0f)
        {
            parameters_values_setup(parameters, values, count);
            texture.~texture_2d();
            new (&texture) rt::gfx::texture_2d(parameters, values, count, path);
        }

        rt::gfx::texture_2d texture;

        bool from_file;
        std::string path;
        rt::math::vector4 color;

        std::vector<rt::gfx::texture_parameter> parameters;
        std::vector<rt::gfx::texture_parameter_value> values;

        void parameters_values_setup(rt::gfx::texture_parameter* parameters, rt::gfx::texture_parameter_value* values, size_t count)
        {
            for (size_t i = 0; i < count; i++)
            {
                this->parameters.push_back(parameters[i]);
                this->values.push_back(values[i]);
            }
        }

        virtual nlohmann::json serialize() override
        {
            return nlohmann::json {
                from_file, path, color.x, color.y, color.z, color.w,
                parameters, values, parameters.size()
            };
        }

        virtual void deserialize(nlohmann::json dt) override
        {
            std::vector<rt::gfx::texture_parameter> parameters = dt[6];

            std::vector<rt::gfx::texture_parameter_value> values = dt[7];

            if (dt[0])
            {
                new (&texture) rt::gfx::texture_2d(parameters.data(), values.data(), dt[8], dt[1]);
            }
            else
            {
                new (&texture) rt::gfx::texture_2d(parameters.data(), values.data(), dt[8], rt::math::vector4{ dt[2], dt[3], dt[4], dt[5] });
            }
        }

        static const char* serialize_key()
        {
            return "component_texture2d";
        }
    };

}