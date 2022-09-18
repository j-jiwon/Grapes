#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "common.h"

class Buffer {
public:
    static std::unique_ptr<Buffer> CreateWithData(uint32_t _bufferType, uint32_t _usage,
        const void* _data, size_t stride, size_t count);
    ~Buffer();
    uint32_t Get() const { return buffer; }
    size_t GetStride() const { return stride; }
    size_t GetCount() const { return count; }
    void Bind() const;

private:
    Buffer() {}
    bool Init(uint32_t _bufferType, uint32_t _usage,
                const void* _data, size_t _stride, size_t _count);
    uint32_t buffer {0};
    uint32_t bufferType {0};
    uint32_t usage {0};
    size_t stride {0};
    size_t count {0}; 
};

#endif
