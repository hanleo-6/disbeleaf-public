#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "GameObject.h"

using namespace glm;
// class definition for camera object
class Camera : public GameObject {
    public:
        Camera(vec3 player_pos, float player_dist = 1.8f);
        void Reset(vec3 player_pos);
        mat4 GetViewMatrix(vec3 player_pos);
        void ProcessMouseLooking(float xoffset, float yoffset, vec3 player_pos, float player_yaw);

    private:
        float mDistFromPlayer; // not constant as will be change in future features, in order to dynamically change FOV on movement

        void updatePos(vec3 player_pos, float player_yaw);

        const float mMouseSens = 0.1f;
        const glm::vec3 WORLD_UP = glm::vec3(0.0, 1.0, 0.0); // view calculation needs a reference UP vector
};
