#include "process_launch.h"
#include <windows.h>
#include <stdio.h> 

void ForegroundProcess(const char* filePath) {
    SHELLEXECUTEINFOA shellInfo;
    ZeroMemory(&shellInfo, sizeof(SHELLEXECUTEINFOA));

    shellInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
    shellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    shellInfo.hwnd = NULL;
    shellInfo.lpVerb = "open";
    shellInfo.lpFile = filePath;
    shellInfo.lpParameters = NULL;
    shellInfo.lpDirectory = NULL;
    shellInfo.nShow = SW_SHOWNORMAL;

    if (ShellExecuteExA(&shellInfo)) {
        WaitForSingleObject(shellInfo.hProcess, INFINITE);
        CloseHandle(shellInfo.hProcess);
        printf("File opened in foreground and process completed.\n");
    } else {
        printf("Failed to open file in foreground.\n");
    }
}

void BackgroundProcess(const char* filePath) {
    SHELLEXECUTEINFOA shellInfo;
    ZeroMemory(&shellInfo, sizeof(SHELLEXECUTEINFOA));

    shellInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
    shellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    shellInfo.hwnd = NULL;
    shellInfo.lpVerb = "open";
    shellInfo.lpFile = filePath;
    shellInfo.lpParameters = NULL;
    shellInfo.lpDirectory = NULL;
    shellInfo.nShow = SW_SHOW;

    if (ShellExecuteExA(&shellInfo)) {
        printf("File opened in background.\n");
    } else {
        printf("Failed to open file in background.\n");
    }
}
