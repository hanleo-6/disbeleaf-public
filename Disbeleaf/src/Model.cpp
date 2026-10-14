#include "Model.h"
using namespace std;

Model::Model(string const &path) {
    loadModelPriv(path);
}

void Model::Draw(Shader &shader) {
    for (auto it: mModelMeshes) {
        it.Draw(shader);
    }
}

void Model::loadModelPriv(string const &path) {
    // load model with assimp
    // assimp importer automatically cleans up memory when it goes out of scope
    Assimp::Importer model_importer;
    const aiScene* scene = model_importer.ReadFile(path, aiProcess_Triangulate |  aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // verify scene was loaded properly
    if (!scene || !scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
        cout << "Model could not be loaded by assimp" << "\n";
        return;
    }
    mDir = path.substr(0, path.find_last_of('/'));
    // start recursive processing of nodes, going down tree
    processNode(scene, scene->mRootNode);
}

void Model::processNode(const aiScene* scene, aiNode* node) {
    if (node->mNumMeshes > 0) {
        // process all meshes attached to current node
        for (int i = 0; i < node->mNumMeshes; i++) {
            aiMesh* cur_mesh = scene->mMeshes[node->mMeshes[i]];
            mModelMeshes.push_back(processMesh(scene, cur_mesh));
        }
    }
    if (node->mNumChildren > 0) {
        // recursively process each of the child nodes of current node
        for (int i = 0; i < node->mNumChildren; i++) {
            processNode(scene, node->mChildren[i]);
        }
    }
    return;
}

Mesh Model::processMesh(const aiScene* scene, aiMesh* mesh) {
    vector<Vertex> mesh_vertices;
    vector<int> mesh_indices;
    string tex_name;

    for (int i = 0; i < mesh->mNumVertices; i++) {
        Vertex cur_vertex;
        glm::vec3 temp_vec;
        // set mesh vertex positions, normals and texture coords
        if (mesh->HasPositions()) {
            temp_vec.x = mesh->mVertices[i].x;
            temp_vec.y = mesh->mVertices[i].y;
            temp_vec.z = mesh->mVertices[i].z;
            cur_vertex.pos = temp_vec;
        }
        
        if (mesh->HasNormals()) {
            temp_vec.x = mesh->mNormals[i].x;
            temp_vec.y = mesh->mNormals[i].y;
            temp_vec.z = mesh->mNormals[i].z;
            cur_vertex.normal = temp_vec;
        }
        glm::vec2 temp_tex_vec = glm::vec2(0.0f, 0.0f);
        if (mesh->HasTextureCoords(0)) { // assuming tex coords are at index 0 (only 1 set of texture)
            temp_tex_vec.x = mesh->mTextureCoords[0][i].x;
            temp_tex_vec.y = mesh->mTextureCoords[0][i].y;
        }
        cur_vertex.tex_coord = temp_tex_vec;
        // add vertex to this mesh's processed vertices
        mesh_vertices.push_back(cur_vertex);
    }
    // Get indices of all faces in mesh
    for (int i = 0; i < mesh->mNumFaces; i++) {
        aiFace cur_face = mesh->mFaces[i];
        for (int j = 0; j < cur_face.mNumIndices; j++) {
            mesh_indices.push_back(cur_face.mIndices[j]);
        }
    }
    string str_tex_name;
    unsigned int tex_id;
    if (scene->HasMaterials()) {
        aiString cur_tex_name;
        aiMaterial* cur_mat = scene->mMaterials[mesh->mMaterialIndex];
        auto tex_type = aiTextureType_DIFFUSE; // models for this game only use diffuse texture
        cur_mat->GetTexture(tex_type, 0, &cur_tex_name); // get texture's file name (including .png etc.)
        str_tex_name = cur_tex_name.C_Str();
        // cout << str_tex_name << "\n"; // for debugging, to see texture paths
        if (str_tex_name[0] == '*') { // embedded texture path starts with *
            aiTexture* model_texture = scene->mTextures[0];
            tex_id = ResourceManager::LoadModelTexture(model_texture);
        }
        else { // external texture file
            string file_path = (mDir + '/');
            file_path.append(str_tex_name);
            tex_id = ResourceManager::LoadTexture(file_path.c_str(), false);
        }
    }

    return Mesh(mesh_vertices, mesh_indices, tex_id);
}

void Model::Cleanup() {
    for (auto it: mModelMeshes) {
        it.Cleanup();
    }
}