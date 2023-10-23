#ifndef ACL_MEMORY_ALLOCATOR_HPP
#define ACL_MEMORY_ALLOCATOR_HPP

#include <vector>

namespace acl
{
    template<typename _Ty>
    class MemoryAllocator
    {
    public:
        MemoryAllocator() = default;
        virtual ~MemoryAllocator() = default;

        virtual void Init()     = 0;
        virtual void Destroy()  = 0;

        virtual void* Alloc(size_t aSize) = 0;
        virtual void Free(void* aPointer) = 0;

    public:
        virtual void dumpStat() const   = 0;
        virtual void dumpBlocks() const = 0;

    };
}

#endif // !ACL_MEMORY_ALLOCATOR_HPP
