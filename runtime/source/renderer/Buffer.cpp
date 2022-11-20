#include "Buffer.h"

#include <glad/glad.h>

#include "core/Log.h"

namespace rt
{
    namespace gfx
    {

        vertex_buffer::vertex_buffer()
            : p_id(0)
        {
            glCreateBuffers(1, &p_id);
            glBindBuffer(GL_ARRAY_BUFFER, p_id);

            RT_TRACE("Created {} vertex_buffer", p_id);
        }

        vertex_buffer::~vertex_buffer()
        {
            RT_TRACE("Deleting {} vertex_buffer", p_id);
            glDeleteBuffers(1, &p_id);
        }

        void vertex_buffer::bind()
        {
            glBindBuffer(GL_ARRAY_BUFFER, p_id);
        }

        void vertex_buffer::unbind()
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void vertex_buffer::set_data(const void* f_Data, uint32_t f_Size)
        {
            glBindBuffer(GL_ARRAY_BUFFER, p_id);
            glBufferData(GL_ARRAY_BUFFER, f_Size, f_Data, GL_STATIC_DRAW);
        }

        index_buffer::index_buffer()
            : p_id(0), p_count(0)
        {
            glCreateBuffers(1, &p_id);
            glBindBuffer(GL_ARRAY_BUFFER, p_id);

            RT_TRACE("Created {} index_buffer", p_id);
        }

        index_buffer::~index_buffer()
        {
            RT_TRACE("Deleting {} index_buffer", p_id);
            glDeleteBuffers(1, &p_id);
        }

        void index_buffer::bind()
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, p_id);
        }

        void index_buffer::unbind()
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        void index_buffer::set_data(uint32_t* f_Indices, uint32_t f_Count)
        {
            p_count = f_Count;
            glBindBuffer(GL_ARRAY_BUFFER, p_id);
            glBufferData(GL_ARRAY_BUFFER, f_Count * sizeof(unsigned int), f_Indices, GL_STATIC_DRAW);
        }

        vertex_array::vertex_array()
            : p_id(0)
        {
            glCreateVertexArrays(1, &p_id);

            RT_TRACE("Created {} vertex_array", p_id);
        }

        vertex_array::~vertex_array()
        {
            RT_TRACE("Deleting {} vertex_array", p_id);
            glDeleteVertexArrays(1, &p_id);
        }

        void vertex_array::bind()
        {
            glBindVertexArray(p_id);
        }

        void vertex_array::unbind()
        {
            glBindVertexArray(0);
        }

        static GLenum shader_element_type(shader_data_type element)
        {
            switch (element)
            {
                case shader_data_type::float_1: return GL_FLOAT;
                case shader_data_type::float_2: return GL_FLOAT;
                case shader_data_type::float_3: return GL_FLOAT;
                case shader_data_type::float_4: return GL_FLOAT;
                case shader_data_type::int_1:   return GL_INT;
            }
            return 0;
        }

        static uint32_t shader_data_type_size(shader_data_type type)
        {
            switch (type)
            {
                case shader_data_type::float_1: return 4;
                case shader_data_type::float_2: return 4 * 2;
                case shader_data_type::float_3: return 4 * 3;
                case shader_data_type::float_4: return 4 * 4;
                case shader_data_type::int_1:   return 4;
            }
            return 0;
        }

        static uint32_t shader_data_type_count(shader_data_type type)
        {
            switch (type)
            {
                case shader_data_type::float_1: return 1;
                case shader_data_type::float_2: return 2;
                case shader_data_type::float_3: return 3;
                case shader_data_type::float_4: return 4;
                case shader_data_type::int_1:   return 1;
            }
            return 0;
        }

        void vertex_array::bind_vertex_buffer(vertex_buffer* vertex_buffer, buffer_layout& layout)
        {
            glBindVertexArray(p_id);
            vertex_buffer->bind();

            int a = 0;
            int offset = 0;
            for (shader_data_type e : layout.get_elements())
            {
                glVertexAttribPointer(a, shader_data_type_count(e), shader_element_type(e), GL_FALSE, layout.get_stride(), (void*)offset);
                glEnableVertexAttribArray(a);
                a++;
                offset += shader_data_type_size(e);
            }
        }

        void vertex_array::bind_index_buffer(index_buffer* index_buffer)
        {
            glBindVertexArray(p_id);
            index_buffer->bind();
        }

        buffer_layout::buffer_layout(std::initializer_list<shader_data_type> elements)
            : p_stride(0)
        {
            p_elements = elements;
            p_stride = 0;
            for (shader_data_type e : p_elements)
            {
                p_stride += shader_data_type_size(e);
            }
        }

    }
}
