#ifndef ALC_COALESCE_ALLOCATOR_HPP
#define ALC_COALESCE_ALLOCATOR_HPP

#include <cstdint>

namespace acl
{
    class CoalesceAllocator
    {
    public:
        virtual ~CoalesceAllocator() = default;

        virtual void init() = 0;
        virtual void* alloc(std::uint32_t aBlockSize) = 0;

        virtual void destroy() = 0;
        virtual void free(void* apMemBlock) = 0;

        virtual bool containsAddress(void* aAddressPointer) = 0;

    public:
        static CoalesceAllocator& Create();

    };
}

#endif // !ALC_COALESCE_ALLOCATOR_HPP
