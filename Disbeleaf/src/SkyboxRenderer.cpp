#include "SkyboxRenderer.h"

void SkyboxRenderer::Initialise(const char* vshader_path, const char* fshader_path, string shader_name, string model_path, glm::mat4 projection) {
    mShader = ResourceManager::LoadShader(vshader_path, fshader_path, shader_name);
    // load all 6 textures & setup shader
    tex_id = ResourceManager::LoadCubemapTexture(FACES);
    mShader.Use();
    mShader.SetInt("skybox_sampler", 0);
    mShader.SetMat4("projection", projection);
    // load cube vertices into vertex buffer
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    // position vertex attribute at position 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void SkyboxRenderer::Render(LightingEngine &lighting_engine) {
    // make sure depth test passes when <= instead of <, since skybox's depth values are the maximum, so would get cutoff otherwise
    glDepthFunc(GL_LEQUAL); 
    // bind texture and setup shader
    ResourceManager::GetTexture(tex_id).BindCubemap();
    mShader.Use();
    mShader.SetMat4("view", view);
    model = glm::scale(model, glm::vec3(60000.0f));
    mShader.SetMat4("model", model);
    // get vertices and draw the triangles defined by them
    glBindVertexArray(skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default to prevent any rendering glitches with the other objects
}

void SkyboxRenderer::SetNextProperties(glm::mat4 p_view, glm::mat4 p_model) {
    view = p_view;
    model = p_model;
}

void SkyboxRenderer::Cleanup() {
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
}