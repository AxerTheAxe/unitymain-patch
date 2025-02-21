#include <stdio.h>
#include <windows.h>

#define UNITY_PLAYER "UnityPlayer.dll"
#define UNITY_MAIN "UnityMain"

// Function pointer type to match the signature of UnityMain,
// the function used by Unity to initialize an application.
// The signature is as defined here:
// https://docs.unity3d.com/6000.0/Documentation/Manual/UnityasaLibrary-Windows.html
typedef int (*UnityMain)(HINSTANCE, HINSTANCE, LPWSTR, int);

static inline void logBasic(
    FILE* const outputFile, const char* const prefix, const char* const format,
    const va_list arguments
) {
    fputs(prefix, outputFile);
    vfprintf(outputFile, format, arguments);
}

static void logMessage(const char* const format, ...) {
    va_list arguments;
    va_start(arguments, format);

    logBasic(stdout, "unitymain-patch: ", format, arguments);

    va_end(arguments);
}

static void logError(const char* const format, ...) {
    va_list arguments;
    va_start(arguments, format);

    logBasic(stderr, "error: ", format, arguments);

    va_end(arguments);
}

int WINAPI
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, const int nCmdShow) {
    const HMODULE unityPlayer = LoadLibraryA(UNITY_PLAYER);
    if (!unityPlayer) {
        const DWORD dllLoadError = GetLastError();
        switch (dllLoadError) {
            case ERROR_MOD_NOT_FOUND:
                logError("could not find the Unity player DLL '%s'\n", UNITY_PLAYER);
                break;
            case ERROR_BAD_EXE_FORMAT:
                logError(
                    "could not parse the Unity player DLL '%s' as a valid DLL\n", UNITY_PLAYER
                );
                break;
            default:
                logError(
                    "failed to load the Unity player DLL '%s'. Returning with code "
                    "'%lu'\n",
                    UNITY_PLAYER, dllLoadError
                );
        }
        return dllLoadError;
    }
    logMessage("successfully loaded the Unity player DLL '%s'\n", UNITY_PLAYER);

    const UnityMain unityMain = (UnityMain)(uintptr_t)GetProcAddress(unityPlayer, "UnityMain");
    if (!unityMain) {
        const DWORD addressError = GetLastError();

        logError(
            "error: failed to retrieve the function '%s' from the Unity "
            "player DLL '%s'\n",
            UNITY_MAIN, UNITY_PLAYER
        );

        FreeLibrary(unityPlayer);
        return addressError;
    }
    logMessage(
        "unitymain-patch: successfully retrieved the function '%s' from "
        "the Unity player DLL '%s'\n",
        UNITY_MAIN, UNITY_PLAYER
    );

    const int unityMainReturn = unityMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    FreeLibrary(unityPlayer);
    return unityMainReturn;
}
