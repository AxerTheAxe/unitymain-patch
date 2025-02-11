#include <stdio.h>
#include <windows.h>

#define UNITY_PLAYER "UnityPlayer.dll"
#define UNITY_MAIN "UnityMain"

typedef int (*UnityMain)(HINSTANCE, HINSTANCE, LPWSTR, int);

static inline void logBasic(
    FILE* const outputFile, const char* const prefix, const char* const message,
    const va_list arguments
) {
    fprintf(outputFile, "%s", prefix);
    vfprintf(outputFile, message, arguments);
}

static void logMessage(const char* const message, ...) {
    va_list arguments;
    va_start(arguments, message);

    logBasic(stdout, "unitymain-patch: ", message, arguments);

    va_end(arguments);
}

static void logError(const char* const message, ...) {
    va_list arguments;
    va_start(arguments, message);

    logBasic(stderr, "error: ", message, arguments);

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
