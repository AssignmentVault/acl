#ifndef ACL_WIN32_FIXED_SIZE_ALLOCATOR_HPP
#define ACL_WIN32_FIXED_SIZE_ALLOCATOR_HPP

#include <memory>
#include <Windows.h>

#include "acl/memory/FixedSizeAllocator.hpp"

namespace acl
{
    class Win32FixedSizeAllocator : public FixedSizeAllocator
    {
        const std::uint32_t DEFAULT_SIZE = 4096;

        struct MemoryPage
        {
            MemoryPage* next;
            int numInit;
            int fh;
        };

        struct MemoryBlock
        {
            int freeIndex;
        };

    private:
        MemoryPage* mMemPageHead;
        std::uint32_t mBlockSize;

    private:
        MemoryPage* CreateMemPage() const;

    public:
        Win32FixedSizeAllocator();
        virtual ~Win32FixedSizeAllocator();

        virtual void init(std::uint32_t aBlockSize) override;
        virtual void* alloc(std::uint32_t aBlockSize) override;

        virtual void destroy() override;
        virtual void free(void* apMemBlock) override;

        virtual bool containsAddress(void* aAddressPointer) override;
    };
}

#endif // !ACL_WIN32_FIXED_SIZE_ALLOCATOR_HPP
