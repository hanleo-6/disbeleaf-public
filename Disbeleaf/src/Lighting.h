#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;


struct DirectionalLight { // only directional light is sun
    vec3 direction;
    vec3 position; // for calculating light direction depending on time of day
    vec3 scale; // for sun model size
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
};

class LightingEngine {
    public:
        DirectionalLight Sun;

        void Initialise();
        void UpdateSun(float dt);
    private:
        const float SUN_MOVE_TICK_SCALAR = 0.1f;
};