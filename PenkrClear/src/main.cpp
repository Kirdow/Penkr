#include "main.hpp"
#include <filesystem>

#ifdef KTN_PLATFORM_WINDOWS
    #include <Windows.h>
#else
    #error "Platform not supported!"
#endif

namespace Penkr
{
    static void PlayOkay()
    {
#ifdef KTN_PLATFORM_WINDOWS
        PlaySound(L"C:/Windows/Media/recycle.wav", 0, SND_ALIAS);
#else
        #error "Not implemented!"
#endif
    }

    static void PlayError()
    {
#ifdef KTN_PLATFORM_WINDOWS
        PlaySound(L"C:/Windows/Media/Windows Hardware Fail.wav", 0, SND_ALIAS);
#else
        #error "Not implemented!"
#endif
    }

    static const char* GetPassFile()
    {
        return "pass.csv";
    }

    int Clear()
    {
        std::filesystem::path passFile = GetPassFile();
        if (std::filesystem::exists(passFile))
        {
            if (!std::filesystem::remove(passFile))
            {
                PlayError();
                return 1;
            }
        }
        
        PlayOkay();
        return 0;
    }
}