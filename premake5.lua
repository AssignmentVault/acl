include "dependencies.lua"

workspace "acl"
    architecture "x86_64"
    configurations { "Debug", "Release" }

    binoutput = "%{cfg.buildcfg}.%{cfg.system}.%{cfg.architecture}"

    project "acl"
        kind "ConsoleApp"
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

        links
        {
        }

        filter "configurations:Debug"
            symbols "on"

        filter "configurations:Release"
            optimize "on"
