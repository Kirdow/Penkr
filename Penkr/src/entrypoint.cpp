#include "pch.hpp"
#include "main.hpp"

#define KTN_CLEAR_ON 0

#ifdef KTN_PLATFORM_WINDOWS
#include <Windows.h>
#endif

int main2(int argc, char** argv);

#if KTN_DIST && defined(KTN_PLATFORM_WINDOWS)

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
    return main2(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
    main2(argc, argv);
}

#endif

int main2(int argc, char** argv)
{
#if KTN_CLEAR_ON == 1
    return Penkr::Clear();
#else
    return Penkr::Main(argc, argv);
#endif
}