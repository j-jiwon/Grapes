#include "texture.h"

std::unique_ptr<Texture> Texture::CreateFromImage(const Image* image) {
    // read image data from texture
    auto texture = std::unique_ptr<Texture>(new Texture());
    texture->CreateTexture();
    texture->SetTextureFromImage(image);
    return std::move(texture);
}

Texture::~Texture() {
    if (textureId) {
        glDeleteTextures(1, &textureId);
    }
}

void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void Texture::SetFilter(uint32_t minFilter, uint32_t magFilter) const {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::SetWrap(uint32_t sWrap, uint32_t tWrap) const {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);
}

void Texture::CreateTexture() {
    // generate 1 texture and bind
    glGenTextures(1, &textureId);
    Bind();
    SetFilter(GL_LINEAR, GL_LINEAR);
    SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

void Texture::SetTextureFromImage(const Image* image) {
    GLenum format = GL_RGBA;
    switch(image->GetChannelCount()) {
        default: break;
        case 1: format = GL_RED; break;
        case 2: format = GL_RG; break;
        case 3: format = GL_RGB; break;        
    }
    
    // copy image data and send to GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        image->GetWidth(), image->GetHeight(), 0, 
        format, GL_UNSIGNED_BYTE,
        image->GetData());
}