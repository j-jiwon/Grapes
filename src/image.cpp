#include "image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

std::unique_ptr<Image> Image::Load(const std::string& filepath) {
    auto image = std::unique_ptr<Image>(new Image());
    if(!image->LoadWithStb(filepath))
        return nullptr;
    return std::move(image);
}

Image::~Image() {
    if (data) {
        stbi_image_free(data);
    }
}

bool Image::LoadWithStb(const std::string& filepath) {
    data = stbi_load(filepath.c_str(), &width, &height, &channelCount, 0);
    if (!data) {
        SPDLOG_ERROR("failed to load image: {}", filepath);
        return false;
    }
    return true;
}
