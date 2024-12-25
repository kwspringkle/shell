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