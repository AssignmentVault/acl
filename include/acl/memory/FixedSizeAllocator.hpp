#ifndef ACL_FIXED_SIZE_ALLOCATOR_HPP
#define ACL_FIXED_SIZE_ALLOCATOR_HPP

#include <cstdint>

namespace acl
{
    class FixedSizeAllocator
    {
    public:
        virtual ~FixedSizeAllocator() = default;

        virtual void init(std::uint32_t aBlockSize) = 0;
        virtual void* alloc(std::uint32_t aBlockSize) = 0;

        virtual void destroy() = 0;
        virtual void free(void* apMemBlock) = 0;

        virtual bool containsAddress(void* aAddressPointer) = 0;

    public:
        static FixedSizeAllocator& Create();

    };
}

#endif // !ACL_FIXED_SIZE_ALLOCATOR_HPP
