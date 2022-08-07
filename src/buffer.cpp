#include "buffer.h"

std::unique_ptr<Buffer> Buffer::CreateWithData(uint32_t _bufferType, uint32_t _usage,
    const void* _data, size_t _dataSize){
    auto buf = std::unique_ptr<Buffer>(new Buffer());
    if (!buf->Init(_bufferType, _usage, _data, _dataSize))
        return nullptr;
    return std::move(buf);
}

Buffer::~Buffer() {
    if (buffer) {
        glDeleteBuffers(1, &buffer);
    }
}

void Buffer::Bind() const {
    glBindBuffer(bufferType, buffer);
}

bool Buffer::Init(uint32_t _bufferType, uint32_t _usage,
                const void* _data, size_t _dataSize) {
    bufferType = _bufferType;
    usage = _usage;
    glGenBuffers(1, &buffer);
    Bind();
    glBufferData(_bufferType, _dataSize, _data, _usage);
    return true;
}
