#include "Renderer.h"

void ModelRenderer::Initialise(const char* vshader_path, const char* fshader_path, string shader_name, string model_path, glm::mat4 projection) {
    mShader = ResourceManager::LoadShader(vshader_path, fshader_path, shader_name);
    mModel = new Model(model_path);
    mShader.Use();
    mShader.SetInt("tex_sampler", 0); // link texture pointer to shader (all textures stored at location 0)
    mShader.SetMat4("projection", projection);

    // LIGHTING
};

void ModelRenderer::Render(LightingEngine &lighting_engine) {

    // ResourceManager::GetTexture(texture_name).Bind();
    mShader.Use();
    // lighting
    mShader.SetVec3("sunlight.direction", lighting_engine.Sun.direction);
    mShader.SetVec3("sunlight.ambient", lighting_engine.Sun.ambient);
    mShader.SetVec3("sunlight.diffuse", lighting_engine.Sun.diffuse);
    mShader.SetVec3("sunlight.specular", lighting_engine.Sun.specular);
    mShader.SetVec3("sunlight.colour", glm::vec3(1.0f, 1.0f, 1.0f));
    mShader.SetVec3("view_pos", cam_pos);
    // matrices
    mShader.SetMat4("view", view);
    model = glm::translate(model, pos);
    model = glm::scale(model, scale);
    model = glm::rotate_slow(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate_slow(model, glm::radians(pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    mShader.SetMat4("model", model);

    mModel->Draw(mShader);
};

void ModelRenderer::SetNextProperties(glm::vec3 p_pos, float p_yaw, float p_pitch, glm::vec3 p_scale, glm::mat4 p_view, glm::mat4 p_model, glm::vec3 p_cam_pos) {
    pos = p_pos;
    yaw = p_yaw;
    pitch = p_pitch;
    scale = p_scale;
    view = p_view;
    model = p_model;
    cam_pos = p_cam_pos;
};

void ModelRenderer::Cleanup() {
    mModel->Cleanup();
}