#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<int> indices, unsigned int tex_id) {
    mVertices = vertices;
    mIndices = indices;
    mTexID = tex_id;

    setupMesh();
    // opengl context is not yet initialised so mesh can only be setup after it is
};

void Mesh::Draw(Shader &shader) {
    // bind & activate texture + shader 
    ResourceManager::GetTexture(mTexID).Bind();
    string s = to_string(mTexID);
    shader.SetInt(s.c_str(), 0);
    // bind VAO and draw vertices referenced by the indices
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mIndices.size()), GL_UNSIGNED_INT, 0);
    
    // unbind VAO + texture
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh() {
    // setup VAO, VBO, EBO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // generate buffer ids and link to VBO & EBO
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], GL_STATIC_DRAW); // copy vertex data into buffer
    // bind Element Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW); // copy index data into buffer

    // position attribute at index 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    // vertex normal attribute at index 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    // texture coord attribute at index 2
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(2);
    
    // unbind VAO
    glBindVertexArray(0);
}

void Mesh::Cleanup() {
    // free GPU memory
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // clear stored mesh data
    mVertices.clear();
    mIndices.clear();
}