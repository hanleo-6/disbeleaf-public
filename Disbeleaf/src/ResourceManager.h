#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include <map>
#include <string>

#include "Texture.h"
#include "Shader.h"

using namespace std;

// using static functions so no class object needs to be defined
class ResourceManager {
    public:
        
        // functions acting as public interface
        static unsigned int LoadTexture(const char* file, bool flip); // load normal texture image
        static unsigned int LoadModelTexture(aiTexture* model_texture); // load cubemap texture (6 textures)
        static unsigned int LoadCubemapTexture(vector<string> faces); // load embedded texture (from glb)
        static Texture2D GetTexture(unsigned int id);

        static Shader LoadShader(const char* vertex_path, const char* fragment_path, string name);
        static Shader GetShader(string name);

        static void Clear();
    private:
        // store loaded shaders + textures
        static unordered_map<unsigned int, Texture2D> Textures;
        static unordered_map<string, Shader> Shaders;

        ResourceManager() = delete; // explictly a static class

        static Texture2D loadTexturePriv(const char* file, bool flip); 
        static Texture2D loadCubemapPriv(vector<string> faces); 
        static Texture2D loadModelTexturePriv(aiTexture* model_texture);
        static Shader loadShaderPriv(const char* vertex_path, const char* fragment_path);
        int tex_count = 0;
};