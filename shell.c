#include "include/shell.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_PATH_LENGTH 1024

//Kiểm tra xem có ấn Ctrl + c không
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
    if (fdwCtrlType == CTRL_C_EVENT) {
        printf("\nCtrl+C pressed\n");
        return TRUE;
    }
    return FALSE;
}

//Xử lý trường hợp chạy file .bat
void executeBatchFile(const char* filename, ProcessList* processList) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open batch file: %s\n", filename);
        return;
    }

    char line[MAX_COMMAND_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        if (strlen(line) > 0 && line[0] != '@' && line[0] != ':') {
            printf("Executing: %s\n", line);
            executeCommand(line, processList);
        }
    }

    fclose(file);
}

void executeCommand(const char* command, ProcessList* processList) {
    // Kiểm tra file .bat
    if (strstr(command, ".bat") != NULL) {
        executeBatchFile(command, processList);
        return;
    }

    // Kiểm tra Background process
    int isBackground = 0;
    char cmd[MAX_COMMAND_LENGTH];
    strncpy(cmd, command, MAX_COMMAND_LENGTH-1);
    if (strlen(cmd) > 0 && cmd[strlen(cmd)-1] == '&') {
        isBackground = 1;
        cmd[strlen(cmd)-1] = '\0';
    }

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (CreateProcess(NULL, cmd, NULL, NULL, FALSE,
                     CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        
        addProcess(processList, pi.dwProcessId, pi.hProcess, cmd, isBackground);
        
        if (!isBackground) {
            WaitForSingleObject(pi.hProcess, INFINITE);
            removeProcess(processList, pi.dwProcessId, pi.hProcess, cmd, isBackground);
            CloseHandle(pi.hProcess);
        }
        CloseHandle(pi.hThread);
    } else {
        printf("Error creating process\n");
    }
}
