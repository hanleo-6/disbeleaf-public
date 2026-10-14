#include "Alga.h"

Alga::Alga(glm::vec3 pos, float yaw, float pitch, int algae_id) {
    mPos = pos;
    mYaw = yaw;
    mPitch = pitch;
    // consistent scale, not constant as in future will want to vary scale
    mScale = glm::vec3(2.6f, 2.6f, 2.6f);
    mAlgaeID = algae_id;
}

AlgaeManager::AlgaeManager(int num_algae) {
    mNumAlgae = num_algae;
    // spawn bounds are -40 to +40 in all axes
    spawn_distribution = std::uniform_real_distribution<float>(-40.0f, 40.0f);
    // angle orientation can be any angle
    orient_distribution = std::uniform_real_distribution<float>(0.0f, 360.0f);
}
std::vector<Alga>& AlgaeManager::GetAlgae() {
    return mAlgae;
}
void AlgaeManager::InitialGeneration() {
    mAlgae.clear();
    // seed generator with current time to get a new set of random values
    generator.seed(time(&timestamp));
    for (int i = 0; i < mNumAlgae; i++) {
        spawnAlga();
    }
}
void AlgaeManager::spawnAlga() {
    glm::vec3 i_coords;
    // randomly generate 3 ordinates
    for (int j = 0; j < 3; j++) {
        i_coords[j] = spawn_distribution(generator);
    }
    // randomly generate orientation
    float rand_yaw = orient_distribution(generator);
    float rand_pitch = orient_distribution(generator);
    // create new algae with these properties
    Alga alg(i_coords, rand_yaw, rand_pitch, idCounter);
    idCounter++;
    mAlgae.push_back(alg);
}
void AlgaeManager::removeAlga(int alg_id) {
    // remove an algae based on its ID
    for (int i = 0; i < mNumAlgae; i++) {
        if (mAlgae[i].mAlgaeID == alg_id) {
            mAlgae.erase(mAlgae.begin() + i);
            break;
        }
    }
}

int AlgaeManager::DetectCollisions(glm::vec3 player_pos, float player_radius) {
    // brute force method (fast enough for 60fps with 500 algae)
    std::queue<int> algae_to_remove;
    int n_collisions;
    for (int i = 0; i < mNumAlgae; i++) {
        // player & algae modelled as spheres
        // calculate distance between centres
        float x = player_pos.x-mAlgae[i].mPos.x;
        float y = player_pos.y-mAlgae[i].mPos.y;
        float z = player_pos.z-mAlgae[i].mPos.z;
        float dist_centres = x*x + y*y + z*z; // this is squared value
        float radii_sum = ALGA_RADIUS + player_radius; // also squared value
        
        if (dist_centres < radii_sum) {
            algae_to_remove.push(mAlgae[i].mAlgaeID);
        }
    }
    n_collisions = algae_to_remove.size();
    for (int i = 0; i < algae_to_remove.size(); i++) {
        int alga_remove_id = algae_to_remove.front();
        algae_to_remove.pop();
        removeAlga(alga_remove_id);
        spawnAlga();
    }
    // return number of algae collided with
    return n_collisions;
}


