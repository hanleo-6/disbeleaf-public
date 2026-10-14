#include "Lighting.h"

using namespace glm;

void LightingEngine::Initialise() {
    Sun.direction = vec3(-1.0f, 0.0f, 0.0f);
    Sun.position = vec3(-9000.0f, 0.0f, 0.0f);
    Sun.scale = vec3(80.0f);
    Sun.ambient = vec3(0.6f);
    Sun.diffuse = vec3(0.6f);
    Sun.specular = vec3(2.5f);
}

void LightingEngine::UpdateSun(float dt) {
    // move sun proportional amount according to delta time around its circular path
    // for (int i = 0; i < dt/0.001; i++) {
    //     vec3 sun_tangent = cross(Sun.position, vec3(0.0f, 0.0f, 1.0f));
    //     Sun.position += sun_tangent * SUN_MOVE_TICK_SCALAR;
    // }
    Sun.direction = normalize(-Sun.position);
}
