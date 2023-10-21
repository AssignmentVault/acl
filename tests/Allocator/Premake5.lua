include "../../dependencies.lua"


workspace "Allocator"
    architecture "x86_64"
    configurations { "Debug", "Release" }

    binoutput = "%{cfg.buildcfg}.%{cfg.system}.%{cfg.architecture}"

    project "Allocator"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

        targetdir("bin/" .. binoutput .. "/%{prj.name}")
        objdir("int/" .. binoutput .. "/%{prj.name}")

        files
        {
            "./**.hpp",
            "./**.cpp",
        }

        includedirs
        {
            "../../include",
            "../../%{IncludeThirdpartyDirs.gtest}"
        }

        links
        {
        }

        filter "configurations:Debug"
            symbols "on"

            links
            {
                "../../%{IncludeThirdpartyLibs.gtest_d}"
            }

        filter "configurations:Release"
            optimize "on"

            links
            {
                "../../%{IncludeThirdpartyLibs.gtest}"
            }