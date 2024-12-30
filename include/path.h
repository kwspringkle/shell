#ifndef PATH_H
#define PATH_H

// Bảng biến môi trường nội bộ, không dùng của hệ điều hành
typedef struct {
    char variables[100][256];  //100: số lượng tối đa biến môi trường; 256: độ dài của biến môi trườngtrường
    int count;                // Số lượng biến
} EnvTable;

void initEnvTable(EnvTable *envTable);
void addEnvVariable(EnvTable *envTable, const char *variable);
void showPath(const EnvTable *envTable);
void addPath(EnvTable *envTable, const char *newPath);
void removePath(EnvTable *envTable, const char *pathToRemove);
void updatePath(EnvTable *envTable, const char *oldPath, const char *newPath);
int isCommandInPath(const EnvTable *envTable, const char *cmdName);

#endif