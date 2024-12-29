#ifndef PROCESS_H
#define PROCESS_H

#include <windows.h>
#include <stdbool.h>

typedef struct {
    DWORD processID;
    HANDLE hProcess;
    char cmdName[256];
    char status[20];
    int isBackground;
} Process;

typedef struct {
    Process processes[100];
    int count;
}ProcessList;

void addProcess(ProcessList * list, DWORD processID, HANDLE hProcess, const char * command, int isBackground);
void removeProcess(ProcessList* list, DWORD processID);
void listProcesses(const ProcessList* list);
void runForeground(ProcessList* list, const char* cmdName);
void runBackground(ProcessList* list, const char* cmdName);
void updateProcessStatus(ProcessList* list, DWORD processID, const char* newStatus);
void stopProcess(ProcessList* list, DWORD processID);
void killProcess(ProcessList* list, DWORD processID);
void stopForeground(ProcessList * list, int signal);
void resumeProcess(ProcessList* list, DWORD processID);
void checkBackgroundProcesses(ProcessList *list);
#endif