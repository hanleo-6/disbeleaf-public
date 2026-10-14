#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

#include "GameObject.h"

class Player : public GameObject{
    public:
        Player();
        void Update(int algae_collected); // (calls all the update subroutines every frame) 
        void Reset();
        void AccelForward();
        int GetAlgaeCount();
        float GetPlayerRadius();


        bool MouseDown;
        std::unordered_map<std::string, bool> KeysDown {
            {"W", false},
            {"A", false},
            {"S", false},
            {"D", false},
            {"ESC", false}
        };

    private:
        float mSpeed;
        glm::vec3 mFacingVec;
        int mAlgaeCount;
        void incrementAlgae(int n);

        // Movement + turning
        void tickDecel();
        void processKeyLooking();
        void updateVecPos();

        const float PLAYER_RADIUS = 0.06f; // for collision
        // constant movement settings
        const float ACCEL_INCREMENT = 0.001f;
        const float DECEL_INCREMENT = 0.008f;
        const float SPEED_LIM = 0.1f;
        const float TURNS_SENS = 2.5f;

        // starting position and properties
        const glm::vec3 START_POS = glm::vec3(0.0f, 0.0f, 0.0f);
        const float START_VEL = 0.0f;
        const glm::vec3 START_FACING = glm::vec3(0.0f, 0.0f, -1.0f);
        const float START_YAW = -90.0f;
        const float START_PITCH = 0.0f;
};