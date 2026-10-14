#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
    public:
        unsigned int mID;
        Shader() {};
        
        void Compile(const char* vertexPath, const char* fragmentPath);
        void Use();
        
        void SetInt(const char* name, int value);
        void SetMat4(const char* name, glm::mat4 value);
        void SetVec3(const char* name, glm::vec3 value);

    private:
        void checkCompileErrors(unsigned int object, std::string type);
};
