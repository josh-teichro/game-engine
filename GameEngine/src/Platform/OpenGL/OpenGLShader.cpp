#include "gepch.h"
#include "OpenGLShader.h"
#include "GameEngine/Core.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

namespace GameEngine
{

    OpenGLShader::OpenGLShader(const std::string& filepath) :
        m_filepath(filepath)
    {
        struct ShaderProgramSource source = ParseShader(filepath);

        m_id = glCreateProgram();
        unsigned int vs = CompileShader(GL_VERTEX_SHADER, source.vertexSource);
        unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, source.fragmentSource);

        glAttachShader(m_id, vs);
        glAttachShader(m_id, fs);
        glLinkProgram(m_id);
        glValidateProgram(m_id);

        // check that shaders linked correctly (TODO: move to separate function?)
        GLint isLinked = 0;
        glGetProgramiv(m_id, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(m_id, maxLength, &maxLength, &infoLog[0]);

            glDetachShader(m_id, vs);
            glDetachShader(m_id, fs);
            glDeleteProgram(m_id);
            glDeleteShader(vs);
            glDeleteShader(fs);
            return;
        }

        glDetachShader(m_id, vs);
        glDetachShader(m_id, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);

        Bind();
    }

    OpenGLShader::~OpenGLShader()
    {
        glDeleteProgram(m_id);
    }

    void OpenGLShader::Bind() const
    {
        glUseProgram(m_id);
    }

    void OpenGLShader::Unbind() const
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetUniform1i(const std::string& name, int v)
    {
        glUniform1i(GetUniformLocation(name), v);
    }

    void OpenGLShader::SetUniform1f(const std::string& name, float v)
    {
        glUniform1f(GetUniformLocation(name), v);
    }

    void OpenGLShader::SetUniform4f(const std::string& name, float v1, float v2, float v3, float v4)
    {
        glUniform4f(GetUniformLocation(name), v1, v2, v3, v4);
    }

    void OpenGLShader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
    {
        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
    }

    int OpenGLShader::GetUniformLocation(const std::string& name)
    {
        if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
            return m_uniformLocationCache[name];
        }

        int location = glGetUniformLocation(m_id, name.c_str());
        if (location == -1)
        {
            std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
        }

        m_uniformLocationCache[name] = location;
        return location;
    }

    ShaderProgramSource OpenGLShader::ParseShader(const std::string& filepath)
    {
        std::ifstream stream(filepath);

        if (stream.fail()) {
            GE_CORE_ERROR("Could not find shader. File does not exist!");
            return { "", "" };
        }

        enum class ShaderType
        {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };

        ShaderType type = ShaderType::NONE;
        std::string line;
        std::stringstream ss[2];
        while (getline(stream, line))
        {
            if (line.find("shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                {
                    type = ShaderType::VERTEX;
                }
                else if (line.find("fragment") != std::string::npos)
                {
                    type = ShaderType::FRAGMENT;
                }
            }
            else
            {
                ss[(int)type] << line << '\n';
            }
        }

        return { ss[0].str(), ss[1].str() };
    }

    int OpenGLShader::CompileShader(unsigned int type, const std::string& source)
    {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);

        // Error checking
        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            GE_CORE_ERROR("{0}", message);
            if (type == GL_VERTEX_SHADER) {
                message = "Vertex shader failed to compile!";
            }
            else {
                message = "Fragment shader failed to compile!";
            }
            GE_CORE_ASSERT(false, message);

            glDeleteShader(id);
            return 0;
        }

        return id;
    }

}
