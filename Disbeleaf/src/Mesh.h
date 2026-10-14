#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "Shader.h"
#include "Texture.h"
#include "ResourceManager.h"

using namespace std;

struct Vertex {
    // meshes are made of vertices
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 tex_coord;
};

class Mesh {
    // a mesh contains all the information for a single drawable thing by opengl
    public:
        vector<Vertex> mVertices; // public as needs to be freely modified by Model
        vector<int> mIndices;
        unsigned int mTexID;
        
        Mesh(vector<Vertex> vertices, vector<int> indices, unsigned int tex_id);
        void Draw(Shader &shader);
        void Cleanup();
        
    private:
        unsigned int VAO, VBO, EBO;
        void setupMesh(); // called by constructor
};