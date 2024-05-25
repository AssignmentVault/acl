#include "acl/memory/MemoryManager.hpp"

#include "acl/memory/win32/Win32MemoryManager.hpp"

namespace acl
{
    MemoryManager& MemoryManager::Create()
    {
        #ifdef ACL_WIN32
            return Win32MemoryManager();
        #endif // ACL_WINDOWS
    }

}