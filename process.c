#include "include/process.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <signal.h>

//Hàm thêm process vào list
void addProcess(ProcessList * list, DWORD processID, HANDLE hProcess, const char * command, int isBackground){
    if(list->count >=100){
        printf("Process list is full! \n");
        return;
    }

    Process * newProcess = &list->processes[list->count];
    newProcess->processID = processID;
    newProcess->hProcess = hProcess;
    strcpy(newProcess->cmdName, command);
    strcpy(newProcess->status, "Running");
    newProcess->isBackground = isBackground;
    list->count++;
}
//Hàm xóa process khỏi list
void removeProcess(ProcessList* list, DWORD processID) {
    for (int i = 0; i < list->count; i++) {
        if (list->processes[i].processID == processID) {
            for (int j = i; j < list->count - 1; j++) {
                list->processes[j] = list->processes[j + 1];
            }
            list->count--;
            return;
        }
    }
    printf("Process with PID: %lu not found.\n", processID);
}
//Hàm list ra các process
void listProcesses(const ProcessList* list) {
    printf("Current processes:\n");
    printf("%-8s %-15s %-20s %-10s\n", "PID", "Status", "Command", "Type"); 
    for (int i = 0; i < list->count; i++) {
    printf("%-8lu %-15s %-20s %-10s\n",
           list->processes[i].processID,
           list->processes[i].status,
           list->processes[i].cmdName,
           list->processes[i].isBackground ? "Background" : "Foreground");
    }
}
//Hàm chạy ở foreground
void runForeground(ProcessList* list, const char* cmdName) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL, (LPSTR)cmdName, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed (%lu).\n", GetLastError());
        return;
    }

    addProcess(list, pi.dwProcessId, pi.hProcess, cmdName, 0);

    // Chờ tiến trình kết thúc
    WaitForSingleObject(pi.hProcess, INFINITE);
    printf("\nForeground process (PID: %lu) finished.\n", pi.dwProcessId);

    removeProcess(list, pi.dwProcessId);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}
//Hàm chạy ở background
void runBackground(ProcessList* list, const char* cmdName) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL, (LPSTR)cmdName, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed (%lu).\n", GetLastError());
        return;
    }

    addProcess(list, pi.dwProcessId, pi.hProcess, cmdName, 1);

    printf("Background process (PID: %lu) started.\n", pi.dwProcessId);

    CloseHandle(pi.hThread);
}
//Hàm update trạng thái của process
void updateProcessStatus(ProcessList* list, DWORD processID, const char* newStatus) {
    for (int i = 0; i < list->count; i++) {
        if (list->processes[i].processID == processID) {
            strcpy(list->processes[i].status, newStatus);
            return;
        }
    }
    printf("Process with PID: %lu not found.\n", processID);
}
//Hàm stop process
void stopProcess(ProcessList* list, DWORD processID) {
    for (int i = 0; i < list->count; i++) {
        if (list->processes[i].processID == processID) {
            if (!list->processes[i].isBackground) {
                printf("Cannot stop foreground process (PID: %lu).\n", processID);
                return;
            }

            if (SuspendThread(list->processes[i].hProcess) != -1) {
                updateProcessStatus(list, processID, "Stopped");
                printf("Process (PID: %lu) stopped.\n", processID);
            } else {
                printf("Failed to stop process (PID: %lu).\n", processID);
            }
            return;
        }
    }
    printf("Process with PID: %lu not found.\n", processID);
}
//Hàm để kill process
void killProcess(ProcessList* list, DWORD processID) {
    for (int i = 0; i < list->count; i++) {
        if (list->processes[i].processID == processID) {
            if (!list->processes[i].isBackground) {
                printf("Cannot stop foreground process (PID: %lu).\n", processID);
                return;
            }

            if (TerminateProcess(list->processes[i].hProcess, 0)) {
                printf("Process (PID: %lu) terminated.\n", processID);
                CloseHandle(list->processes[i].hProcess);
                removeProcess(list, processID);
            } else {
                printf("Failed to terminate process (PID: %lu). Error: %lu\n", processID, GetLastError());
            }
            return;
        }
    }
    printf("Process with PID: %lu not found.\n", processID);
}

// Hàm resume process cho background
void resumeProcess(ProcessList* list, DWORD processID) {
    for (int i = 0; i < list->count; i++) {
        if (list->processes[i].processID == processID) {
            // Kiểm tra xem có phải background process không
            if (!list->processes[i].isBackground) {
                printf("Cannot resume foreground process (PID: %lu).\n", processID);
                return;
            }
            
            if (ResumeThread(list->processes[i].hProcess) != -1) {
                updateProcessStatus(list, processID, "Running");
                printf("Background process (PID: %lu) resumed.\n", processID);
            } else {
                printf("Failed to resume background process (PID: %lu).\n", processID);
            }
            return;
        }
    }
    printf("Process with PID: %lu not found.\n", processID);
}

//Ctrl + C
void stopForeground(ProcessList * list, int signal){
    printf("\nCtrl + C pressed. Stop the process...\n");
    for(int i = 0; i < list->count; i++){
        if(!list->processes[i].isBackground){
            if(TerminateProcess(list->processes[i].hProcess, 0)){
                printf("Foreground process (PID: %lu) terminated. \n",list->processes[i].processID);
                CloseHandle(list->processes[i].hProcess);
                removeProcess(list,list->processes[i].processID);
            }
            return;
        }
     }
     printf("No foreground process running.\n");
}
