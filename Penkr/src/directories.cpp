#include "pch.hpp"
#include "directories.hpp"

namespace Penkr
{
#define CHROME_DIR(str) std::filesystem::path(str)
#define CHROME(a, b) CHROME_DIR(a) / CHROME_DIR(b)

    std::vector<std::filesystem::path> Directories = {
        CHROME("Google", "Chrome"),
        CHROME("BraveSoftware", "Brave-Browser"),
        CHROME("Microsoft", "Edge"),
        CHROME("Opera Software", "Opera Stable"),
        CHROME("Opera Software", "Opera GX Stable"),
    };
}