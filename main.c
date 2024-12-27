#include "include/shell.h"
#include "include/process.h"
#include "process_launch.h"
#include <stdio.h>
#include <windows.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_PATH_LENGTH 1024

// Danh sách tiến trình
ProcessList processList;

// Thực thi lệnh
void executeCommand(char *command) {
    char *args[10];
    char *token;
    int i = 0;

    // Tách các đối số của lệnh
    token = strtok(command, " ");
    while (token != NULL) {
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    // Xử lý lệnh
    if (strcmp(args[0], "date") == 0) {
        printDate();
    } else if (strcmp(args[0], "time") == 0) {
        printTime();
    } else if (strcmp(args[0], "pwd") == 0) {
        printPwd();
    } else if (strcmp(args[0], "mkdir") == 0) {
        if (args[1] != NULL) {
            makeDir(args[1]);
        } else {
            printf("Please specify a directory name.\n");
        }
    } else if (strcmp(args[0], "rmdir") == 0) {
        if (args[1] != NULL) {
            removeDir(args[1]);
        } else {
            printf("Please specify a directory name.\n");
        }
    } else if (strcmp(args[0], "ls") == 0) {
        if (args[1] != NULL) {
            listDirectoryTree(args[1]);
        } else {
            printf("Please specify a directory.\n");
        }
    } else if (strcmp(args[0], "cd") == 0) {
        if (args[1] != NULL) {
            changeDirectory(args[1]);
        } else {
            printf("Please specify a path.\n");
        }
    } else if (strcmp(args[0], "add") == 0) {
        if (i > 1) {
            STARTUPINFO si = {sizeof(si)};
            PROCESS_INFORMATION pi = {0};
            if (CreateProcess(NULL, args[1], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                addProcess(&processList, pi.dwProcessId, pi.hProcess, args[1], 0);
                printf("Added process: %s (PID: %lu)\n", args[1], pi.dwProcessId);
            } else {
                printf("Failed to create process: %s\n", args[1]);
            }
        } else {
            printf("Please specify the command to run.\n");
        }
    }
    else if (strcmp(args[0], "ps") == 0) {
        listProcess(&processList);
    } else if (strcmp(args[0], "suspend") == 0) {
        if (args[1] != NULL) {
            DWORD pid = (DWORD)atoi(args[1]);
            SuspendProcessById(&processList, pid);
        } else {
            printf("Please specify a PID.\n");
        }
    } else if (strcmp(args[0], "resume") == 0) {
        if (args[1] != NULL) {
            DWORD pid = (DWORD)atoi(args[1]);
            ResumeProcessById(&processList, pid);
        } else {
            printf("Please specify a PID.\n");
        }
    } else if (strcmp(args[0], "kill") == 0) {
        if (args[1] != NULL) {
            DWORD pid = (DWORD)atoi(args[1]);
            removeProcess(&processList, pid);
            printf("Process with PID %lu removed from list.\n", pid);
        } else {
            printf("Please specify a PID.\n");
        }
    } else if (strcmp(args[0], "fg") == 0) {
        // Lệnh "fg": Chạy file hoặc ứng dụng ở chế độ Foreground (chờ hoàn thành)
    if (args[1] != NULL) {
        ForegroundProcess(args[1]);
    } else {
        printf("Please specify a file path to execute in foreground.\n");
    }
} else if (strcmp(args[0], "bg") == 0) {
    // Lệnh "bg": Chạy file hoặc ứng dụng ở chế độ Background (chạy song song)
    if (args[1] != NULL) {
        BackgroundProcess(args[1]);
    } else {
        printf("Please specify a file path to execute in background.\n");
    }
}
    else {
        printf("Unknown command: %s\n", args[0]);
    }
}

int main() {
    char command[1024];

    // Khởi tạo danh sách tiến trình
    initProcessList(&processList);

    // Lặp vô hạn để nhận lệnh từ người dùng
    while (1) {
        printf("minishell> ");
        fgets(command, sizeof(command), stdin);

        // Xóa ký tự newline ở cuối lệnh
        command[strcspn(command, "\n")] = 0;

        // Nếu lệnh là "exit", thoát khỏi minishell
        if (strcmp(command, "exit") == 0) {
            break;
        }

        // Thực thi lệnh
        executeCommand(command);
    }

    return 0;
}
