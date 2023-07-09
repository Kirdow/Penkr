project "PenkrClear"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.hpp",
        "src/**.cpp"
    }

    includedirs
    {
        "src"
    }

    links
    {
        "Winmm.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines "KTN_PLATFORM_WINDOWS"

    filter "configurations:Debug"
        defines "KTN_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "KTN_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        kind "WindowedApp"
        defines "KTN_DIST"
        runtime "Release"
        optimize "on"
