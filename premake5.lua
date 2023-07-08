include "./vendor/premake/premake_customization/solution_items.lua"

workspace "Penkr"
    architecture "x64"
    startproject "VkApp"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    solution_items
    {
    }

    flags
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
    include "vendor/premake"
group ""

group "Core"
    include "Penkr"
group ""
