#ifndef SHELL_H
#define SHELL_H

#include "process.h"

void printDate();
void printTime();
void printPwd();
void makeDir(const char * folderName);
void removeDir(const char * folderName);
void listDirectoryTree(const char *path);
void changeDirectory(const char * path);
void createFile(const char *filename);
void readFile(const char *filename);
void writeFile(const char *filename, const char *content);
void appendFile(const char *filename, const char *content);
void removeFile(const char *filename);
char*processEscapeSequences(const char* input);

#endif

