#include "acl/memory/CoalesceAllocator.hpp"

#include "acl/memory/win32/Win32CoalesceAllocator.hpp"

namespace acl
{
    CoalesceAllocator& CoalesceAllocator::Create()
    {
        #ifdef ACL_WIN32
            return Win32CoalesceAllocator();
        #endif // ACL_WINDOWS
    }

}