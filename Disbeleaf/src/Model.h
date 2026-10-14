#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <iostream>
#include <map>
#include <vector>

#include "Mesh.h"
#include "Shader.h"
#include "ResourceManager.h"

using namespace std;

class Model {
    public:
        string mDir;
        Model(string const &path);
        void Draw(Shader &shader);
        void Cleanup();
        
    private:
        vector<Mesh> mModelMeshes; // model meshes only used inside Model class, so there isn't even a function to access it

        void loadModelPriv(string const &path); // called by constructor
        void processNode(const aiScene* scene, aiNode* node); // recursive method to process model scene nodes
        Mesh processMesh(const aiScene* scene, aiMesh* mesh);
};

