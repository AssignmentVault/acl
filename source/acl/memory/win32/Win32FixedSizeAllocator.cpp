#include "acl/memory/win32/Win32FixedSizeAllocator.hpp"

#include <cassert>

namespace acl
{
    Win32FixedSizeAllocator::Win32FixedSizeAllocator()
        : mBlockSize(-1), mMemPageHead(nullptr)
    {
    }

    Win32FixedSizeAllocator::~Win32FixedSizeAllocator()
    {
        if (mMemPageHead != nullptr) destroy();
    }

    void Win32FixedSizeAllocator::init(std::uint32_t aBlockSize)
    {
        if (mMemPageHead != nullptr) return;

        mBlockSize = aBlockSize;
        mMemPageHead = CreateMemPage();
    }

    void* Win32FixedSizeAllocator::alloc(std::uint32_t aBlockSize)
    {
        assert(mMemPageHead != nullptr);

        if (aBlockSize > mBlockSize) {
            return nullptr;
        }

        MemoryPage* memPage = mMemPageHead;
        while (true) {
            if (memPage->numInit < DEFAULT_SIZE) {
                memPage->numInit++;

                return (BYTE*)(memPage) + sizeof(memPage) + (memPage->numInit - 1) * mBlockSize;
            }

            if (memPage->fh >= 0) {
                int fh = memPage->fh;

                memPage->fh = ((MemoryBlock*)((BYTE*)memPage + sizeof(memPage) + memPage->fh * mBlockSize))->freeIndex;
                return (BYTE*)memPage + sizeof(memPage) + fh * mBlockSize;
            }

            if (memPage->next == nullptr) {
                break;
            }

            memPage = memPage->next;
        }

        MemoryPage* newPage = CreateMemPage();

        if (newPage == nullptr) {
            return nullptr;
        }

        newPage->numInit = 1;
        memPage->next = newPage;

        return (BYTE*)newPage + sizeof(MemoryPage);
    }

    void Win32FixedSizeAllocator::destroy()
    {
        assert(mMemPageHead != nullptr);

        while (mMemPageHead) {
            MemoryPage* next = mMemPageHead->next;

            if (!VirtualFree(mMemPageHead, 0, MEM_RELEASE)) {
                return;
            }

            mMemPageHead = next;
        }
    }

    void Win32FixedSizeAllocator::free(void* apMemBlock)
    {
        assert(mMemPageHead != nullptr);

        MemoryPage* memPage = mMemPageHead;
        while (true) {
            int blockNumber = (int)((BYTE*)apMemBlock - (BYTE*)memPage - sizeof(MemoryPage)) / (int)mBlockSize;

            if (blockNumber >= 0 && blockNumber <= DEFAULT_SIZE) {
                ((MemoryBlock*)((BYTE*)memPage + sizeof(MemoryPage) + blockNumber * mBlockSize))->freeIndex = memPage->fh;
                memPage->fh = blockNumber;

                return;
            }

            if (memPage->next == nullptr) {
                break;
            }

            memPage = memPage->next;
        }
    }

    bool Win32FixedSizeAllocator::containsAddress(void* aAddressPointer)
    {
        MemoryPage* page = mMemPageHead;
        while (page) {
            if (aAddressPointer >= (BYTE*)page + sizeof(MemoryPage) && aAddressPointer < (BYTE*)page + sizeof(MemoryPage) + mBlockSize * DEFAULT_SIZE)
            {
                return true;
            }

            page = page->next;
        }

        return false;
    }

    Win32FixedSizeAllocator::MemoryPage* Win32FixedSizeAllocator::CreateMemPage() const
    {
        MemoryPage* page = static_cast<MemoryPage*>(VirtualAlloc(nullptr, sizeof(MemoryPage) + mBlockSize * DEFAULT_SIZE,
            MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));

        if (page == nullptr) {
            return nullptr;
        }

        page->next = nullptr;
        page->numInit = 0;
        page->fh = -1;

        return page;
    }
}