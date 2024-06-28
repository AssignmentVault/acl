include "../../dependencies.lua"


workspace "Allocator"
    architecture "x64"
    configurations { "Debug", "Release" }

    binoutput = "%{cfg.buildcfg}.%{cfg.system}.%{cfg.architecture}"

    -- group "Dependencies"
    --     include "../../premake5"
    -- group ""

    project "Allocator"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

        targetdir("bin/" .. binoutput .. "/%{prj.name}")
        objdir("int/" .. binoutput .. "/%{prj.name}")

        files
        {
            "./main.cpp"
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
                "../../%{IncludeThirdpartyLibs.gtest_d}",
                "../../bin/Debug.windows.x86_64/acl/acl.lib"
            }

        filter "configurations:Release"
            optimize "on"

            links
            {
                "../../%{IncludeThirdpartyLibs.gtest}",
                "../../bin/Release.windows.x86_64/acl/acl.lib"
            }