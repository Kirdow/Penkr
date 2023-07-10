#include "main.hpp"
#include <filesystem>

#ifdef KTN_PLATFORM_WINDOWS
    #include <Windows.h>
#else
    #error "Platform not supported!"
#endif

namespace Penkr
{
    // Play sound when CSV file is deleted.
    static void PlayOkay()
    {
#ifdef KTN_PLATFORM_WINDOWS
        PlaySound(L"C:/Windows/Media/recycle.wav", 0, SND_ALIAS);
#else
        #error "Not implemented!"
#endif
    }

    // Play sound when CSV file could not be deleted.
    static void PlayError()
    {
#ifdef KTN_PLATFORM_WINDOWS
        PlaySound(L"C:/Windows/Media/Windows Hardware Fail.wav", 0, SND_ALIAS);
#else
        #error "Not implemented!"
#endif
    }

    // Get the path to the CSV file.
    // This needs to be the same as in Penkr.
    static const char* GetPassFile()
    {
        return "pass.csv";
    }

    int Clear()
    {
        // Get the path to the CSV file.
        std::filesystem::path passFile = GetPassFile();

        // Make sure the file exists.
        if (std::filesystem::exists(passFile))
        {
            // If it does, try to delete it.
            if (!std::filesystem::remove(passFile))
            {
                // If it couldn't delete it, something is probably using it,
                // so we play an error sound.
                PlayError();
                return 1;
            }
        }
        
        // If we could delete it or if it didn't exist,
        // play a recycle sound.
        PlayOkay();
        return 0;
    }
}