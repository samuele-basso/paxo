#include "Texture.h"
#include "core/Log.h"
#include "math/Math.h"

#include <stb_image/stb_image.h>
#include <glad/glad.h>


namespace rt
{
    namespace gfx
    {

        texture::texture()
            : p_id(0), p_width(0), p_height(0), p_format(0), p_internal_format(0), p_target(0)
        {
        }

        texture::~texture()
        {
            RT_TRACE("Deleting {} texture object", p_id);
            glDeleteTextures(1, &p_id);
        }

        void texture::bind(int slot)
        {
            glBindTextureUnit(slot, p_id);
        }

        static GLenum TextureParameterType(texture_parameter parameter)
        {
            switch (parameter)
            {
                case texture_parameter::wrap_s:     return GL_TEXTURE_WRAP_S;
                case texture_parameter::wrap_t:     return GL_TEXTURE_WRAP_T;
                case texture_parameter::wrap_r:     return GL_TEXTURE_WRAP_R;
                case texture_parameter::filter_min: return GL_TEXTURE_MIN_FILTER;
                case texture_parameter::filter_mag: return GL_TEXTURE_MAG_FILTER;
            }
            return 0;
        }

        static GLenum TextureParameterTypeValue(texture_parameter_value value)
        {
            switch (value)
            {
                case texture_parameter_value::clamp_to_edge: return GL_CLAMP_TO_EDGE;
                case texture_parameter_value::repeat:        return GL_REPEAT;
                case texture_parameter_value::nearest:       return GL_NEAREST;
                case texture_parameter_value::linear:        return GL_LINEAR;
            }
            return 0;
        }

        void texture::apply_parameters(texture_parameter* parameters, texture_parameter_value* values, size_t count)
        {
            for (size_t i = 0; i < count; i++)
            {
                glTextureParameteri(p_id, TextureParameterType(parameters[i]), TextureParameterTypeValue(values[i]));
            }
        }

        void texture::set_format(int channels)
        {
            switch (channels)
            {
                case 1:
                {
                    p_format = GL_RED;
                    p_internal_format = GL_R8;
                    break;
                }
                case 2:
                {
                    p_format = GL_RG;
                    p_internal_format = GL_RG8;
                    break;
                }
                case 3:
                {
                    p_format = GL_RGB;
                    p_internal_format = GL_RGB8;
                    break;
                }
                case 4:
                {
                    p_format = GL_RGBA;
                    p_internal_format = GL_RGBA8;
                    break;
                }
                default:
                {
                    RT_ASSERT(false);
                }
            }
        }

        texture_2d::texture_2d()
            : texture()
        {
        }

        texture_2d::texture_2d(texture_parameter* parameters, texture_parameter_value* values, size_t count, const math::vector4& color)
        {
            p_width = 1;
            p_height = 1;
            p_format = GL_RGBA;
            p_internal_format = GL_RGBA8;

            glCreateTextures(GL_TEXTURE_2D, 1, &p_id);
            glTextureStorage2D(p_id, 1, p_internal_format, p_width, p_height);

            apply_parameters(parameters, values, count);

            glTextureSubImage2D(p_id, 0, 0, 0, p_width, p_height, p_format, GL_FLOAT, &color.x);
            glGenerateTextureMipmap(p_id);

            RT_TRACE("Created texture_2d - id: {} width: {} height: {}", p_id, p_width, p_height);
        }

        texture_2d::texture_2d(texture_parameter* parameters, texture_parameter_value* values, size_t count, const std::string& path)
        {
            stbi_set_flip_vertically_on_load(1);

            int channels = 0;

            stbi_uc* image = nullptr;
            image = stbi_load(path.c_str(), &p_width, &p_height, &channels, 0);

            if (image)
            {
                set_format(channels);

                glCreateTextures(GL_TEXTURE_2D, 1, &p_id);
                glTextureStorage2D(p_id, 1, p_internal_format, p_width, p_height);

                apply_parameters(parameters, values, count);

                glTextureSubImage2D(p_id, 0, 0, 0, p_width, p_height, p_format, GL_UNSIGNED_BYTE, image);
                glGenerateTextureMipmap(p_id);

                stbi_image_free(image);
            }

            RT_TRACE("Created texture_2d - id: {} width: {} height: {}", p_id, p_width, p_height);
        }

        texture_2d::texture_2d(texture_parameter* parameters, texture_parameter_value* values, size_t count, int width, int height, int channels, void* data)
        {
            p_width = width;
            p_height = height;

            set_format(channels);

            glCreateTextures(GL_TEXTURE_2D, 1, &p_id);
            glTextureStorage2D(p_id, 1, p_internal_format, p_width, p_height);

            apply_parameters(parameters, values, count);

            glTextureSubImage2D(p_id, 0, 0, 0, p_width, p_height, p_format, GL_UNSIGNED_BYTE, data);
            glGenerateTextureMipmap(p_id);

            RT_TRACE("Created texture_2d - id: {} width: {} height: {}", p_id, p_width, p_height);
        }

        static GLenum get_internal_format(texture_internal_format value)
        {
            switch (value)
            {
                case texture_internal_format::r8:       return GL_R8_SNORM;
                case texture_internal_format::rgb8:     return GL_RGB8;
                case texture_internal_format::rgba8:    return GL_RGBA8;
                case texture_internal_format::rgba16f:  return GL_RGBA16F;
                case texture_internal_format::rgba32f:  return GL_RGBA32F;
            }
            return 0;
        }

        texture_2d::texture_2d(texture_parameter* parameters, texture_parameter_value* values, size_t count, int width, int height, texture_internal_format internal_format)
        {
            p_width = width;
            p_height = height;

            p_format = p_internal_format = get_internal_format(internal_format);

            glCreateTextures(GL_TEXTURE_2D, 1, &p_id);
            glTextureStorage2D(p_id, 1, p_internal_format, p_width, p_height);

            apply_parameters(parameters, values, count);

            RT_TRACE("Created texture_2d - id: {} width: {} height: {}", p_id, p_width, p_height);
        }

        texture_2d::texture_2d(texture_parameter* parameters, texture_parameter_value* values, size_t count, int width, int height, texture_internal_format internal_format, void* data)
        {
            p_width = width;
            p_height = height;

            p_format = p_internal_format = get_internal_format(internal_format);

            glCreateTextures(GL_TEXTURE_2D, 1, &p_id);
            glTextureStorage2D(p_id, 1, p_internal_format, p_width, p_height);

            apply_parameters(parameters, values, count);

            glTextureSubImage2D(p_id, 0, 0, 0, p_width, p_height, GL_RGBA, GL_FLOAT, data);

            RT_TRACE("Created texture_2d - id: {} width: {} height: {}", p_id, p_width, p_height);
        }

        texture_2d_multisample::texture_2d_multisample(texture_parameter* parameters, texture_parameter_value* values, size_t count, int width, int height, texture_internal_format internal_format, int samples)
        {
            p_width = width;
            p_height = height;

            p_format = p_internal_format = get_internal_format(internal_format);

            p_samples = samples;

            glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &p_id);
            glTextureStorage2DMultisample(p_id, p_samples, p_internal_format, p_width, p_height, true);

            apply_parameters(parameters, values, count);

            RT_TRACE("Created texture_2d_multisample - id: {} samples: {} width: {} height: {}", p_id, p_samples, p_width, p_height);
        }
    }
}