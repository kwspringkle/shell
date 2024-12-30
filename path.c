#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "include/path.h"

// Khởi tạo bảng biến môi trường nội bộ
void initEnvTable(EnvTable *envTable) {
    envTable->count = 0;
}


// Hàm thêm biến vào bảng
void addEnvVariable(EnvTable *envTable, const char *variable) {
    if (envTable->count >= 100) {
        printf("Environment table is full!\n");
        return;
    }
    strcpy(envTable->variables[envTable->count], variable);
    envTable->count++;
}

// Hàm hiển thị giá trị biến `PATH`
void showPath(const EnvTable *envTable) {
    printf("Current PATH: ");
    for (int i = 0; i < envTable->count; i++) {
        printf("%s", envTable->variables[i]);
        if (i < envTable->count - 1) {
            printf(";");
        }
    }
    printf("\n");
}

// Hàm thêm giá trị vào biến `PATH`
void addPath(EnvTable *envTable, const char *newPath) {
    for (int i = 0; i < envTable->count; i++) {
        if (strcmp(envTable->variables[i], newPath) == 0) {
            printf("Path '%s' already exists in the environment table.\n", newPath);
            return;
        }
    }

    addEnvVariable(envTable, newPath);
    printf("Added '%s' to PATH.\n", newPath);
}

// Hàm xóa giá trị khỏi biến PATH
void removePath(EnvTable *envTable, const char *pathToRemove) {
    int found = 0;
    for (int i = 0; i < envTable->count; i++) {
        if (strcmp(envTable->variables[i], pathToRemove) == 0) {
            found = 1;
            // Dịch chuyển các phần tử còn lại lên
            for (int j = i; j < envTable->count - 1; j++) {
                strcpy(envTable->variables[j], envTable->variables[j + 1]);
            }
            envTable->count--;
            printf("Removed '%s' from PATH.\n", pathToRemove);
            break;
        }
    }

    if (!found) {
        printf("Path '%s' not found in the environment table.\n", pathToRemove);
    }
}

// Hàm cập nhật giá trị PATH
void updatePath(EnvTable *envTable, const char *oldPath, const char *newPath) {
    int found = 0;
    for (int i = 0; i < envTable->count; i++) {
        if (strcmp(envTable->variables[i], oldPath) == 0) {
            strcpy(envTable->variables[i], newPath);
            found = 1;
            printf("Updated PATH: '%s' -> '%s'\n", oldPath, newPath);
            break;
        }
    }

    if (!found) {
        printf("Path '%s' not found in the environment table.\n", oldPath);
    }
}

int isCommandInPath(const EnvTable *envTable, const char *cmdName){
    for (int i = 0; i < envTable->count; i++) {
        char fullPath[MAX_PATH];
        snprintf(fullPath, MAX_PATH, "%s\\%s", envTable->variables[i], cmdName);

        if (GetFileAttributes(fullPath) != INVALID_FILE_ATTRIBUTES) {
            return 1; 
        }
    }
    return 0; 
}