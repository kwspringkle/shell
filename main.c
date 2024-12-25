#include "include/shell.h"
#include "include/process.h"
#include <stdio.h>
#include <windows.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_PATH_LENGTH 1024

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
    } else {
        printf("Unknown command: %s\n", args[0]);
    }
}

int main() {
    char command[1024];

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

}