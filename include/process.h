#ifndef PROCESS_H
#define PROCESS_H

#include <windows.h>

typedef struct {
    DWORD processID;
    HANDLE hProcess;
    char cmdName[256];
    char status[20];
    int isBackground;
} ProcessInfo;

typedef struct{
    ProcessInfo * processes;
    int count;
    int capacity;
} ProcessList;


void initProcessList(ProcessList * list);
void addProcess(ProcessList * list, DWORD processID, HANDLE hProcess, const char * cmdName, int isBackground);
void removeProcess(ProcessList * list, DWORD processID);
void listProcess(ProcessList * list);
int killProcess(ProcessList * list, DWORD processID);
void SuspendProcessById(ProcessList *processList, DWORD processID);
void ResumeProcessById(ProcessList *processList, DWORD processID);

ProcessInfo * findProcess(ProcessList * list, DWORD processID);
void cleanProcessList(ProcessList * list);

#endif