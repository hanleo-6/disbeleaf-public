#pragma once
#include "ResourceManager.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include "stb_image.h"

using namespace std;

unordered_map<unsigned int, Texture2D> ResourceManager::Textures;
unordered_map<string, Shader> ResourceManager::Shaders;

// Texture
unsigned int ResourceManager::LoadTexture(const char* file, bool flip)
{
    Texture2D texture = loadTexturePriv(file, flip);
    Textures[texture.mID] = texture;
    return texture.mID;
}
unsigned int ResourceManager::LoadCubemapTexture(vector<string> faces) {
    Texture2D texture = loadCubemapPriv(faces);
    Textures[texture.mID] = texture;
    return texture.mID;
}
unsigned int ResourceManager::LoadModelTexture(aiTexture* model_texture) {
    Texture2D texture = loadModelTexturePriv(model_texture);
    Textures[texture.mID] = texture;
    return texture.mID;
}
Texture2D ResourceManager::GetTexture(unsigned int id)
{
    return Textures[id];
}

Texture2D ResourceManager::loadTexturePriv(const char* file, bool flip)
{
    // create texture object
    Texture2D texture;
    // nrChannels needed as stbi_load has to have it as parameter
    int width, height, nrChannels;
    if (flip) {
        stbi_set_flip_vertically_on_load(true);
    }
    else {
        stbi_set_flip_vertically_on_load(false);
    }
    texture.mPath = file;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    if (data) {
        // generate texture from image
        texture.Generate(width, height, data);
    }
    else {
        cout << "Unable to load texure" << "\n";  
    }
    
    // clear image data
    stbi_image_free(data);
    return texture;
}
Texture2D ResourceManager::loadModelTexturePriv(aiTexture* model_texture) {
    Texture2D texture;
    int width, height, nrChannels;
    unsigned char* data = stbi_load_from_memory((unsigned char*) model_texture->pcData, model_texture->mWidth, &width, &height, &nrChannels, STBI_rgb);
    if (data) {
        texture.Generate(width, height, data);
    }
    else {
        cout << "Unable to load model embedded texure" << "\n";  
    }
    stbi_image_free(data);

    return texture;
}
Texture2D ResourceManager::loadCubemapPriv(vector<string> faces) {
    // create texture object
    Texture2D texture;
    int width, height, nrChannels;
    // flip by default as all skybox textures need to be flipped
    stbi_set_flip_vertically_on_load(false);
    for (int i = 0; i < 6; i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            // generate cubemap texture, which adds it to the texture object
            texture.GenCubemapFace(width, height, data, i);
        }
        else {
            cout << "Unable to load cubemap texure, index: " << i << "\n";  
        }
        stbi_image_free(data);
    }

    return texture;
}

// Shader
Shader ResourceManager::LoadShader(const char* vertex_path, const char* fragment_path, string name)
{
    Shaders[name] = loadShaderPriv(vertex_path, fragment_path);
    return Shaders[name];
}
Shader ResourceManager::GetShader(string name)
{
    return Shaders[name];
}
Shader ResourceManager::loadShaderPriv(const char* vertex_path, const char* fragment_path)
{
    Shader shader;
    // create input streams from file
    ifstream vertex_file_in(vertex_path);
    ifstream fragment_file_in(fragment_path);
    if (!vertex_file_in || !fragment_file_in) {
        cout << "Error reading shader file" << "\n";
    }
    // and read into stringstream buffers
    stringstream ss_vertex_buffer, ss_fragment_buffer;
    ss_vertex_buffer << vertex_file_in.rdbuf();
    ss_fragment_buffer << fragment_file_in.rdbuf();
    vertex_file_in.close();
    fragment_file_in.close();
    // convert into c_str and compile shader
    string vertex_str = ss_vertex_buffer.str();
    string fragment_str = ss_fragment_buffer.str();
    const char* vertex_source = vertex_str.c_str();
    const char* fragment_source = fragment_str.c_str();
    shader.Compile(vertex_source, fragment_source);
    return shader;
}

// Cleanup
void ResourceManager::Clear() {
    // clear shader + texture data on application termination
    for (auto it : Shaders)
        glDeleteProgram(it.second.mID);
    for (auto it : Textures)
        glDeleteTextures(1, &it.second.mID);
}
