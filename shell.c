#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <errno.h>
#include "include/shell.h"

//Hàm cho lệnh date: in ra ngày tháng năm
void printDate() {
    time_t t;
    time(&t);
    struct tm *local = localtime(&t);
    printf("%02d-%02d-%04d\n", local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);
}

//Hàm cho lệnh time: in ra thời gian
void printTime() {
    time_t t;
    time(&t);
    struct tm *local = localtime(&t);
    printf("%02d:%02d:%02d\n", local->tm_hour, local->tm_min, local->tm_sec);
}

//Hàm cho lệnh pwd
void printPwd() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
}

//Hàm cho lệnh mkdir
void makeDir(const char * folderName){
    if(folderName == NULL){
        folderName = "new_folder";
    }
    if(mkdir(folderName)==0){
        printf("Directory created!\n");
    } else{
        perror("Cannot create directory!\n");
    }
}

//Hàm cho lệnh rmdir
void removeDir(const char * folderName){
    if(folderName == NULL){
        printf("Directory doesn't exist!\n");
    } else{
        if(rmdir(folderName) == 0){
            printf("Directory deleted!\n");
        } else {
            perror("Cannot delete directory!\n");
        }
    }
}

//Hàm liệt kê các file trong folder
//Hàm cho lệnh ls
void listDirectoryTree(const char *path){
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("Cannot open directory");
        return;
    }

    struct dirent *entry;
    printf("Listing contents of directory: %s\n", path);
    while ((entry = readdir(dir)) != NULL)
    {
        // Bỏ qua các mục "." và ".."
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            printf("%s\n", entry->d_name);
        }
    }
    closedir(dir);
}

//Hàm cho lệnh cd
void changeDirectory(const char * path){
    if (chdir(path) == 0) {
        printPwd();
    } else {
        perror("Cannot change directory!");
    }
}

//Hàm tạo file mới
void createFile(const char * filename){
    if(filename == NULL){
        printf("Filename cannot be empty!\n");
        return;
    }

    FILE * file = fopen(filename, "w");
    if (file != NULL) {
        printf("File created successfully: %s\n", filename);
        fclose(file);
    } else {
        perror("Cannot create file");
    }
}

// Hàm đọc nội dung trong file
void readFile(const char *filename) {
    if (filename == NULL) {
        printf("Filename cannot be empty!\n");
        return;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Cannot open file");
        return;
    }

    char buffer[1024];
    printf("Contents of %s:\n", filename);
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);
    printf("\n");
}
// Hàm chuyển đổi chuỗi có escape sequence
char* processEscapeSequences(const char* input) {
    if (input == NULL) return NULL;
    
    int len = strlen(input);
    char* output = (char*)malloc(len + 1);  
    int j = 0;
    
    for (int i = 0; i < len; i++) {
        if (input[i] == '\\' && i + 1 < len) {
            switch (input[i + 1]) {
                case 'n':
                    output[j++] = '\n';
                    i++;  
                    break;
                case 't':
                    output[j++] = '\t';
                    i++;
                    break;
                case '\\':
                    output[j++] = '\\';
                    i++;
                    break;
                default:
                    output[j++] = input[i];
            }
        } else {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
    return output;
}
//Hàm ghi nội dung vào file
void writeFile(const char *filename, const char *content) {
    if (filename == NULL || content == NULL) {
        printf("Filename and content cannot be empty!\n");
        return;
    }

    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Cannot open file for writing");
        return;
    }

    char* processed_content = processEscapeSequences(content);
    if (processed_content != NULL) {
        if (fputs(processed_content, file) != EOF) {
            printf("Content written to file successfully\n");
        } else {
            perror("Error writing to file");
        }
        free(processed_content);
    }

    fclose(file);
}
// Hàm ghi thêm nội dung vào file
void appendFile(const char *filename, const char *content) {
    if (filename == NULL || content == NULL) {
        printf("Filename and content cannot be empty!\n");
        return;
    }

    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Cannot open file for appending");
        return;
    }

    char* processed_content = processEscapeSequences(content);
    if (processed_content != NULL) {
        if (fputs(processed_content, file) != EOF) {
            printf("Content appended to file successfully\n");
        } else {
            perror("Error appending to file");
        }
        free(processed_content);
    }

    fclose(file);
}

// Hàm xóa file
void removeFile(const char *filename) {
    if (filename == NULL) {
        printf("Filename cannot be empty!\n");
        return;
    }

    if (remove(filename) == 0) {
        printf("File deleted successfully: %s\n", filename);
    } else {
        perror("Cannot delete file");
    }
}

void help() {
    printf("\n\tHOW TO USE TINYSHELL?\n");
    printf("\t\t (._.)? \n");
    printf("Tinyshell provide 24 built-in commands\n");
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("\n");
    printf("\n\t----- BASIC COMMANDS -----\n");
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("| %-3s | %-10s | %-50s | %-30s |\n", "No.", "Command", "Description", "Usage");
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 1, "date", "Display current date", "date");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 2, "time", "Display current time", "time");
    printf("--------------------------------------------------------------------------------------------------\n");

    printf("\t----- DIRECTORY COMMANDS -----\n");
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 3, "pwd", "Print current directory", "pwd");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 4, "mkdir", "Create a new directory", "mkdir <dirname>");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 5, "rmdir", "Remove a directory", "rmdir <dirpath>");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 6, "ls", "List directory contents", "ls <dirpath>");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 7, "cd", "Change directory", "cd <dirpath>");
    printf("--------------------------------------------------------------------------------------------------\n");

    printf("\t----- FILE COMMANDS -----\n");
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 8, "create", "Create a new file", "create <filename>");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 9, "read", "Read a file's content", "read <filename>");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 10, "write", "Write to a file", "write <filename> <content>");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 11, "append", "Append content to a file", "append <filename> <content>");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 12, "remove", "Delete a file", "remove <filename>");
    printf("--------------------------------------------------------------------------------------------------\n");

    printf("\t----- PROCESS COMMANDS -----\n");
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 13, "bg", "Run a process in the background", "bg <command>");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 14, "fg", "Bring background process to foreground", "fg <command>");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 15, "list", "List running processes", "list");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 16, "stop", "Stop a process by PID", "stop <pid>");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 17, "resume", "Resume a process by PID", "resume <pid>");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 18, "kill", "Kill a process by PID", "kill <pid>");
    printf("--------------------------------------------------------------------------------------------------\n");

    printf("\t----- ENVIRONMENT VARIABLES COMMANDS -----\n");
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 19, "showpath", "Show environment variable PATH", "showpath");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 20, "addpath", "Add a new path to PATH", "addpath <path>");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 21, "removepath", "Remove a path from PATH", "removepath <path>");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 22, "updatepath", "Update a path in PATH", "updatepath <oldPath> <newPath>");
    printf("--------------------------------------------------------------------------------------------------\n");

    printf("\t----- .BAT FILE COMMAND -----\n");
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 23, "bat", "Run commands from a .bat file", "bat <filename>");
    printf("--------------------------------------------------------------------------------------------------\n");

    printf("\t----- HELP COMMAND -----\n");
    printf("--------------------------------------------------------------------------------------------------\n");
    printf("| %-3d | %-10s | %-50s | %-30s |\n", 24, "help", "Print out all commands available", "help");
    printf("--------------------------------------------------------------------------------------------------\n");
}

