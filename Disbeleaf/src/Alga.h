#pragma once

#include <random>
#include <iostream>
#include <vector>
#include <queue>
#include <time.h>

#include "GameObject.h"

class Alga : public GameObject{
    public:
        int mAlgaeID;
        Alga(glm::vec3 pos, float yaw, float pitch, int algae_id);
};

class AlgaeManager {
    public:
        AlgaeManager(int num_algae);
        int mNumAlgae;

        std::vector<Alga>& GetAlgae();
        void InitialGeneration();
        int DetectCollisions(glm::vec3 player_pos, float player_radius);

    private:
        std::vector<Alga> mAlgae;
        int idCounter = 0;
        void spawnAlga();
        void removeAlga(int alg_id);

        const float ALGA_RADIUS = 0.15f;

        std::mt19937 generator;
        time_t timestamp;
        std::uniform_real_distribution<float> spawn_distribution;
        std::uniform_real_distribution<float> orient_distribution;
};

