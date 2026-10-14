#pragma once

#include <glad/glad.h>
#include <string>

class Texture2D
{
    public:
        unsigned int mID;
        int mIndex;
        std::string mPath;

        Texture2D();
        void Generate(int width, int height, unsigned char* data);
        void GenCubemapFace(int width, int height, unsigned char* data, int ix);
        void Bind() const;
        void BindCubemap() const;
};