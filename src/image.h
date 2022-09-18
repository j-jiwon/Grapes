#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "common.h"

class Image {
public:
    static std::unique_ptr<Image> Load(const std::string& filepath);
    static std::unique_ptr<Image> Create(int width, int height, int channelCount=4);
    static std::unique_ptr<Image> CreateSingleColorImage(int width, int height, const glm::vec4& color);
    ~Image();

    const uint8_t* GetData() const { return data; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetChannelCount() const { return channelCount; }
    void SetCheckerBoardImage(int gridX, int gridY);

private:
    Image() {};
    bool LoadWithStb(const std::string& filepath);
    bool Allocate(int width, int height, int channelCount);
    int width { 0 };
    int height { 0 };
    int channelCount { 0 };
    uint8_t* data { nullptr };
};

#endif 
