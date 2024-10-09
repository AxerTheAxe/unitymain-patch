#include <stdio.h>
#include <windows.h>

#define UNITY_DLL "UnityPlayer.dll"
#define UNITY_MAIN "UnityMain"

typedef int (*UnityFuncPtr)(HINSTANCE, HINSTANCE, LPWSTR, int);

int WINAPI wWinMain(const HINSTANCE hInstance, const HINSTANCE hPrevInstance, const LPWSTR lpCmdLine, const int nCmdShow) {
    const HMODULE unityDll = LoadLibraryA(UNITY_DLL);
    if (!unityDll) {
        const DWORD error = GetLastError();
        
        if (error == ERROR_MOD_NOT_FOUND) {
            fprintf(stderr, "Could not find '%s'.\n", UNITY_DLL);
        } else if (error == ERROR_BAD_EXE_FORMAT) {
            fprintf(stderr, "'%s' is not a valid DLL.\n", UNITY_DLL);
        } else {
            fprintf(stderr, "Failed to load '%s'. Error code '%lu'.\n", UNITY_DLL, error);
        }

        return EXIT_FAILURE;
    }

    UnityFuncPtr UnityMain = (UnityFuncPtr)GetProcAddress(unityDll, UNITY_MAIN);
    if (!UnityMain) {
        fprintf(stderr, "Could not get the process address of '%ls'.\n", UNITY_MAIN);
        return EXIT_FAILURE;
    }

    // Returns the exit code from the Unity application
    return UnityMain(hInstance, hPrevInstance,  lpCmdLine, nCmdShow);
}
