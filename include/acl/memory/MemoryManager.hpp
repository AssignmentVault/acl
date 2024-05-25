#ifndef ACL_MEMORY_MANAGER_HPP
#define ACL_MEMORY_MANAGER_HPP

#include <cstdint>

namespace acl
{
    #define _16BYTES    0x000010
    #define _32BYTES    0x000020
    #define _64BYTES    0x000040
    #define _128BYTES   0x000080
    #define _256BYTES   0x000100
    #define _512BYTES   0x000200

    class MemoryManager
    {
    public:
        virtual ~MemoryManager() = default;

        virtual void init() = 0;
        virtual void* alloc(std::uint32_t aBlockSize) = 0;

        virtual void destroy() = 0;
        virtual void free(void* aPointer) = 0;

    public:
        static MemoryManager& Create();

    };
}

#endif // !ACL_MEMORY_MANAGER_HPP
