#pragma once

#if API_VULKAN

#include "Graphics/GraphicsConstants.h"
#include "Graphics/Vulkan/Buffer.h"

class DestroyQueue;

class MultiBuffer
{
public:

    MultiBuffer(BufferType bufferType, size_t size, const char* debugName, const void* srcData = nullptr);

    void Update(const void* srcData, size_t srcSize, size_t dstOffset = 0);

    VkBuffer Get();
    VkBuffer Get(uint32_t frameIndex);

    Buffer* GetBuffer();
    Buffer* GetBuffer(uint32_t frameIndex);

    size_t GetSize() const;

protected:

    friend class DestroyQueue;
    virtual ~MultiBuffer();

    Buffer* mBuffers[MAX_FRAMES] = {};
};


struct UniformBlock
{
    class UniformBuffer* mUniformBuffer = nullptr;
    uint8_t* mData = nullptr;
    uint32_t mOffset = 0;
    uint32_t mSize = 0;
};

// TODO: For optimal memory usage, this should be converted to a Ring Buffer.
class UniformBuffer : public MultiBuffer
{
public:
    UniformBuffer(size_t size, const char* debugName, const void* srcData = nullptr);

    void Reset(uint32_t frameIndex);

    UniformBlock AllocBlock(uint32_t blockSize);

protected:

    int32_t mHead[MAX_FRAMES] = {};
};

#endif