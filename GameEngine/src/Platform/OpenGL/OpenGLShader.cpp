#include "gepch.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "GameEngine/Core/Core.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

namespace GameEngine
{
    OpenGLShader::OpenGLShader(const std::string& filepath)
    {
        GE_PROFILE_FUNCTION();

        std::string sourceRaw = ReadFile(filepath);
        std::unordered_map<GLenum, std::string> source = ParseShader(sourceRaw);

        m_id = CompileShader(source);

        // Extract name from filepath
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = filepath.rfind('.');
        auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
        m_name = filepath.substr(lastSlash, count);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& filepath) :
        m_name(name)
    {
        GE_PROFILE_FUNCTION();

        std::string sourceRaw = ReadFile(filepath);
        std::unordered_map<GLenum, std::string> source = ParseShader(sourceRaw);

        m_id = CompileShader(source);
    }

    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) :
        m_name(name)
    {
        GE_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertexSrc;
        sources[GL_FRAGMENT_SHADER] = fragmentSrc;

        m_id = CompileShader(sources);
    }

    OpenGLShader::~OpenGLShader()
    {
        GE_PROFILE_FUNCTION();

        glDeleteProgram(m_id);
    }

    void OpenGLShader::Bind() const
    {
        GE_PROFILE_FUNCTION();

        glUseProgram(m_id);
    }

    void OpenGLShader::Unbind() const
    {
        GE_PROFILE_FUNCTION();

        glUseProgram(0);
    }

    void OpenGLShader::SetUniform1i(const std::string& name, int v)
    {
        GE_PROFILE_FUNCTION();

        glUniform1i(GetUniformLocation(name), v);
    }

    void OpenGLShader::SetUniform1f(const std::string& name, float v)
    {
        GE_PROFILE_FUNCTION();

        glUniform1f(GetUniformLocation(name), v);
    }

    void OpenGLShader::SetUniform2f(const std::string& name, const glm::vec2& value)
    {
        GE_PROFILE_FUNCTION();

        glUniform2f(GetUniformLocation(name), value[0], value[1]);
    }

    void OpenGLShader::SetUniform3f(const std::string& name, const glm::vec3& value)
    {
        GE_PROFILE_FUNCTION();

        glUniform3f(GetUniformLocation(name), value[0], value[1], value[2]);
    }

    void OpenGLShader::SetUniform4f(const std::string& name, const glm::vec4& value)
    {
        GE_PROFILE_FUNCTION();

        glUniform4f(GetUniformLocation(name), value[0], value[1], value[2], value[3]);
    }

    void OpenGLShader::SetUniformMat3f(const std::string& name, const glm::mat3& matrix)
    {
        GE_PROFILE_FUNCTION();

        glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
    }

    void OpenGLShader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
    {
        GE_PROFILE_FUNCTION();

        glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
    }

    int OpenGLShader::GetUniformLocation(const std::string& name)
    {
        GE_PROFILE_FUNCTION();

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

    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        GE_PROFILE_FUNCTION();

        std::string result;
        std::ifstream in(filepath, std::ios::in | std::ios::binary);	
        
        if (in)
        {
            in.seekg(0, std::ios::end);
            result.resize(in.tellg());
            in.seekg(0, std::ios::beg);
            in.read(&result[0], result.size());
            in.close();
            ;
        }
        else
        {
            GE_CORE_ERROR("Could not open file '{0}'", filepath);
        }

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::ParseShader(const std::string& shaderSource)
    {
        GE_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;

        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = shaderSource.find(typeToken, 0);

        while (pos != std::string::npos)
        {
            size_t eol = shaderSource.find_first_of("\r\n", pos);
            GE_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            pos += typeTokenLength + 1;
            std::string type = shaderSource.substr(pos, eol - pos);
            pos = shaderSource.find_first_not_of("\r\n", eol);
            size_t nextPos = shaderSource.find(typeToken, pos);

            if (type == "vertex") 
            {
                shaderSources[GL_VERTEX_SHADER] = shaderSource.substr(pos, nextPos - pos);
            }
            else if (type == "fragment" || type == "pixel")
            {
                shaderSources[GL_FRAGMENT_SHADER] = shaderSource.substr(pos, nextPos - pos);
            }
            else
            {
                GE_CORE_ASSERT(false, "Invalid shader type specified");
            }

            pos = nextPos;
        }

        return shaderSources;
    }

    int OpenGLShader::CompileShader(std::unordered_map<GLenum, std::string> shaderSources)
    {
        GE_PROFILE_FUNCTION();

        uint32_t program = glCreateProgram();
        GE_CORE_ASSERT(shaderSources.size() <= 3, "Too many shaders! We only support 3 for now.")
        std::array<GLenum, 3> glShaderIDs;
        uint32_t glShaderIDIndex = 0;

        // compile
        for (auto& kv : shaderSources)
        {
            GLenum type = kv.first;
            const std::string& source = kv.second;

            GLuint shader = glCreateShader(type); 
            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, 0);
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                std::vector<GLchar> infoLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                glDeleteShader(shader);

                GE_CORE_ERROR("{0}", infoLog.data());
                GE_CORE_ASSERT(false, "Shader compilation failure!");
                break;
            }

            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        // link
        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(program);

            for (auto id : glShaderIDs)
            {
                glDetachShader(program, id);
                glDeleteShader(id);
            }

            GE_CORE_ERROR("{0}", infoLog.data());
            GE_CORE_ASSERT(false, "Shader link failure!");

            return -1;
        }

        // cleanup
        for (auto id : glShaderIDs)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }

        return program;
    }

}
