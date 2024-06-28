include "dependencies.lua"

workspace "acl"
    architecture "x64"
    configurations { "Debug", "Release" }

    binoutput = "%{cfg.buildcfg}.%{cfg.system}.%{cfg.architecture}"

    project "acl"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

        targetdir("bin/" .. binoutput .. "/%{prj.name}")
        objdir("int/" .. binoutput .. "/%{prj.name}")

        files
        {
            "./include/acl/**.hpp",
            "./source/acl/**.cpp",
        }

        includedirs
        {
            "./include"
        }

        filter "configurations:Debug"
            symbols "on"

            defines
            {
                "ACL_DEBUG"
            }

        filter "configurations:Release"
            optimize "on"

            defines
            {
                "ACL_RELEASE"
            }
        
        filter "system:Windows"

            defines
            {
                "ACL_WIN32"
            }

        filter "system:Unix"

            defines
            {
                "ACL_UNIX"
            }

        filter "system:Mac"
        
            defines
            {
                "ACL_MAC"
            }