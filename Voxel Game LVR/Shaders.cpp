#include "Shaders.h"
#include "common.h"  // ReadTextFile()
#include <iostream>

ShaderProgram::ShaderProgram()
    : mProgId(0)
{
}

ShaderProgram::ShaderProgram(const std::string& vsPath, const std::string& fsPath)
    : mProgId(0)
{
    load(vsPath, fsPath);
}

ShaderProgram::~ShaderProgram()
{
    unload();
}

bool ShaderProgram::isValid() const
{
    return mProgId != 0;
}

void ShaderProgram::load(const std::string& vsPath, const std::string& fsPath)
{
    // load shader source code
    std::string vsString = ReadTextFile(vsPath);
    std::string fsString = ReadTextFile(fsPath);

    // create vertex shader object
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);

    // attach vertex shader source code
    const char* vsCString = vsString.c_str();
    glShaderSource(vs, 1, &vsCString, NULL);

    // compile vertex shader
    glCompileShader(vs);

    CHECK_GL_SHADER(vs, "vertex shader");

    // create fragment shader object
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    // attach fragment shader source code
    const char* fsCString = fsString.c_str();
    glShaderSource(fs, 1, &fsCString, NULL);

    // compile fragment shader
    glCompileShader(fs);

    CHECK_GL_SHADER(fs, "fragment shader");

    // create GPU program
    mProgId = glCreateProgram();

    // attach vertex and fragment shaders to program
    glAttachShader(mProgId, vs);
    glAttachShader(mProgId, fs);

    // link program
    glLinkProgram(mProgId);

    CHECK_GL_PROGRAM(mProgId, "GPU program");

    // shader objects are no longer needed, so release them
    glDeleteShader(vs);
    glDeleteShader(fs);
}

void ShaderProgram::unload()
{
    if (mProgId)
        glDeleteProgram(mProgId);
    mProgId = 0;
}

void ShaderProgram::activate() const
{
    glUseProgram(mProgId);
}

void ShaderProgram::deactivate() const
{
    glUseProgram(0);
}

void ShaderProgram::sendUniform(const std::string& name, GLfloat scalar)
{
    GLint location = glGetUniformLocation(mProgId, name.c_str());
    glUniform1f(location, scalar);
}

void ShaderProgram::sendUniformInt(const std::string& name, GLint scalar)
{
    GLint location = glGetUniformLocation(mProgId, name.c_str());
    glUniform1i(location, scalar);
}

void ShaderProgram::sendUniform(const std::string& name, const glm::vec2& vec)
{
    GLint location = glGetUniformLocation(mProgId, name.c_str());
    glUniform2fv(location, 1, glm::value_ptr(vec));
}

void ShaderProgram::sendUniform(const std::string& name, const glm::vec3& vec)
{
    GLint location = glGetUniformLocation(mProgId, name.c_str());
    glUniform3fv(location, 1, glm::value_ptr(vec));
}

void ShaderProgram::sendUniform(const std::string& name, const glm::vec4& vec)
{
    GLint location = glGetUniformLocation(mProgId, name.c_str());
    glUniform4fv(location, 1, glm::value_ptr(vec));
}

void ShaderProgram::sendUniform(const std::string& name, const glm::mat3& mat)
{
    GLint location = glGetUniformLocation(mProgId, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::sendUniform(const std::string& name, const glm::mat4& mat)
{
    GLint location = glGetUniformLocation(mProgId, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

GLint ShaderProgram::getUniformLocation(const std::string& name)
{
    return glGetUniformLocation(mProgId, name.c_str());
}

void ShaderProgram::sendUniform(GLint location, GLfloat scalar)
{
    glUniform1f(location, scalar);
}

void ShaderProgram::sendUniformInt(GLint location, GLint scalar)
{
    glUniform1i(location, scalar);
}

void ShaderProgram::sendUniform(GLint location, const glm::vec2& vec)
{
    glUniform2fv(location, 1, glm::value_ptr(vec));
}

void ShaderProgram::sendUniform(GLint location, const glm::vec3& vec)
{
    glUniform3fv(location, 1, glm::value_ptr(vec));
}

void ShaderProgram::sendUniform(GLint location, const glm::vec4& vec)
{
    glUniform4fv(location, 1, glm::value_ptr(vec));
}

void ShaderProgram::sendUniform(GLint location, const glm::mat3& mat)
{
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::sendUniform(GLint location, const glm::mat4& mat)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}
