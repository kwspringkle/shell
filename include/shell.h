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

#endif
