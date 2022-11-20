#pragma once
#include <string>
#include <unordered_map>

namespace rt
{
    namespace gfx
    {

        class shader
        {
        public:
            shader(const std::string& vs_path, const std::string& fs_path);
            ~shader();

            void bind();
            void unbind();

            void u_1i(const std::string& name, const int value);

            void u_1iv(const std::string& name, int count, const int* value);

            void u_1uiv(const std::string& name, int count, const unsigned int* value);

            void u_1fv(const std::string& name, int count, const float* value);
            void u_2fv(const std::string& name, int count, const float* value);
            void u_3fv(const std::string& name, int count, const float* value);
            void u_4fv(const std::string& name, int count, const float* value);

            void u_mat_4fv(const std::string& name, int count, unsigned char transpose, const float* value);

        private:
            std::unordered_map<std::string, int> p_map;
            int u_location(unsigned int id, const std::string& name);

            std::tuple<std::string, std::string> load_shader(const std::string& vs_path, const std::string& fs_path);

            unsigned int compile_shader(unsigned int type, const std::string& source);

            void create_program(const std::string& vs, const std::string& fs);

            unsigned int p_id;
        };

    }
}