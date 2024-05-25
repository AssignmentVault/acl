#include "acl/memory/win32/Win32MemoryManager.hpp"

#include <cassert>

namespace acl
{
    void Win32MemoryManager::init()
    {
        mFSA16.init(_16BYTES);
        mFSA32.init(_32BYTES);
        mFSA64.init(_64BYTES);
        mFSA128.init(_128BYTES);
        mFSA256.init(_256BYTES);
        mFSA512.init(_512BYTES);
        mCoalesceAllocator.init();
    }

    void* Win32MemoryManager::alloc(std::uint32_t aBlockSize)
    {
        if (aBlockSize > 0 && aBlockSize <= _512BYTES)
        {
            std::uint32_t value = aBlockSize;
            if (value < 9) value = 9;

            value--;
            value |= value >> 1;
            value |= value >> 2;
            value |= value >> 4;
            value |= value >> 8;
            value |= value >> 16;
            value++;

            switch (value)
            {
                case 16:
                    return mFSA16.alloc(_16BYTES);
                    break;
                case 32:
                    return mFSA32.alloc(_32BYTES);
                    break;
                case 64:
                    return mFSA64.alloc(_64BYTES);
                    break;
                case 128:
                    return mFSA128.alloc(_128BYTES);
                    break;
                case 256:
                    return mFSA256.alloc(_256BYTES);
                    break;
                case 512:
                    return mFSA512.alloc(_512BYTES);
                    break;
            }
        }
        else if (aBlockSize <= Win32CoalesceAllocator::MAX_PAGE_SIZE) {
            return mCoalesceAllocator.alloc(aBlockSize);
        }
        else {
            auto* memPage = (VirtualMemoryPage*)VirtualAlloc(nullptr, aBlockSize + sizeof(VirtualMemoryPage), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            memPage->next = mVirtualMemoryPageHead;

            if (memPage->next) memPage->next->prev = memPage;
            memPage->prev = nullptr;

            mVirtualMemoryPageHead = memPage;
            return (BYTE*)memPage + sizeof(VirtualMemoryPage);
        }

        return nullptr;
    }

    void Win32MemoryManager::destroy()
    {
        mFSA16.destroy();
        mFSA32.destroy();
        mFSA64.destroy();
        mFSA128.destroy();
        mFSA256.destroy();
        mFSA512.destroy();
        mCoalesceAllocator.destroy();

        assert(mVirtualMemoryPageHead == nullptr);
    }

    void Win32MemoryManager::free(void* apMemBlock)
    {
        if (mFSA16.containsAddress(apMemBlock))         { mFSA16.free(apMemBlock); return; }
        else if (mFSA32.containsAddress(apMemBlock))    { mFSA32.free(apMemBlock); return; }
        else if (mFSA64.containsAddress(apMemBlock))    { mFSA64.free(apMemBlock); return; }
        else if (mFSA128.containsAddress(apMemBlock))   { mFSA128.free(apMemBlock); return; }
        else if (mFSA256.containsAddress(apMemBlock))   { mFSA256.free(apMemBlock); return; }
        else if (mFSA512.containsAddress(apMemBlock))   { mFSA512.free(apMemBlock); return; }

        if (mCoalesceAllocator.containsAddress(apMemBlock)) { mCoalesceAllocator.free(apMemBlock); return; }

        VirtualMemoryPage* page = mVirtualMemoryPageHead;
        while (page) {
            if ((BYTE*)+sizeof(VirtualMemoryPage) == (BYTE*)apMemBlock) {
                if (page->next) page->next->prev = page->prev;
                if (page->prev) page->prev->next = page->next;

                else mVirtualMemoryPageHead = page->next;
                VirtualFree(page, 0, MEM_RELEASE);

                return;
            }

            page = page->next;
        }

        assert(false);
    }

}