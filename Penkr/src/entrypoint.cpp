#include "pch.hpp"
#include "main.hpp"

#ifdef KTN_PLATFORM_WINDOWS
#include <Windows.h>
#endif

// Forward declaration for a common main function.
int main2(int argc, char** argv);

// If compiling for Dist and we're on windows, use WinMain
// as Dist on Windows is using WindowedApp as opposed to
// ConsoleApp.
#if KTN_DIST && defined(KTN_PLATFORM_WINDOWS)

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    // Redirect input to the common main function
    return main2(__argc, __argv);
}

// If it's not running on Windows or the build config isn't Dist,
// then run a normal main function as we're building a ConsoleApp.
#else

int main(int argc, char** argv)
{
    // Redirect input to the common main function.
    main2(argc, argv);
}

#endif

// The common main function also redirects the input to the Penkr code.
int main2(int argc, char** argv)
{
    return Penkr::Main(argc, argv);
}