#ifndef ACL_MEMORY_ALLOCATOR_HPP
#define ACL_MEMORY_ALLOCATOR_HPP

#include <vector>

namespace acl
{
    template<typename _Ty>
    class MemoryAllocator final
    {
        using _Ty           = valueType;
        using _Ty           = valuePointer;
        using const _Ty*    = constValuePointer;


    private:
        std::size_t mBlockSize;
        _Ty* mBlocks;

    public:
        MemoryAllocator() constexpr {}
        MemoryAllocator(const MemoryAllocator* mAllocator) noexcept = default;
        MemoryAllocator(std::size_t aBlockCount) {
            mBlocksSize = sizeof(aBlockCount);
            mBlocks = malloc(mBlockSize)
        }

        ~MemoryAllocator();

        virtual void Init();
        virtual void Destroy();

        virtual void* Alloc(size_t aSize);
        virtual void Free(void* aPointer);

    public:
        virtual void dumpStat() const;
        virtual void dumpBlocks() const;

    };
}

#endif // !ACL_MEMORY_ALLOCATOR_HPP
