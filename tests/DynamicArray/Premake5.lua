include "../../dependencies.lua"


workspace "DynamicArray_Test"
    architecture "x64"
    configurations { "Debug", "Release" }

    binoutput = "%{cfg.buildcfg}.%{cfg.system}.%{cfg.architecture}"

    project "DynamicArray_Test"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

        targetdir("bin/" .. binoutput .. "/%{prj.name}")
        objdir("int/" .. binoutput .. "/%{prj.name}")

        files
        {
            "./main.cpp",
        }

        includedirs
        {
            "../../include",
            "../../%{IncludeThirdpartyDirs.gtest}"
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