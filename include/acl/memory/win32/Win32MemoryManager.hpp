#ifndef ACL_WIN32_MEMORY_MANGER_HPP
#define ACL_WIN32_MEMORY_MANGER_HPP

#include "acl/memory/win32/Win32FixedSizeAllocator.hpp"
#include "acl/memory/win32/Win32CoalesceAllocator.hpp"

#include "acl/memory/MemoryManager.hpp"

namespace acl
{
    class Win32MemoryManager : public MemoryManager
    {
        struct VirtualMemoryPage
        {
            VirtualMemoryPage* next;
            VirtualMemoryPage* prev;
        };

    private:
        Win32FixedSizeAllocator mFSA16 = Win32FixedSizeAllocator();
        Win32FixedSizeAllocator mFSA32 = Win32FixedSizeAllocator();
        Win32FixedSizeAllocator mFSA64 = Win32FixedSizeAllocator();
        Win32FixedSizeAllocator mFSA128 = Win32FixedSizeAllocator();
        Win32FixedSizeAllocator mFSA256 = Win32FixedSizeAllocator();
        Win32FixedSizeAllocator mFSA512 = Win32FixedSizeAllocator();

        Win32CoalesceAllocator mCoalesceAllocator;
        VirtualMemoryPage* mVirtualMemoryPageHead{};

    public:
        Win32MemoryManager() = default;
        virtual ~Win32MemoryManager() = default;

        virtual void init() override;
        virtual void* alloc(std::uint32_t aBlockSize) override;

        virtual void destroy() override;
        virtual void free(void* apMemBlock) override;

    };
}

#endif // !ACL_WIN32_MEMORY_MANGER_HPP
