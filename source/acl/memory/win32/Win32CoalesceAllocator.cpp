#include "acl/memory/win32/Win32CoalesceAllocator.hpp"

#include <cassert>

namespace acl
{
    Win32CoalesceAllocator::Win32CoalesceAllocator()
        : mMemPageHead(nullptr)
    {
    }

    Win32CoalesceAllocator::~Win32CoalesceAllocator()
    {
        if (mMemPageHead != nullptr) destroy();
    }

    void Win32CoalesceAllocator::init()
    {
        if (mMemPageHead != nullptr) return;
        mMemPageHead = CreateMemPage();
    }

    void* Win32CoalesceAllocator::alloc(std::uint32_t aBlockSize)
    {
        assert(mMemPageHead != nullptr);

        if (aBlockSize > MAX_PAGE_SIZE)
            return nullptr;

        MemoryPage* page = mMemPageHead;

        while (true) {

            MemoryChunk* fb = FindFreeChunk(page, sizeof(MemoryChunk) + aBlockSize + sizeof(MemoryEndBlock));

            if (fb != nullptr) {
                ValidateChunk(fb, true);

                if (fb->size >= aBlockSize + 2 * sizeof(MemoryChunk) + 2 * sizeof(MemoryEndBlock)) {
                    auto* nfb = (MemoryChunk*)((BYTE*)fb + sizeof(MemoryChunk) + aBlockSize + sizeof(MemoryEndBlock));
                    SetupMemChunk(nfb, fb->size - aBlockSize - sizeof(MemoryChunk) - sizeof(MemoryEndBlock), fb->next, fb->prev, true);
                    fb->size -= nfb->size;
                    if (page->head == fb) page->head = nfb;
                }
                else {
                    if (fb->next) fb->next->prev = fb->prev;
                    if (fb->prev) fb->prev->next = fb->next;
                    else page->head = fb->next;
                }

                SetupMemChunk(fb, fb->size, nullptr, nullptr, false);
                return (BYTE*)fb + sizeof(MemoryChunk);
            }

            if (page->next == nullptr)
                break;

            page = page->next;
        }

        page->next = CreateMemPage();
        page = page->next;

        std::uint32_t blockSize = page->head->size;
        if (page->head->size >= aBlockSize + 2 * sizeof(MemoryChunk) + 2 * sizeof(MemoryEndBlock)) {
            auto* nfb = (MemoryChunk*)((BYTE*)page->head + sizeof(MemoryChunk) + aBlockSize + sizeof(MemoryEndBlock));
            SetupMemChunk(nfb, page->head->size - sizeof(MemoryChunk) - aBlockSize - sizeof(MemoryEndBlock), nullptr, nullptr, true);
            blockSize -= nfb->size;
            page->head = nfb;
        }
        else {
            page->head = nullptr;
        }

        SetupMemChunk(((MemoryChunk*)((BYTE*)page + sizeof(MemoryPage))), blockSize, nullptr, nullptr, false);
        return (BYTE*)page + sizeof(MemoryPage) + sizeof(MemoryChunk);
    }

    void Win32CoalesceAllocator::destroy()
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

    void Win32CoalesceAllocator::free(void* apMemBlock)
    {
        assert(mMemPageHead != nullptr);

        MemoryPage* page = mMemPageHead;
        while (page != nullptr) {

            if (IsInsidePage(page, apMemBlock)) {
                ValidateChunk((MemoryChunk*)((BYTE*)apMemBlock - sizeof(MemoryChunk)), false);
                break;
            }
            page = page->next;
        }

        if (page == nullptr)
            return;

        auto* cb = (MemoryChunk*)((BYTE*)apMemBlock - sizeof(MemoryChunk));
        size_t lbs = ((MemoryEndBlock*)((BYTE*)cb - sizeof(MemoryEndBlock)))->size;
        auto* lb = (MemoryChunk*)((BYTE*)cb - lbs);
        auto* rb = (MemoryChunk*)((BYTE*)cb + cb->size);

        if (!IsInsidePage(page, (BYTE*)lb + sizeof(MemoryChunk)) || lb->alloc)
            lb = nullptr;
        if (!IsInsidePage(page, (BYTE*)rb + sizeof(MemoryChunk)) || rb->alloc)
            rb = nullptr;

        if (lb) ValidateChunk(lb, true);
        if (rb) ValidateChunk(rb, true);

        if (lb != nullptr) {
            lb->size += cb->size;
            cb = lb;
        }

        if (rb != nullptr) {
            cb->size += rb->size;

            if (rb->next) rb->next->prev = rb->prev;
            if (rb->prev) rb->prev->next = rb->next;
            else page->head = rb->next;
        }

        // if no joins or only right join, update head
        if ((lb == nullptr && rb == nullptr) || (lb == nullptr)) {
            if (page->head)
                page->head->prev = cb;

            cb->next = page->head;
            cb->prev = nullptr;
            page->head = cb;
        }

        SetupMemChunk(cb, cb->size, cb->next, cb->prev, true);
    }

    bool Win32CoalesceAllocator::containsAddress(void* aAddressPointer)
    {
        MemoryPage* page = mMemPageHead;
        while (page) {
            if (IsInsidePage(page, aAddressPointer))
                return true;

            page = page->next;
        }

        return false;
    }

    Win32CoalesceAllocator::MemoryPage* Win32CoalesceAllocator::CreateMemPage()
    {
        MemoryPage* memPage = static_cast<MemoryPage*>(VirtualAlloc(nullptr, sizeof(MemoryPage) + sizeof(MemoryChunk) + MAX_PAGE_SIZE + sizeof(MemoryEndBlock),
            MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE));

        if (memPage == nullptr) {
            return nullptr;
        }

        memPage->next = nullptr;
        memPage->head = (MemoryChunk*)((BYTE*)memPage + sizeof(MemoryPage));
        SetupMemChunk(memPage->head, sizeof(MemoryChunk) + MAX_PAGE_SIZE + sizeof(MemoryEndBlock), nullptr, nullptr, true);

        return memPage;
    }

    void Win32CoalesceAllocator::SetupMemChunk(MemoryChunk* aMemChunk, std::uint32_t aSize, MemoryChunk* aNextChunk, MemoryChunk* aPrevChunk, bool aFree)
    {
        aMemChunk->alloc = aFree ? 0 : 1;
        aMemChunk->size = aSize;
        aMemChunk->next = aNextChunk;
        aMemChunk->prev = aPrevChunk;

        if (aMemChunk->next) aMemChunk->next->prev = aMemChunk;
        if (aMemChunk->prev) aMemChunk->prev->next = aMemChunk;

        auto* end = (MemoryEndBlock*)((BYTE*)aMemChunk + aSize - sizeof(MemoryEndBlock));
        end->size = aSize;
    }

    Win32CoalesceAllocator::MemoryChunk* Win32CoalesceAllocator::FindFreeChunk(const MemoryPage* aMemoryPage, std::uint32_t aChunkSize)
    {
        MemoryChunk* chunk = aMemoryPage->head;

        while (chunk != nullptr) {
            if (chunk->size >= aChunkSize)
                return chunk;

            chunk = chunk->next;
        }

        return nullptr;
    }

    void Win32CoalesceAllocator::ValidateChunk(MemoryChunk* aMemoryChunk, bool aFree)
    {
        auto* blockEnd = (MemoryEndBlock*)((BYTE*)aMemoryChunk + aMemoryChunk->size - sizeof(MemoryEndBlock));

        if (aFree) {
            assert(aMemoryChunk->alloc == 0);
        }
        else {
            assert(aMemoryChunk->alloc);
        }
    }

    bool Win32CoalesceAllocator::IsInsidePage(MemoryPage* aMemPage, void* aPointer)
    {
        return ((BYTE*)aPointer >= (BYTE*)aMemPage + sizeof(MemoryPage) + sizeof(MemoryChunk) &&
            (BYTE*)aPointer <= (BYTE*)aMemPage + sizeof(MemoryPage) + MAX_PAGE_SIZE + sizeof(MemoryChunk));
    }
}