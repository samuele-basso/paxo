#pragma once

#include "core/Log.h"
#include "Texture.h"

#include <glad/glad.h>

namespace rt
{
    namespace gfx
    {

        template<unsigned int N>
        class framebuffer
        {
        public:
            framebuffer() : p_attachments(nullptr), p_count(0), p_id(0), p_renderer_buffer(0), p_samples(0), p_is_multisample(false), p_width(0), p_height(0)
            {
                memset(p_textures, 0, sizeof(p_textures));
            }

            framebuffer(unsigned int id) : p_id(id)
            {

            }

            inline void init(texture_internal_format* value, size_t count, int width, int height, bool is_multisample = false, int samples = 4)
            {
                p_is_multisample = is_multisample;
                p_samples = samples;
                p_width = width;
                p_height = height;

                GLint supported_attachments = 0;
                glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &supported_attachments);

                GLint supported_draw_buffers = 0;
                glGetIntegerv(GL_MAX_DRAW_BUFFERS, &supported_draw_buffers);

                RT_ASSERT(supported_attachments > N && supported_draw_buffers > N);

                for (int i = 0; i < N; i++)
                {
                    p_color_attachments[i] = GL_COLOR_ATTACHMENT0 + i;
                }

                reset_attachments();
                p_attachments = new texture_internal_format[count];
                memcpy(p_attachments, value, sizeof(texture_internal_format) * count);
                p_count = count;

                glGenFramebuffers(1, &p_id);

                gen_attachments_internal(width, height);

                glBindFramebuffer(GL_FRAMEBUFFER, p_id);

                RT_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                RT_TRACE("Framebuffer created");
            }

            ~framebuffer()
            {
                reset_attachments();
                clear();
                glDeleteFramebuffers(1, &p_id);
                RT_TRACE("Framebuffer deleted");
            }

            framebuffer(const framebuffer<N>&) = default;

            void clear()
            {
                glDeleteRenderbuffers(1, &p_renderer_buffer);
                for (int i = 0; i < N; i++)
                {
                    if (p_textures[i]) delete p_textures[i];
                }
            }

            void bind()
            {
                glBindFramebuffer(GL_FRAMEBUFFER, p_id);
            }

            void unbind()
            {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }

            void bind_read()
            {
                glBindFramebuffer(GL_READ_FRAMEBUFFER, p_id);
            }

            texture* get_texture(int index) { return p_textures[index]; }

            void gen_attachments(int width, int height)
            {
                clear();

                p_width = width;
                p_height = height;
                gen_attachments_internal(width, height);
            }

            void update_samples(int samples)
            {
                clear();

                p_samples = samples;
                gen_attachments_internal(p_width, p_height);
            }

            void enable_multisampling()
            {
                clear();

                p_is_multisample = true;
                gen_attachments_internal(p_width, p_height);
            }

            void enable_multisampling(int samples)
            {
                clear();

                p_is_multisample = true;
                p_samples = samples;
                gen_attachments_internal(p_width, p_height);
            }

            void disable_multisampling()
            {
                clear();

                p_is_multisample = false;
                gen_attachments_internal(p_width, p_height);
            }

            int get_width() { return p_width; }
            int get_height() { return p_height; }

            int get_samples() { return p_samples; }

            bool is_multisample() { return p_is_multisample; }

        private:
            void gen_attachments_internal(int width, int height)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, p_id);

                glGenRenderbuffers(1, &p_renderer_buffer);
                glBindRenderbuffer(GL_RENDERBUFFER, p_renderer_buffer);

                if (p_is_multisample)
                {
                    glRenderbufferStorageMultisample(GL_RENDERBUFFER, p_samples, GL_DEPTH24_STENCIL8, width, height);

                    for (int i = 0; i < N; i++)
                    {
                        p_textures[i] = new texture_2d_multisample(nullptr, nullptr, 0, width, height, p_attachments[i], p_samples);
                        glFramebufferTexture2D(GL_FRAMEBUFFER, p_color_attachments[i], GL_TEXTURE_2D_MULTISAMPLE, p_textures[i]->get_id(), 0);
                    }
                }
                else
                {
                    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

                    for (int i = 0; i < N; i++)
                    {
                        p_textures[i] = new texture_2d(nullptr, nullptr, 0, width, height, p_attachments[i]);
                        glFramebufferTexture2D(GL_FRAMEBUFFER, p_color_attachments[i], GL_TEXTURE_2D, p_textures[i]->get_id(), 0);
                    }
                }

                glDrawBuffers(N, p_color_attachments);

                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, p_renderer_buffer);
                glBindRenderbuffer(GL_RENDERBUFFER, 0);

                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }

            void reset_attachments()
            {
                if (p_attachments)
                {
                    delete[] p_attachments;
                    p_attachments = nullptr;
                }
                p_count = 0;
            }

        private:
            texture_internal_format* p_attachments;
            size_t p_count;
            texture* p_textures[N];

            unsigned int p_renderer_buffer;
            unsigned int p_id;

            bool p_is_multisample;
            int p_samples;

            int p_width;
            int p_height;

            GLenum p_color_attachments[N];
        };

    }
}