#include "Fish.h"
using namespace std;

/////////////////////////////////////////////
// FISH
Fish::Fish(glm::vec3 pos, glm::vec3 vel) {
    mPos = pos;
    mVel = vel;
    mYaw = 0.0f;
    mPitch = 0.0f;
    // consistent scale, not constant as in future will want to vary scale
    mScale = glm::vec3(0.35f, 0.35f, 0.35f);
    CalcAngles();
}

void Fish::CalcAngles() {
    // yaw calculated by arctan(x/z)
    mYaw = glm::degrees(glm::atan(mVel.x / mVel.z));
    // if velocity is in opposite direction to positive z axis (default facing direction)
    // then flip yaw by 180 so fish is rotated correctly
    // if (glm::dot(glm::normalize(mVel), glm::vec3(0.0f, 0.0f, 1.0f)) < 0.0f) {
    //     mYaw += 180.0f;
    // }
    // if (mVel.x >= 0 && mVel.y >= 0) {
    //     don't change yaw
    // }
    if (mVel.x <= 0 && mVel.z >= 0) {
        mYaw = 180.0f - mYaw;
    }
    else if (mVel.x >= 0 && mVel.z <= 0) {
        mYaw = -mYaw;
    }
    else if (mVel.x <= 0 && mVel.z <= 0) {
        mYaw += 180.0f;
    }

    // pitch calculated by arctan(y/xz)
    mPitch = glm::degrees(glm::atan(mVel.y / glm::length(glm::vec2(mVel.x, mVel.z))));
    // if velocity is in opposite direction to negative y axis (default upwards direction)
    // then flip pitch
    if (glm::dot(glm::normalize(mVel), glm::vec3(0.0f, -1.0f, 0.0f)) < 0.0f) {
        mPitch = -mPitch;
    }
    

}
/////////////////////////////////////////////

/////////////////////////////////////////////
// FISH MANAGER
FishManager::FishManager(int num_fish) {
    mNumFish = num_fish;
}
vector<Fish>& FishManager::GetFish() {
    return mFish;
}
int FishManager::GetNumFish() {
    return mNumFish;
}
void FishManager::InitialGeneration() {
    for (int i = 0; i < mNumFish; i++) {
        
        glm::vec3 cur_fish_vel = genFishVel();
        glm::vec3 cur_fish_pos = genFishPos();

        Fish new_fish = Fish(cur_fish_pos, cur_fish_vel);
        mFish.push_back(new_fish);
    }
}
glm::vec3 FishManager::genFishVel() {
    int posneg[2] = {1, -1};
    // randomly make x, y, z velocities positive or negative
    int neg_x = posneg[rand() % 2];
    int neg_y = posneg[rand() % 2];
    int neg_z = posneg[rand() % 2];
    // return random velocities under max
    return glm::vec3{neg_x * rand() % (int)MAX_SPEED, neg_y * rand() % (int)MAX_SPEED, neg_z * rand() % (int)MAX_SPEED};
}
glm::vec3 FishManager::genFishPos() {
    float x, y, z;
    // randomly make x, y, z coords positive or negative
    int posneg[2] = {1, -1};
    int neg_x = posneg[rand() % 2];
    int neg_y = posneg[rand() % 2];
    int neg_z = posneg[rand() % 2];
    // spawn fish within +/- 5 units (spawn_offset) of spawn bounds
    x = (neg_x)*SPAWN_BOUNDS.x + (neg_x)*(rand() % SPAWN_OFFSET + 1);
    y = (neg_y)*SPAWN_BOUNDS.y + (neg_y)*(rand() % SPAWN_OFFSET + 1);
    z = (neg_z)*SPAWN_BOUNDS.z + (neg_z)*(rand() % SPAWN_OFFSET + 1);
    glm::vec3 fish_pos = {x, y, z};
    return fish_pos;
}

void FishManager::UpdateAllFishPos(float dt, glm::vec3 player_pos) {
    for (int i = 0; i < mNumFish; i++)
    {
        Fish * cur_fish = &mFish[i];
        // update velocity based on shoaling rules
        updateFishVel(i, player_pos);
        // update position by velocity * time, for smoother movement, according to frame rate
        cur_fish->mPos += cur_fish->mVel * dt;
        cur_fish->CalcAngles(); // recalculate rotation
    }
}
void FishManager::updateFishVel(int ix, glm::vec3 player_pos)
{
    // Calculate acceleration based on shoaling rules and add to current velocity
    Fish * cur_fish = &mFish[ix];

    pair<glm::vec3, glm::vec3> neighbour_prop = sumFishNeighbourProp(ix);
    glm::vec3 POS_SUM = neighbour_prop.first;
    glm::vec3 VEL_SUM = neighbour_prop.second;
    int n_neighbours = cur_fish->mNeighbours.size();

    // calculating acceleration is based on 3 main rules: cohesion, alignment and separation
    glm::vec3 acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cohesion_force = getCohesionForce(POS_SUM, cur_fish->mPos, n_neighbours);
    // cout << cohesion_force.x << "\n";
    glm::vec3 alignment_force = getAlignmentForce(VEL_SUM, cur_fish->mVel, n_neighbours);
    // cout << "align" << alignment_force.x << "\n";
    glm::vec3 separation_force = getSeparationForce(cur_fish->mPos, player_pos);
    // cout << separation_force.x << "\n";
    acceleration = cohesion_force + alignment_force + separation_force;

    // add acceleration to velocity, while normalising it to bound velocity
    cur_fish->mVel += acceleration;
    float speed = length(cur_fish->mVel);
    glm::vec3 direction = ScalarDiv(cur_fish->mVel, speed);
    speed *= SPEED_SCALAR;
    speed = min(speed, MAX_SPEED);
    speed = max(speed, MIN_SPEED);
    cur_fish->mVel = direction * speed;    

    // if out of bounds, then velocity will just be set to return to bounds
    boundFishPos(ix);
}

pair<glm::vec3, glm::vec3> FishManager::sumFishNeighbourProp(int ix)
{
    // sum the positions and velocities of neighbours of current fish index ix
    // for calculating cohesion + alignment rules
    glm::vec3 pos_res = {0, 0, 0};
    glm::vec3 vel_res = {0, 0, 0};
    updateFishNeighbours(ix);

    while (!mFish[ix].mNeighbours.empty())
    {
        // pop all neighbours from queue, adding its properties
        int j = mFish[ix].mNeighbours.front();
        mFish[ix].mNeighbours.pop();
        pos_res += mFish[j].mPos;
        vel_res += mFish[j].mVel;
    }
    return pair(pos_res, vel_res);
}
void FishManager::updateFishNeighbours(int ix)
{
    // update which fish are in range and in view angle of current fish index ix
    for (int j = 0; j < mNumFish; j++)
    {
        glm::vec3 AB = mFish[ix].mPos - mFish[j].mPos;
        // check VIEW RANGE
        bool in_view = glm::length(AB) < VIEW_RANGE;

        // check VIEW ANGLE
        // a . b
        // ------ = cos(theta)
        // |a| |b|
        bool in_angle = glm::acos(glm::dot(mFish[ix].mVel, AB) / 
                        (glm::length(mFish[ix].mVel) * glm::length(AB))) < VIEW_ANGLE;

        // if in range and in view angle (and not self), then push to neighbours queue to be processed
        if (ix != j && in_view && in_angle)
        {
            mFish[ix].mNeighbours.push(j);
        }
    }
}

glm::vec3 FishManager::getCohesionForce(glm::vec3 POS_SUM, glm::vec3 cur_pos, int n_neighbours) {
    // Rule 1 Cohesion: tries to move towards perceived centre of the flock (avg pos of all other fish within range)
    //    perceived centre = sum neighbour fish centres / num neighbours
    //    self->perceived centre = perceived centre - self (vectors)
    glm::vec3 cohesion_force = glm::vec3{0, 0, 0};
    glm::vec3 perceived_centre = ScalarDiv(POS_SUM, (float) n_neighbours);
    cohesion_force = ScalarDiv(glm::normalize(perceived_centre - cur_pos), BASE_COHESION * (1.0f / SCALE_COHESION));
    return cohesion_force;
}
glm::vec3 FishManager::getAlignmentForce(glm::vec3 VEL_SUM, glm::vec3 cur_vel, int n_neighbours) {
    // Rule 2 Alignment: tries to match velocity with near fish
    //    avg vel = sum neighbour fish velocities / num neighbours
    glm::vec3 alignment_force = glm::vec3{0, 0, 0};
    glm::vec3 avg_vel = ScalarDiv(VEL_SUM, (float) n_neighbours);
    alignment_force = ScalarDiv(glm::normalize(avg_vel-cur_vel), BASE_ALIGNMENT * (1.0f / SCALE_ALIGNMENT));
    return alignment_force;
}
glm::vec3 FishManager::getSeparationForce(glm::vec3 cur_pos, glm::vec3 player_pos) {
    // Rule 3 Separation: tries to keep small distance away from other fish
    glm::vec3 separation_force = glm::vec3{0, 0, 0};
    glm::vec3 displacement;
    for (int i = 0; i < mFish.size(); i++)
    {
        displacement = mFish[i].mPos - cur_pos;
        // if distance (magnitude of displacement) is less than X units:
        // move away from that fish as far as you already are
        if (glm::length(displacement) < BASE_SEPARATION * SCALE_SEPARATION)
        {
            separation_force -= (displacement);
        }
    }
    // Rule 3b: avoid player if too close, weighted more
    displacement = player_pos - cur_pos;
    if (glm::length(displacement) < PLAYER_AVOID_DIST) {
        separation_force -= displacement * PLAYER_AVOID_WEIGHT;
    }
    return separation_force;
}

void FishManager::boundFishPos(int ix)
{
    // redirect fish if going too far away
    Fish * cur_fish = &mFish[ix];
    if (cur_fish->mPos.x < -SPAWN_BOUNDS.x)
    {
        cur_fish->mVel.x = TURN_FACTOR_FLAT;
    }
    else if (cur_fish->mPos.x > SPAWN_BOUNDS.x)
    {
        cur_fish->mVel.x = -TURN_FACTOR_FLAT;
    }
    else if (cur_fish->mPos.y < -SPAWN_BOUNDS.y)
    {
        cur_fish->mVel.y = TURN_FACTOR_FLAT;
    }
    else if (cur_fish->mPos.y > SPAWN_BOUNDS.y)
    {
        cur_fish->mVel.y = -TURN_FACTOR_FLAT;
    }
    else if (cur_fish->mPos.z < -SPAWN_BOUNDS.z)
    {
        cur_fish->mVel.z = TURN_FACTOR_FLAT;
    }
    else if (cur_fish->mPos.z > SPAWN_BOUNDS.z)
    {
        cur_fish->mVel.z = -TURN_FACTOR_FLAT;
    }
}

/////////////////////////////////////////////
// helper function
glm::vec3 FishManager::ScalarDiv(glm::vec3 Dividend, float Divisor)
{
    // helper function for vector scalar division
    if (Divisor != 0)
        return Dividend / glm::vec3(Divisor, Divisor, Divisor);
    return Dividend;
}
