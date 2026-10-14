#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class GameObject {
    public:
        glm::vec3 mPos;
        float mYaw, mPitch;
        glm::vec3 mScale; // for scaling model when rendering
};
