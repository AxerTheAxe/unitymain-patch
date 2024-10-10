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
        
        if (error == ERROR_MOD_NOT_FOUND) {
            eprintln("Could not find '%s'.", UNITY_DLL);
        } else if (error == ERROR_BAD_EXE_FORMAT) {
            eprintln("'%s' is not a valid DLL.", UNITY_DLL);
        } else {
            eprintln("Failed to load '%s'. Error code '%lu'.", UNITY_DLL, error);
        }

        return EXIT_FAILURE;
    }

    UnityFuncPtr UnityMain = (UnityFuncPtr)GetProcAddress(unityDll, UNITY_MAIN);
    if (!UnityMain) {
        eprintln("Could not get the process address of '%s'.", UNITY_MAIN);
        return EXIT_FAILURE;
    }

    // Returns the exit code from the Unity application
    return UnityMain(hInstance, hPrevInstance,  lpCmdLine, nCmdShow);
}
