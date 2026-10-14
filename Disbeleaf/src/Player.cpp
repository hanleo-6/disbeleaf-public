#include "Player.h"
using namespace glm; // updateVecPos uses glm namespace to be more readable

Player::Player() {
    // consistent scale, not constant as in future will want to vary scale
    mScale = vec3(0.085f, 0.085f, -0.085f);
    // mScale = vec3(0.005f, 0.005f, -0.005f); // scale for old seacreature model
    Reset();
}
void Player::Update(int algae_collected) {
    processKeyLooking();
    updateVecPos();
    tickDecel();
    incrementAlgae(algae_collected);
    MouseDown = false;
}
void Player::Reset() {
    mPos = START_POS;
    mSpeed = START_VEL;
    mFacingVec = START_FACING;
    mYaw = START_YAW; // left and right (horizontal rotation)
    mPitch = START_PITCH; // up and down (vertical rotation)
    mAlgaeCount = 0;
    MouseDown = false;
}

float Player::GetPlayerRadius() {
    return PLAYER_RADIUS;
}

void Player::AccelForward() {
    MouseDown = true;
    mSpeed += ACCEL_INCREMENT;
    if (mSpeed > SPEED_LIM) {
        mSpeed = SPEED_LIM;
    }
}
void Player::tickDecel() {
    // decelerate player to a stop if mouse is not down
    if (!MouseDown) {
        mSpeed -= DECEL_INCREMENT;
        if (mSpeed < 0.0f) {
            mSpeed = 0.0f;
        }
    }
}
void Player::processKeyLooking() {
    if (KeysDown["W"]) {
        mPitch += TURNS_SENS;
    }
    if (KeysDown["A"]) {
        mYaw -= TURNS_SENS;
    }
    if (KeysDown["S"]) {
        mPitch -= TURNS_SENS;
    }
    if (KeysDown["D"]) {
        mYaw += TURNS_SENS;
    }

    // wrap so values aren't able to go extreme in case many turns in same direction
    if (mYaw > 360.0f) {
        mYaw -= 360.0f;
    }
    else if (mYaw < -360.f) {
        mYaw += 360.0f;
    }
    if (mPitch > 360.0f) {
        mPitch -= 360.0f;
    }
    else if (mPitch < -360.f) {
        mPitch += 360.0f;
    }
}
void Player::updateVecPos() {
    // update player facing direction based on 3D trig
    vec3 newFacingVec;
    newFacingVec.x = cos(radians(mYaw)) * cos(radians(mPitch));
    newFacingVec.y = sin(radians(mPitch));
    newFacingVec.z = sin(radians(mYaw)) * cos(radians(mPitch));
    // normalise so only velocity influences change in position (want it to be a unit vector/length 1 unit)
    mFacingVec = normalize(newFacingVec);
    // update player position
    // minus sign because axis are the other way around to move 'into' screen when going forwards
    mPos -= mFacingVec * mSpeed;
}

int Player::GetAlgaeCount() {
    return mAlgaeCount;
}
void Player::incrementAlgae(int n) {
    mAlgaeCount += n;
}

