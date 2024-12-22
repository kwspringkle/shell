#include "include/process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Khởi tạo danh sách tiến trình để quản lý
void initProcessList(ProcessList * list){
    list->capacity=10;
    list->count=0;
    list->processes = (ProcessInfo*)malloc(sizeof(ProcessInfo) *list->capacity);
}

//Hàm để tạo 1 tiến trình mới
void addProcess(ProcessList * list, DWORD processID, HANDLE hProcess, const char * cmdName, int isBackground){
    if(list->count >= list->capacity){
        list->capacity *=2;
        list->processes = (ProcessInfo*)malloc(sizeof(ProcessInfo) *list->capacity);
    }

    ProcessInfo * process = &list->processes[list->count];
    process->processID = processID;
    process->hProcess = hProcess;
    strncpy(process->cmdName, cmdName, 255);
    process->cmdName[255] = '\0';
    //strncpy(process->status, status, 20);
    //process->status[20] = '\0';
    process->isBackground = isBackground;

    list->count++;
}

//Hàm xóa tiến trình
void removeProcess(ProcessList * list, DWORD processID){
    for(int i = 0; i < list->count; i++){
        if(list->processes[i].processID == processID){
            for (int j = i; j < list->count - 1; j++) {
                list->processes[j] = list->processes[j + 1];
            }
            list->count--;
            break;
        }
    }
}

//Hàm liệt kê tiến trình
void listProcess(ProcessList * list){
    printf("PID\tStatus\t\tBackground\tCommand\n");
    for (int i = 0; i < list->count; i++) {
        DWORD exitCode;
        GetExitCodeProcess(list->processes[i].hProcess, &exitCode);
        printf("%lu\t%s\t\t%s\t\t%s\n", 
            list->processes[i].processID,
            exitCode == STILL_ACTIVE ? "Running" : "Finished",
            list->processes[i].isBackground ? "Yes" : "No",
            list->processes[i].cmdName);
    }
}

