#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "common.h"

class Buffer {
public:
    static std::unique_ptr<Buffer> CreateWithData(uint32_t _bufferType, uint32_t _usage,
        const void* _data, size_t _dataSize);
    ~Buffer();
    uint32_t Get() const { return buffer; }
    void Bind() const;

private:
    Buffer() {}
    bool Init(uint32_t _bufferType, uint32_t _usage,
                const void* _data, size_t _dataSize);
    uint32_t buffer {0};
    uint32_t bufferType {0};
    uint32_t usage {0};
};

#endif
