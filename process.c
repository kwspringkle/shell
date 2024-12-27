#include "include/process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <tlhelp32.h>

// Hàm tạm dừng tiến trình trong danh sách dựa trên PID
void SuspendProcessById(ProcessList *list, DWORD processID) {
    for (int i = 0; i < list->count; i++) {
        if (list->processes[i].processID == processID) {
            HANDLE hProcess = list->processes[i].hProcess;
            if (hProcess == NULL) {
                printf("Failed to find process with PID %lu.\n", processID);
                return;
            }

            // Tạo snapshot của tất cả các luồng
            HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
            if (hThreadSnap == INVALID_HANDLE_VALUE) {
                printf("Failed to create thread snapshot.\n");
                return;
            }

            THREADENTRY32 te32;
            te32.dwSize = sizeof(THREADENTRY32);

            // Đình chỉ các luồng thuộc tiến trình
            if (Thread32First(hThreadSnap, &te32)) {
                do {
                    if (te32.th32OwnerProcessID == processID) {
                        HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
                        if (hThread != NULL) {
                            SuspendThread(hThread);
                            CloseHandle(hThread);
                        }
                    }
                } while (Thread32Next(hThreadSnap, &te32));
            }

            CloseHandle(hThreadSnap);
            printf("Process with PID %lu has been suspended.\n", processID);
            return;
        }
    }
    printf("Process with PID %lu not found in the list.\n", processID);
}

// Hàm tiếp tục tiến trình trong danh sách dựa trên PID
void ResumeProcessById(ProcessList *list, DWORD processID) {
    for (int i = 0; i < list->count; i++) {
        if (list->processes[i].processID == processID) {
            HANDLE hProcess = list->processes[i].hProcess;
            if (hProcess == NULL) {
                printf("Failed to find process with PID %lu.\n", processID);
                return;
            }

            // Tạo snapshot của tất cả các luồng
            HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
            if (hThreadSnap == INVALID_HANDLE_VALUE) {
                printf("Failed to create thread snapshot.\n");
                return;
            }

            THREADENTRY32 te32;
            te32.dwSize = sizeof(THREADENTRY32);

            // Tiếp tục các luồng thuộc tiến trình
            if (Thread32First(hThreadSnap, &te32)) {
                do {
                    if (te32.th32OwnerProcessID == processID) {
                        HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
                        if (hThread != NULL) {
                            ResumeThread(hThread);
                            CloseHandle(hThread);
                        }
                    }
                } while (Thread32Next(hThreadSnap, &te32));
            }

            CloseHandle(hThreadSnap);
            printf("Process with PID %lu has been resumed.\n", processID);
            return;
        }
    }
    printf("Process with PID %lu not found in the list.\n", processID);
}


//Hàm khởi tạo danh sách tiến trình để quản lý
void initProcessList(ProcessList * list){
    list->capacity=10;
    list->count=0;
    list->processes = (ProcessInfo*)malloc(sizeof(ProcessInfo) *list->capacity);
}

// //Hàm để tạo 1 tiến trình mới
// void addProcess(ProcessList * list, DWORD processID, HANDLE hProcess, const char * cmdName, int isBackground){
//     if(list->count >= list->capacity){
//         list->capacity *=2;
//         list->processes = (ProcessInfo*)malloc(sizeof(ProcessInfo) *list->capacity);
//     }

//     ProcessInfo * process = &list->processes[list->count];
//     process->processID = processID;
//     process->hProcess = hProcess;
//     strncpy(process->cmdName, cmdName, 255);
//     process->cmdName[255] = '\0';
//     //strncpy(process->status, status, 20);
//     //process->status[20] = '\0';
//     process->isBackground = isBackground;

//     list->count++;
// }

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

