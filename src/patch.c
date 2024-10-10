#include <stdio.h>
#include <windows.h>

#define UNITY_DLL "UnityPlayer.dll"
#define UNITY_MAIN "UnityMain"

#define eprintln(msg, ...) fprintf(stderr, msg"\n", ##__VA_ARGS__)

typedef int (*UnityFuncPtr)(HINSTANCE, HINSTANCE, LPWSTR, int);

int WINAPI wWinMain(const HINSTANCE hInstance, const HINSTANCE hPrevInstance, const LPWSTR lpCmdLine, const int nCmdShow) {
    const HMODULE unityDll = LoadLibraryA(UNITY_DLL);
    if (!unityDll) {
        const DWORD error = GetLastError();
        
        switch (error) {
            case ERROR_MOD_NOT_FOUND:
                eprintln("Could not find '%s'.", UNITY_DLL);
                break;
            case ERROR_BAD_EXE_FORMAT:
                eprintln("'%s' is not a valid DLL.", UNITY_DLL);
                break;
            default:
                eprintln("Failed to load '%s'. Error code '%lu'.", UNITY_DLL, error);
                break;
        };

        return EXIT_FAILURE;
    }

    UnityFuncPtr UnityMain = (UnityFuncPtr)GetProcAddress(unityDll, UNITY_MAIN);
    if (!UnityMain) {
        eprintln("Could not get the process address of the '%s' function.", UNITY_MAIN);
        FreeLibrary(unityDll);
        return EXIT_FAILURE;
    }

    const int unityExit = UnityMain(hInstance, hPrevInstance,  lpCmdLine, nCmdShow);
    FreeLibrary(unityDll);

    return unityExit;
}
