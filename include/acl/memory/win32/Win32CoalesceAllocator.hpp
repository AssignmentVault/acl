#ifndef ACL_WIN32_COALESCE_ALLOCATOR_HPP
#define ACL_WIN32_COALESCE_ALLOCATOR_HPP

#include <memory>
#include <Windows.h>

#include "acl/memory/CoalesceAllocator.hpp"

namespace acl
{
    class Win32CoalesceAllocator : public CoalesceAllocator
    {
        struct MemoryChunk
        {
            MemoryChunk* next;
            MemoryChunk* prev;
            std::uint32_t size;
            char alloc;
        };

        struct MemoryEndBlock
        {
            std::uint32_t size;
        };

        struct MemoryPage
        {
            MemoryPage* next;
            MemoryChunk* head;
        };

    private:
        MemoryPage* mMemPageHead;

    private:
        MemoryPage* CreateMemPage();
        void SetupMemChunk(MemoryChunk* aMemChunk, std::uint32_t aSize, MemoryChunk* aNextChunk, MemoryChunk* aPrevChunk, bool aFree);
        MemoryChunk* FindFreeChunk(const MemoryPage* aMemoryPage, std::uint32_t aChunkSize);
        void ValidateChunk(MemoryChunk* aMemoryChunk, bool aFree);
        bool IsInsidePage(MemoryPage* aMemPage, void* aPointer);

    public:
        Win32CoalesceAllocator();
        virtual ~Win32CoalesceAllocator();

        virtual void init() override;
        virtual void* alloc(std::uint32_t aBlockSize) override;

        virtual void destroy() override;
        virtual void free(void* apMemBlock) override;

        virtual bool containsAddress(void* aAddressPointer) override;

    public:
        static const std::uint32_t MAX_PAGE_SIZE = 10 * 1024 * 1024;

    };
}

#endif // !ACL_WIN32_COALESCE_ALLOCATOR_HPP
