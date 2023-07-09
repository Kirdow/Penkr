#include "main.hpp"

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
    return main2(argc, argv);
}

#endif

int main2(int argc, char** argv)
{
    return Penkr::Clear();
}