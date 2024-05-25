#include "acl/memory/FixedSizeAllocator.hpp"

#include "acl/memory/win32/Win32FixedSizeAllocator.hpp"

namespace acl
{
    FixedSizeAllocator& FixedSizeAllocator::Create()
    {
        #ifdef ACL_WIN32
            return Win32FixedSizeAllocator();
        #endif // ACL_WINDOWS
    }

}