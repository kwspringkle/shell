#include "include/shell.h"
#include "include/process.h"
#include "include/path.h"
#include <stdio.h>
#include <windows.h>
#include <signal.h>

#define MAX_COMMAND_LENGTH 1024
#define MAX_PATH_LENGTH 1024
ProcessList processList = {0};
EnvTable envTable = {0};

void signalHandler(int signal) {
    stopForeground(&processList, signal);
}
void runBatFile(const char * filename);

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

    /*-----------------------------------------------
        XỬ LÝ CÁC LỆNH IN RA NGÀY, GIỜ
    -----------------------------------------------*/
    //In ra ngày
    if (strcmp(args[0], "date") == 0) {
        printDate();
    } //In ra giờ 
    else if (strcmp(args[0], "time") == 0) {
        printTime();
    } 
    /*-----------------------------------------------
        XỬ LÝ CÁC LỆNH LIÊN QUAN ĐẾN THƯ MỤC
    -----------------------------------------------*/
    //In ra địa chỉ đang ở hiện tại
    else if (strcmp(args[0], "pwd") == 0) {
        printPwd();
    } 
    //Tạo thư mục mới
    else if (strcmp(args[0], "mkdir") == 0) {
        if (args[1] != NULL) {
            makeDir(args[1]);
        } else {
            printf("Usage: mkdir <dirname>\n");
        }
    } 
    //Xóa thư mục 
    else if (strcmp(args[0], "rmdir") == 0) {
        if (args[1] != NULL) {
            removeDir(args[1]);
        } else {
            printf("Usage: rmdir <dirpath>\n");
        }
    } 
    //Liệt kê thư mục 
    else if (strcmp(args[0], "ls") == 0) {
        if (args[1] != NULL) {
            listDirectoryTree(args[1]);
        } else {
            printf("Usage: ls <dirpath>\n");
        }
    }  
    //Chuyển thư mục
    else if (strcmp(args[0], "cd") == 0) {
        if (args[1] != NULL) {
            changeDirectory(args[1]);
        } else {
            printf("Usage: cd <dirpath>\n");
        }
    } 
    /*-----------------------------------------------
        XỬ LÝ CÁC LỆNH LIÊN QUAN ĐẾN FILE
    -----------------------------------------------*/
    //Tạo file mới
    else if(strcmp(args[0], "create") == 0){
        if(args[1] != NULL){
            createFile(args[1]);
        } else {
            printf("Usage: create <filename> \n");
        }
    }  
    //Đọc nội dung trong file
    else if(strcmp(args[0], "read") == 0){
        if(args[1] != NULL){
            readFile(args[1]);
        } else{
            printf("Usage: read <filename> \n");
        }
    }   
    // Ghi nội dung vào file
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
    }   
    // Thêm nội dung vào file (trước đó đã có nội dung sẵn)
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
    } 
    //Xóa file
    else if (strcmp(args[0], "remove") == 0) {
        if (args[1] != NULL) {
            removeFile(args[1]);
        } else {
            printf("Usage: remove <filename>\n");
        }
    }
    /*------------------------------------------------------
        XỬ LÝ CÁC LỆNH LIÊN QUAN ĐẾN TIẾN TRÌNH/PROCESS
    -------------------------------------------------------*/
    //Chạy tiến trình ở background
    else if (strcmp(args[0], "bg") == 0) {
        if (args[1] != NULL) {
            runBackground(&processList, args[1]);
        } else {
            printf("Usage: bg <command>\n");
        }
    }
    //Chạy tiến trình ở foreground
    else if (strcmp(args[0], "fg") == 0) {
        if (args[1] != NULL) {
            runForeground(&processList, args[1]);
        } else {
            printf("Usage: fg <command>\n");
        }
    }
    // Liệt kê các tiến trình đang chạy
    else if (strcmp(args[0], "list") == 0) {
        listProcesses(&processList);
    }
    // Dừng background process
    else if (strcmp(args[0], "stop") == 0) {
        if (args[1] != NULL) {
            DWORD pid = atoi(args[1]);
            stopProcess(&processList, pid);
        } else {
            printf("Usage: stop <pid>\n");
        }
    }
    // Tiếp tục background process
    else if (strcmp(args[0], "resume") == 0) {
        if (args[1] != NULL) {
            DWORD pid = atoi(args[1]);
            resumeProcess(&processList, pid);
        } else {
            printf("Usage: resume <pid>\n");
        }
    }
    // Kết thúc background process
    else if (strcmp(args[0], "kill") == 0) {
        if (args[1] != NULL) {
            DWORD pid = atoi(args[1]);
            killProcess(&processList, pid);
        } else {
            printf("Usage: kill <pid>\n");
        }
    }
    /*-----------------------------------------------
        XỬ LÝ CÁC LỆNH LIÊN QUAN ĐẾN BIẾN MÔI TRƯỜNG
    -----------------------------------------------*/
    // Lệnh xem các biến môi trường
    else if(strcmp(args[0], "showpath") == 0){
        showPath(&envTable);
    }
    // Thêm các biến môi trường
    else if(strcmp(args[0],"addpath")==0){
        if (args[1] != NULL) {
            addPath(&envTable, args[1]);
        } else {
            printf("Usage: addpath <path>\n");
        }
    }
    //Xóa biến môi trường
    else if(strcmp(args[0], "removepath")==0){
        if(args[1] != NULL){
            removePath(&envTable, args[1]);
        } else {
            printf("Usage: removepath <path>\n");
        }
    }
    //Update biến môi trường
    else if(strcmp(args[0], "updatepath")==0){
        if (args[1] != NULL && args[2] != NULL) {
            updatePath(&envTable, args[1], args[2]);
        } else {
            printf("Usage: updatepath <oldPath> <newPath>\n");
        }
    }
    /*-----------------------------------------------
            XỬ LÝ FILE .BAT
    -----------------------------------------------*/
    else if(strcmp(args[0], "bat") == 0){
        printf("Warning: When using a .bat file, ensure each command is written on a separate line.\n");
        if(args[1] != NULL){
            runBatFile(args[1]);
        } else{
            printf("Usage: bat <filename>\n");
        }
    }
    /*-----------------------------------------------
            HELP        
    -----------------------------------------------*/
    else if(strcmp(args[0], "help")==0){
        help();
    }
    else {
        printf("Unknown command: %s\n", args[0]);
        printf("If you need help, enter: help\n");
    }

}

void runBatFile(const char * filename){
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        printf("Couldn't open the .bat file: %s\n", filename);
        return;
    }
    char line[2048];
    while(fgets(line, sizeof(line), file) != NULL){
        line[strcspn(line, "\n")] = 0;
        if(line[0] == '\0'){
            continue;
        }
        printf("Executing: %s\n", line);
        executeCommand(line);
    }

    fclose(file);
}

int main() {
    char command[1024];
    printf("\t TINYSHELL\n");
    printf("-------------------------------------------------\n");
    printf("Welcome to our tinyshell!\n");
    printf("\t \\(^-^)/ \n");
    printf("Created by Dinh Ngoc Khanh Huyen & Tran Khanh Quynh \n");
    printf("-------------------------------------------------\n");

    signal(SIGINT, signalHandler);
    while (1) {
        printf("tinyshell> ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        // Lệnh exit
        if (strcmp(command, "exit") == 0) {
            printf("Goodbye! See you again!\n");
            printf("\t (^_^)b \n");
            break;
        }
        
        executeCommand(command);

        checkBackgroundProcesses(&processList);
        Sleep(1000); //Dừng 1s để kiểm tra trạng thái của tiến trình background
    }

}