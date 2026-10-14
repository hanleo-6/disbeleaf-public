#pragma once
#include <iostream>

#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

Texture2D::Texture2D() {
    glGenTextures(1, &mID);
    mIndex = 0;
    mPath = "";
}

// subroutines to bind texture to be active one (called just before rendering)
void Texture2D::Bind() const {
    glActiveTexture(GL_TEXTURE0 + mIndex);
    glBindTexture(GL_TEXTURE_2D, mID);
}
void Texture2D::BindCubemap() const {
    glActiveTexture(GL_TEXTURE0 + mIndex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mID);
}

// subroutines to initialise texture and texture options
void Texture2D::Generate(int width, int height, unsigned char* data) {
    // create texture
    glBindTexture(GL_TEXTURE_2D, mID);
    // set wrap and filter options, and create mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // tile texture in case of wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // for interpolation when scaling texture down
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // linearly scale texture up
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture2D::GenCubemapFace(int width, int height, unsigned char* data, int ix) { // ix represents which face
    // create texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, mID);
    // set wrap and filter options
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linearly scale texture up & down
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // snap to edge for tex coords exactly between two faces
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + ix, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // unbind texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
