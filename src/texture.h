#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "image.h"

class Texture {
public:
    static std::unique_ptr<Texture> CreateFromImage(const Image* image);
    ~Texture();

    const uint32_t Get() const { return textureId; }
    void Bind() const;
    void SetFilter(uint32_t minFilter, uint32_t magFilter) const;
    void SetWrap(uint32_t sWrap, uint32_t tWrap) const;

private:
    Texture() {}
    void CreateTexture();
    void SetTextureFromImage(const Image* image);

    uint32_t textureId { 0 };
};

#endif __TEXTURE_H__
