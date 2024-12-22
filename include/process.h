#ifndef PROCESS_H
#define PROCESS_H

#include <windows.h>

typedef struct {
    DWORD processID;
    HANDLE hProcess;
    char cmdName[256];
    int isBackground;
} ProcessInfo;

typedef struct{
    ProcessInfo * processes;
    int count;
    int capacity;
} ProcessList;


void initProcessList(ProcessList * list);
void addProcess(ProcessList * list, DWORD processID, HANDLE hProcess, const char * cmdName, int isBackground);
void removeProcess(ProcessList * list, DWORD processID, HANDLE hProcess, const char * cmdName, int isBackground);
void listProcess(ProcessList * list);
int killProcess(ProcessList * list, DWORD processID);
ProcessInfo * findProcess(ProcessList * list, DWORD processID);
void cleanProcessList(ProcessList * list);

#endif