#include "Camera.h"
using namespace glm; 
// updatePos needs glm namespace to be readable (normally using none or std in other files)

Camera::Camera(vec3 player_pos, float player_dist)  {
    mDistFromPlayer = player_dist;
    Reset(player_pos);
}

void Camera::Reset(vec3 player_pos) {
    mYaw = 0.0f;
    mPitch = 0.0f;
    updatePos(player_pos, 0.0f);
}

mat4 Camera::GetViewMatrix(vec3 player_pos) {
    // use glm calculation for view matrix, based on camera & player position relative to y-axis
    return glm::lookAt(mPos, player_pos, WORLD_UP);
}

void Camera::ProcessMouseLooking(float xoffset, float yoffset, vec3 player_pos, float player_yaw) {
    // update camera relative YAW about player
    mYaw -= xoffset * mMouseSens; // negative as inverted mouse movement relative to angle
    // wrap so values aren't able to go extreme after many turns in same direction
    if (mYaw > 360.0f) {
        mYaw -= 360.0f;
    }
    else if (mYaw < -360.f) {
        mYaw += 360.0f;
    }
    // update camera PITCH about player 
    mPitch -= yoffset * mMouseSens; // negative as inverted mouse movement relative to angle
    // limit range of vertical FOV, don't allow 90 degrees as funny behaviour at extremes
    if (mPitch > 89.0f) {
        mPitch = 89.0f;
    }
    else if (mPitch < -89.0f) {
        mPitch = -89.0f;
    }
    updatePos(player_pos, player_yaw);
}

void Camera::updatePos(vec3 player_pos, float player_yaw) {
    float phi = mYaw; // here would use player_yaw in future, to automatically turn camera softly with player rotation, however currently not needed
    // calculate camera world position using 3D trig
    mPos.x = player_pos.x - (mDistFromPlayer*glm::cos(radians(mPitch)) * glm::sin(radians(phi)));
    mPos.y = player_pos.y + mDistFromPlayer*glm::sin(radians(mPitch));
    mPos.z = player_pos.z - (mDistFromPlayer*glm::cos(radians(mPitch)) * glm::cos(radians(phi)));
}
