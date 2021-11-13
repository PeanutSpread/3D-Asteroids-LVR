#ifndef SHADERS_H_
#define SHADERS_H_

#include "glshell.h"  // includes all necessary GL headers

#include <string>

class ShaderProgram {

    GLuint mProgId;

public:
    ShaderProgram();
    ShaderProgram(const std::string& vsPath, const std::string& fsPath);
    ~ShaderProgram();

    bool isValid() const;

    void load(const std::string& vsPath, const std::string& fsPath);
    void unload();

    void activate() const;
    void deactivate() const;

    // an interface for sending uniform variables to the program
    // NOTE: you must first call activate() method

    void sendUniform(const std::string& name, GLfloat scalar);
    void sendUniformInt(const std::string& name, GLint scalar);
    void sendUniform(const std::string& name, const glm::vec2& vec);
    void sendUniform(const std::string& name, const glm::vec3& vec);
    void sendUniform(const std::string& name, const glm::vec4& vec);
    void sendUniform(const std::string& name, const glm::mat3& mat);
    void sendUniform(const std::string& name, const glm::mat4& mat);

    GLint getUniformLocation(const std::string& name);

    void sendUniform(GLint location, GLfloat scalar);
    void sendUniformInt(GLint location, GLint scalar);
    void sendUniform(GLint location, const glm::vec2& vec);
    void sendUniform(GLint location, const glm::vec3& vec);
    void sendUniform(GLint location, const glm::vec4& vec);
    void sendUniform(GLint location, const glm::mat3& mat);
    void sendUniform(GLint location, const glm::mat4& mat);
};

#endif
