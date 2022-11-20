#include "shader.h"
#include "core/Log.h"

#include <fstream>
#include <sstream>

#include <glad/glad.h>

namespace rt
{
    namespace gfx
    {

        shader::shader(const std::string& vs_path, const std::string& fs_path)
            : p_id(0)
        {
            auto [v, f] = load_shader(vs_path, fs_path);
            create_program(v, f);

            RT_TRACE("Shader {} created", p_id);
        }

        shader::~shader()
        {
            glDeleteProgram(p_id);
            RT_TRACE("Shader {} deleted", p_id);
        }

        void shader::bind()
        {
            glUseProgram(p_id);
        }

        void shader::unbind()
        {
            glUseProgram(0);
        }

        void shader::u_1i(const std::string& name, const int value)
        {
            glUniform1i(u_location(p_id, name), value);
        }

        void shader::u_1iv(const std::string& name, int count, const int* value)
        {
            glUniform1iv(u_location(p_id, name), count, value);
        }

        void shader::u_1uiv(const std::string& name, int count, const unsigned int* value)
        {
            glUniform1uiv(u_location(p_id, name), count, value);
        }

        void shader::u_1fv(const std::string& name, int count, const float* value)
        {
            glUniform1fv(u_location(p_id, name), count, value);
        }

        void shader::u_2fv(const std::string& name, int count, const float* value)
        {
            glUniform2fv(u_location(p_id, name), count, value);
        }

        void shader::u_3fv(const std::string& name, int count, const float* value)
        {
            glUniform3fv(u_location(p_id, name), count, value);
        }

        void shader::u_4fv(const std::string& name, int count, const float* value)
        {
            glUniform4fv(u_location(p_id, name), count, value);
        }

        void shader::u_mat_4fv(const std::string& name, int count, unsigned char transpose, const float* value)
        {
            glUniformMatrix4fv(u_location(p_id, name), count, transpose, value);
        }

        int shader::u_location(unsigned int id, const std::string& name)
        {
            auto location = p_map.find(name);
            if (location != p_map.end())
                return location->second;
            int newLocation = glGetUniformLocation(id, name.c_str());
            if (newLocation != -1)
            {
                p_map.insert(std::make_pair(name, newLocation));
                return newLocation;
            }
            return 0;
        }

        std::tuple<std::string, std::string> shader::load_shader(const std::string& vs_path, const std::string& fs_path)
        {
            std::fstream in;

            in.open(vs_path);

            std::stringstream vs;
            vs << std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

            in.close();
            in.clear();

            in.open(fs_path);

            std::stringstream fs;
            fs << std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

            in.close();
            in.clear();

            return { vs.str(), fs.str() };
        }

        unsigned int shader::compile_shader(unsigned int type, const std::string& source)
        {
            unsigned int ID = glCreateShader(type);
            const char* c_src = source.c_str();
            glShaderSource(ID, 1, &c_src, nullptr);
            glCompileShader(ID);

            int result;
            glGetShaderiv(ID, GL_COMPILE_STATUS, &result);
            if (result == GL_FALSE)
            {
                int length;
                glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &length);
                char* message = (char*)_malloca(length * sizeof(char));

                glGetShaderInfoLog(ID, length, &length, message);

                RT_CRITICAL(message);

                glDeleteShader(ID);
                return 0;
            }

            return ID;
        }

        void shader::create_program(const std::string& vs, const std::string& fs)
        {
            p_id = glCreateProgram();
            unsigned int vs_id = compile_shader(GL_VERTEX_SHADER, vs);
            RT_ASSERT(vs_id, "vertex shader compilation failed");

            unsigned int fs_id = compile_shader(GL_FRAGMENT_SHADER, fs);
            RT_ASSERT(fs_id, "fragment shader compilation failed");

            glAttachShader(p_id, vs_id);
            glAttachShader(p_id, fs_id);

            glLinkProgram(p_id);

            glValidateProgram(p_id);

            glDeleteShader(vs_id);
            glDeleteShader(fs_id);
        }

    }
}