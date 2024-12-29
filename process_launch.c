#include "process_launch.h"
#include <windows.h>
#include <stdio.h> 

// Hàm ForegroundProcess: Mở tiến trình và chờ hoàn thành trước khi tiếp tục
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
    // Thực hiện mở tiến trình
    if (ShellExecuteExA(&shellInfo)) {
        WaitForSingleObject(shellInfo.hProcess, INFINITE);
        CloseHandle(shellInfo.hProcess);
        printf("File opened in foreground and process completed.\n");
    } else {
        printf("Failed to open file in foreground.\n");
    }
}

// Hàm BackgroundProcess: Mở tiến trình và tiếp tục ngay lập tức
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
    // Thực hiện mở tiến trình
    if (ShellExecuteExA(&shellInfo)) {
        printf("File opened in background.\n");
    } else {
        printf("Failed to open file in background.\n");
    }
}
