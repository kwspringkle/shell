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
    //In ra ngày
    if (strcmp(args[0], "date") == 0) {
        printDate();
    } //In ra giờ 
    else if (strcmp(args[0], "time") == 0) {
        printTime();
    } //In ra địa chỉ đang ở hiện tạitại
    else if (strcmp(args[0], "pwd") == 0) {
        printPwd();
    } //Tạo thư mục mới
    else if (strcmp(args[0], "mkdir") == 0) {
        if (args[1] != NULL) {
            makeDir(args[1]);
        } else {
            printf("Usage: mkdir <dirname>\n");
        }
    } //Xóa thư mục 
    else if (strcmp(args[0], "rmdir") == 0) {
        if (args[1] != NULL) {
            removeDir(args[1]);
        } else {
            printf("Usage: rmdir <dirpath>\n");
        }
    } //Liệt kê thư mục 
    else if (strcmp(args[0], "ls") == 0) {
        if (args[1] != NULL) {
            listDirectoryTree(args[1]);
        } else {
            printf("Usage: ls <dirpath>\n");
        }
    }  //Chuyển thư mục
    else if (strcmp(args[0], "cd") == 0) {
        if (args[1] != NULL) {
            changeDirectory(args[1]);
        } else {
            printf("Usage: cd <dirpath>\n");
        }
    } //Tạo file mới
    else if(strcmp(args[0], "create") == 0){
        if(args[1] != NULL){
            createFile(args[1]);
        } else {
            printf("Usage: create <filename> \n");
        }
    }  //Đọc nội dung trong file
    else if(strcmp(args[0], "read") == 0){
        if(args[1] != NULL){
            readFile(args[1]);
        } else{
            printf("Usage: read <filename> \n");
        }
    }   // Ghi nội dung vào file
    else if (strcmp(args[0], "write") == 0) {
        if (args[1] != NULL && args[2] != NULL) {
            char content[MAX_COMMAND_LENGTH] = "";
            for (int j = 2; args[j] != NULL; j++) {
                strcat(content, args[j]);
                strcat(content, " ");
            }
            writeFile(args[1], content);
        } else {
            printf("Usage: write <filename> <content>\n");
        }
    }   // Thêm nội dung vào file (trước đó đã có nội dung sẵn)
    else if (strcmp(args[0], "append") == 0) {
        if (args[1] != NULL && args[2] != NULL) {
            char content[MAX_COMMAND_LENGTH] = "";
            for (int j = 2; args[j] != NULL; j++) {
                strcat(content, args[j]);
                strcat(content, " ");
            }
            appendFile(args[1], content);
        } else {
            printf("Usage: append <filename> <content>\n");
        }
    } //Xóa file
    else if (strcmp(args[0], "remove") == 0) {
        if (args[1] != NULL) {
            removeFile(args[1]);
        } else {
            printf("Usage: remove <filename>\n");
        }
    }

    else {
        printf("Unknown command: %s\n", args[0]);
    }

}

int main() {
    char command[1024];
    printf("Welcome to our shell!\n");
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