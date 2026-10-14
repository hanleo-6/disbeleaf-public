#include "Shader.h"

#include <glad/glad.h>
#include <iostream>

using namespace std;

void Shader::Use() {
    glUseProgram(mID);
}

void Shader::Compile(const char* vertex_source, const char* fragment_source) {
    unsigned int vertexShader, fragmentShader;
    // compile vertex shader source (code)
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_source, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    // compile fragment shader source (code)
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_source, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    // link with shader program
    mID = glCreateProgram();
    glAttachShader(mID, vertexShader);
    glAttachShader(mID, fragmentShader);
    glLinkProgram(mID);
    checkCompileErrors(mID, "PROGRAM");
    
    // cleanup
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
}

// subroutines to set values of variables in shader glsl code
void Shader::SetInt(const char* name, int value) {
    glUniform1i(glGetUniformLocation(mID, name), value);
}
void Shader::SetVec3(const char* name, glm::vec3 value) {
    glUniform3fv(glGetUniformLocation(mID, name), 1, glm::value_ptr(value));
}
void Shader::SetMat4(const char* name, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(mID, name), 1, false, glm::value_ptr(value));
}

// error checking taken from learnopengl.com
// https://learnopengl.com/Getting-started/Shaders
void Shader::checkCompileErrors(unsigned int object, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << "\n";
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << "\n";
        }
    }
}