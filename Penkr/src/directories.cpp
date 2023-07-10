#include "pch.hpp"
#include "directories.hpp"

namespace Penkr
{
    // Normally browsers would be in a folder of
    // /Brand/Browser/, but Chromium only has /Browser/,
    // so we use a variadic function `Dir` to create
    // a nested folder structure for each browser
    template<typename T>
    static std::filesystem::path Dir(const T& path)
    {
        return path;
    }

    template<typename T, typename...Args>
    static std::filesystem::path Dir(const T& path, Args&&...args)
    {
        return path / Dir(std::forward<Args>(args)...);
    }

    // This is the full definition of supported browsers.
    std::vector<std::filesystem::path> Directories = {
        // Google
        Dir("Google", "Chrome"),
        Dir("Google", "Chrome Beta"),
        Dir("Google", "Chrome SxS"),
        Dir("Chromium"),
        // Opera
        Dir("Opera Software", "Opera Stable"),
        Dir("Opera Software", "Opera GX Stable"),
        // Other
        Dir("BraveSoftware", "Brave-Browser"),
        Dir("Microsoft", "Edge"),
    };
}