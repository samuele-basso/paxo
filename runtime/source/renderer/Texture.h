#pragma once
#include <string>
#include "math/Vector4.h"

namespace rt
{
    namespace gfx
    {

        enum class texture_parameter
        {
            wrap_s, wrap_t, wrap_r,
            filter_min, filter_mag
        };

        enum class texture_parameter_value
        {
            clamp_to_edge, repeat,
            nearest, linear
        };

        class texture
        {
        public:
            texture();
            virtual ~texture();
            texture(const texture&) = default;

            const unsigned int get_id() const { return p_id; }
            void bind(int slot);

            int get_width() { return p_width; }
            int get_height() { return p_height; }

        protected:
            void set_format(int channels);
            void apply_parameters(texture_parameter* parameters, texture_parameter_value* values, size_t count);

        protected:
            unsigned int p_id;
            int p_width, p_height;

            unsigned int p_format;
            unsigned int p_internal_format;
            unsigned int p_target;
        };

        enum class texture_internal_format
        {
            none, rgb8, rgba8, rgba16f, rgba32f, r8
        };

        class texture_2d : public texture
        {
        public:
            texture_2d();
            texture_2d(texture_parameter* parameters, texture_parameter_value* values, size_t count, const math::vector4& color);
            texture_2d(texture_parameter* parameters, texture_parameter_value* values, size_t count, const std::string& path);
            texture_2d(texture_parameter* parameters, texture_parameter_value* values, size_t count, int width, int height, int channels, void* data);
            texture_2d(texture_parameter* parameters, texture_parameter_value* values, size_t count, int width, int height, texture_internal_format internal_format);
            texture_2d(texture_parameter* parameters, texture_parameter_value* values, size_t count, int width, int height, texture_internal_format internal_format, void* data);
            virtual ~texture_2d() = default;
            texture_2d(const texture_2d&) = default;
        };

        class texture_2d_multisample : public texture
        {
        public:
            texture_2d_multisample(texture_parameter* parameters, texture_parameter_value* values, size_t count, int width, int height, texture_internal_format internal_format, int samples);
            virtual ~texture_2d_multisample() = default;
            texture_2d_multisample(const texture_2d_multisample&) = default;

        private:
            int p_samples;
        };

    }
}