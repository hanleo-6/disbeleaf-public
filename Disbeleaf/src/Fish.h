#pragma once

#include <iostream>
#include <numeric>
#include <vector>
#include <queue>

#include "GameObject.h"
// Fish
class Fish : public GameObject {
    public:
        Fish(glm::vec3 pos, glm::vec3 vel);
        glm::vec3 mVel;
        std::queue<int> mNeighbours;

        void CalcAngles();
};

// FishManager
class FishManager {
    public:
        FishManager(int num_fish);
        int GetNumFish();
        std::vector<Fish>& GetFish();
        void InitialGeneration();
        void UpdateAllFishPos(float dt, glm::vec3 player_pos);

        // fish shoaling behaviour options (changeable in real time through imgui menu with dev mode)
        float SCALE_COHESION = 0.1f;
        float SCALE_SEPARATION = 1.2f;
        float SCALE_ALIGNMENT = 0.2f;
        float TURN_FACTOR_FLAT = 2.0f;
        float VIEW_RANGE = 8.6f;
        float VIEW_ANGLE = 120.0f;
        float SPEED_SCALAR = 5.5f;

    private:
        int mNumFish;
        std::vector<Fish> mFish;
        // helper functions for InitialGeneration
        glm::vec3 genFishVel();
        glm::vec3 genFishPos();

        // helper functions for UpdateFishVel:
        // main function to update velocity of a single fish depending on the flocking rules
        void updateFishVel(int ix, glm::vec3 player_pos);
        glm::vec3 getCohesionForce(glm::vec3 POS_SUM, glm::vec3 cur_pos, int n_neighbours);
        glm::vec3 getAlignmentForce(glm::vec3 VEL_SUM, glm::vec3 cur_vel, int n_neighbours);
        glm::vec3 getSeparationForce(glm::vec3 cur_pos, glm::vec3 player_pos);
        void boundFishPos(int ix);
        std::pair<glm::vec3, glm::vec3> sumFishNeighbourProp(int ix);
        void updateFishNeighbours(int ix);
        
        // separate helper function to carry out vector scalar division
        glm::vec3 ScalarDiv(glm::vec3 Dividend, float Divisor);

        // fish shoaling paramters
        const float BASE_COHESION = 1.0f;
        const float BASE_SEPARATION = 1.1f;
        const float BASE_ALIGNMENT = 1.0f;
        const glm::vec3 SPAWN_BOUNDS = {40.0f, 40.0f, 40.0f};
        const int SPAWN_OFFSET = 5;
        const float MAX_SPEED = 12.0f;
        const float MIN_SPEED = 0.01f;
        const float PLAYER_AVOID_WEIGHT = 1.5f;
        const float PLAYER_AVOID_DIST = 25.0f;
};

