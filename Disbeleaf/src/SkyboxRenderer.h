#pragma once 

#include <glad/glad.h>
#include <vector>
#include <string>

#include "ResourceManager.h"
#include "Renderer.h"

class SkyboxRenderer : Renderer {
    public:
        using Renderer::SetProjection;
        // inherited subroutines:
        void Initialise(const char* vshader_path, const char* fshader_path, string shader_name, string model_path, glm::mat4 projection) override;
        void Render(LightingEngine &lighting_engine) override;
        void SetNextProperties(glm::mat4 p_view, glm::mat4 p_model);
        void Cleanup() override;
        
        SkyboxRenderer() {};

    private:
        // Shader mShader; // inherited from Renderer
        unsigned int skyboxVAO, skyboxVBO;
        unsigned int tex_id;
        glm::mat4 view, model;

        const std::vector<std::string> FACES = {
            "../assets/textures/skybox/right.png",
            "../assets/textures/skybox/left.png",
            "../assets/textures/skybox/top.png",
            "../assets/textures/skybox/bottom.png",
            "../assets/textures/skybox/front.png",
            "../assets/textures/skybox/back.png"
        };

        // positions for cube vertices (taken from LearnOpenGL.com)
        static constexpr float skyboxVertices[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
        };
};
