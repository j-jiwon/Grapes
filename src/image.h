#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "common.h"

class Image {
public:
    static std::unique_ptr<Image> Load(const std::string& filepath);
    ~Image();

    const uint8_t* GetData() const { return data; }
    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    int GetChannelCount() const { return channelCount; }

private:
    Image() {};
    bool LoadWithStb(const std::string& filepath);
    int width { 0 };
    int height { 0 };
    int channelCount { 0 };
    uint8_t* data { nullptr };
};

#endif 
