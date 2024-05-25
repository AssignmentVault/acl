#ifndef ACL_SYSTEM_DEFINES_HPP
#define ACL_SYSTEM_DEFINES_HPP

#ifdef ACL_BUILD_DLL
    #ifdef _MSC_VER
        #define ACL_API __declspec(dllexport)
    #elif __GNUC__
        #define ACL_API __attribute__((visibility("default")))
    #endif
#else
    #ifdef _MSC_VER
        #define ACL_API __declspec(dllimport)
    #endif
#endif // ACL_BUILD_DLL

#endif // !ACL_SYSTEM_DEFINES_HPP
