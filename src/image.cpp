#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

std::unique_ptr<Image> Image::Load(const std::string& filepath) {
    auto image = std::unique_ptr<Image>(new Image());
    if(!image->LoadWithStb(filepath))
        return nullptr;
    return std::move(image);
}

std::unique_ptr<Image> Image::Create(int width, int height, int channelCount) {
    auto image = std::unique_ptr<Image>(new Image());
    if (!image->Allocate(width, height, channelCount))
        return nullptr;
    return std::move(image);
}

bool Image::Allocate(int _width, int _height, int _channleCount) {
    width = _width;
    height = _height;
    channelCount = _channleCount;
    data = (uint8_t*)malloc(width * height * channelCount);
    return data ? true : false;
}

void Image::SetCheckerBoardImage(int gridX, int gridY) {
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            int pos = (j * width + i) * channelCount;
            bool even = ((i / gridX) + (j / gridY)) % 2 == 0;
            uint8_t value = even ? 255 : 0;
            for (int k = 0; k < channelCount; ++k)
                data[pos + k] = value;
            if (channelCount > 3)
                data[3] = 255;
        }
    }
}

Image::~Image() {
    if (data) {
        stbi_image_free(data);
    }
}

bool Image::LoadWithStb(const std::string& filepath) {
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load(filepath.c_str(), &width, &height, &channelCount, 0);
    if (!data) {
        SPDLOG_ERROR("failed to load image: {}", filepath);
        return false;
    }
    return true;
}

std::unique_ptr<Image>  Image::CreateSingleColorImage(
    int width, int height, const glm::vec4& color) {
    glm::vec4 clamped = glm::clamp(color * 255.0f, 0.0f, 255.0f);
    uint8_t rgba[4] = {
        (uint8_t)clamped.r, 
        (uint8_t)clamped.g, 
        (uint8_t)clamped.b, 
        (uint8_t)clamped.a, 
    };
    auto image = Create(width, height, 4);
    for (int i = 0; i < width * height; i++) {
        memcpy(image->data + 4 * i, rgba, 4);
    }
    return std::move(image);
}
