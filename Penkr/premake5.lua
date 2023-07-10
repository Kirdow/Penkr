project "Penkr"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "pch.hpp"
    pchsource "src/pch.cpp"

    files
    {
        "src/**.hpp",
        "src/**.cpp"
    }

    defines
    {
        "SQLITECPP_COMPILE_DLL"
    }

    includedirs
    {
        "src",
        "vendor/sqlite/include",
        "vendor/cryptopp"
    }

    links
    {
        "Crypt32.lib",
        "vendor/sqlite/lib/sqlite3.lib",
        "vendor/sqlite/lib/SQLiteCpp.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines "KTN_PLATFORM_WINDOWS"

    filter "configurations:Debug"
        defines "KTN_DEBUG"
        runtime "Debug"
        symbols "on"

        links
        {
            "vendor/cryptopp/x64/Output/Debug/cryptlib.lib"
        }

    filter "configurations:Release"
        defines "KTN_RELEASE"
        runtime "Release"
        optimize "on"

        links
        {
            "vendor/cryptopp/x64/Output/Release/cryptlib.lib"
        }

    filter "configurations:Dist"
        kind "WindowedApp"
        defines "KTN_DIST"
        runtime "Release"
        optimize "on"

        links
        {
            "vendor/cryptopp/x64/Output/Release/cryptlib.lib"
        }
