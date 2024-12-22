#include "include/shell.h"
#include "include/process.h"
#include <stdio.h>
#include <windows.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_PATH_LENGTH 1024

int main() {
    // Khởi tạo danh sách tiến trình
    ProcessList processList;
    initProcessList(&processList);

    // Đặt CtrlHandler để bắt tín hiệu Ctrl+C
    if (!SetConsoleCtrlHandler(CtrlHandler, TRUE)) {
        printf("Error setting control handler\n");
        return 1;
    }

    printf("Welcome to myShell! Type 'exit' to quit.\n");

    char command[MAX_COMMAND_LENGTH];
    while (1) {
        printf("myShell> ");
        fgets(command, MAX_COMMAND_LENGTH, stdin);

        // Xóa newline ở cuối chuỗi
        command[strcspn(command, "\n")] = 0;

        // Kiểm tra lệnh "exit"
        if (strcmp(command, "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }
        //Lệnh tạo tiến trình
        if(strcmp(command, "create process")==0){
            
        }

        // Kiểm tra lệnh "list"
        if (strcmp(command, "list") == 0) {
            listProcess(&processList);
            continue;
        }

        // Kiểm tra lệnh "create <process_name>"
        if (strncmp(command, "create ", 7) == 0) {
            executeCreateCommand(command + 7, &processList);  // Tạo tiến trình từ tên lệnh
            continue;
        }

        printf("Unknown command: %s\n", command);  // Lệnh không hợp lệ
    }

    // Giải phóng bộ nhớ
    free(processList.processes);
    return 0;
}
