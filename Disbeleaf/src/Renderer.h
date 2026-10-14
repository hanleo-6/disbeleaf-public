#pragma once

#include <string>

#include "ResourceManager.h"
#include "Shader.h"
#include "Model.h"
#include "Lighting.h"

class Renderer {
    public:
        // pure virtual subroutines
        virtual void Initialise(const char* vshader_path, const char* fshader_path, string shader_name, string model_path, glm::mat4 projection) = 0;
        virtual void Render(LightingEngine &lighting_engine) = 0;
        virtual void Cleanup() = 0;
        // re-upload projection matrix (e.g. after window resize)
        void SetProjection(glm::mat4 projection) {
            mShader.Use();
            mShader.SetMat4("projection", projection);
        }
    protected:
        Shader mShader;
};

class ModelRenderer : Renderer{
    public:
        using Renderer::SetProjection;
        void Initialise(const char* vshader_path, const char* fshader_path, string shader_name, string model_path, glm::mat4 projection) override;
        void Render(LightingEngine &lighting_engine) override;
        void SetNextProperties(glm::vec3 p_pos, float p_yaw, float p_pitch, glm::vec3 p_scale, glm::mat4 p_view, glm::mat4 p_model, glm::vec3 p_cam_pos);
        void Cleanup() override;

        
    private:
        // Shader mShader; // inherited from Renderer
        Model * mModel = nullptr;
        string texture_name;
        glm::vec3 pos, scale;
        float yaw, pitch;
        glm::mat4 view, model;
        glm::vec3 cam_pos;

};