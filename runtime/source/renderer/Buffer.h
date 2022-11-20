#pragma once
#include <stdint.h>
#include <vector>

namespace rt
{
    namespace gfx
    {

        enum class shader_data_type
        {
            none, float_1, float_2, float_3, float_4, int_1
        };

        class buffer_layout
        {
        public:
            buffer_layout(std::initializer_list<shader_data_type> elements);

            const std::vector<shader_data_type>& get_elements() const { return p_elements; }
            uint32_t get_stride() { return p_stride; }

        private:
            std::vector<shader_data_type> p_elements;
            uint32_t p_stride;
        };

        class vertex_buffer
        {
        public:
            vertex_buffer();
            ~vertex_buffer();

            void bind();
            void unbind();

            void set_data(const void* data, uint32_t size);

        private:
            unsigned int p_id;
        };

        class index_buffer
        {
        public:
            index_buffer();
            ~index_buffer();

            void bind();
            void unbind();

            void set_data(uint32_t* indices, uint32_t count);

            unsigned int get_count() { return p_count; }

        private:
            uint32_t p_count;
            uint32_t p_id;
        };

        class vertex_array
        {
        public:
            vertex_array();
            ~vertex_array();

            void bind();
            void unbind();

            void bind_vertex_buffer(vertex_buffer* vertex_buffer, buffer_layout& layout);
            void bind_index_buffer(index_buffer* index_buffer);

        private:
            unsigned int p_id;
        };

    }
}