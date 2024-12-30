#include "include/process.h"
#include "include/path.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <signal.h>
#include <tlhelp32.h>
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
void runForeground(ProcessList* list, const char* cmdName, EnvTable *envTable) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!isCommandInPath(envTable, cmdName)) {
        return;
    }

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


//Hàm chạy ở background
void runBackground(ProcessList* list, const char* cmdName, EnvTable *envTable) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!isCommandInPath(envTable, cmdName)) {
        return;
    }

    if (!CreateProcess(NULL, (LPSTR)cmdName, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed (%lu).\n", GetLastError());
        return;
    }

    addProcess(list, pi.dwProcessId, pi.hProcess, cmdName, 1);
    printf("Background process (PID: %lu) started.\n", pi.dwProcessId);

    CloseHandle(pi.hThread);  
}

//Kiểm tra xem tiến trình background đã kết thúc chưa. Nếu kết thúc rồi thì xóa ra khỏi list
void checkBackgroundProcesses(ProcessList *list) {
    for (int i = 0; i < list->count; i++) {
        if (list->processes[i].isBackground) {
            DWORD exitCode;
            if (GetExitCodeProcess(list->processes[i].hProcess, &exitCode)) {
                if (exitCode != STILL_ACTIVE) {
                    printf("Background process (PID: %lu) has finished.\n", list->processes[i].processID);
                    removeProcess(list, list->processes[i].processID);
                    CloseHandle(list->processes[i].hProcess);
                }
            } else {
                printf("Failed to get exit code for PID: %lu\n", list->processes[i].processID);
            }
        }
    }
}

//Hàm update trạng thái (trừ khi đã hoàn thành) của process
void updateProcessStatus(ProcessList* list, DWORD processID, const char* newStatus) {
    for (int i = 0; i < list->count; i++) {
        if (list->processes[i].processID == processID) {
            strcpy(list->processes[i].status, newStatus);
            return;
        }
    }
    printf("Process with PID: %lu not found.\n", processID);
}

//Hàm stop/suspend process
void stopProcess(ProcessList *list, DWORD processID) {
    for (int i = 0; i < list->count; i++) {
        if (list->processes[i].processID == processID) {
            if (list->processes[i].isBackground) {
                // Dừng tiến trình nền (background)
                HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
                if (hSnapshot == INVALID_HANDLE_VALUE) {
                    printf("Failed to create snapshot. Error: %lu\n", GetLastError());
                    return;
                }

                THREADENTRY32 te32;
                te32.dwSize = sizeof(THREADENTRY32);

                // Duyệt qua các thread của tiến trình
                if (Thread32First(hSnapshot, &te32)) {
                    do {
                        if (te32.th32OwnerProcessID == processID) {
                            // Tạm dừng tất cả các thread của tiến trình
                            HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
                            if (hThread != NULL) {
                                SuspendThread(hThread);  // Tạm dừng thread
                                CloseHandle(hThread);
                            }
                        }
                    } while (Thread32Next(hSnapshot, &te32));
                }

                CloseHandle(hSnapshot);
                strcpy(list->processes[i].status, "Stopped");
                printf("Background process (PID: %lu) stopped.\n", processID);
            } else {
                printf("Foreground process (PID: %lu) cannot be stopped.\n", processID);
            }
            return;
        }
    }
    printf("Process with PID: %lu not found.\n", processID);
}

//Hàm để kill process
void killProcess(ProcessList *list, DWORD processID) {
    for (int i = 0; i < list->count; i++) {
        if (list->processes[i].processID == processID) {
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


// Hàm resume process
void resumeProcess(ProcessList *list, DWORD processID) {
    for (int i = 0; i < list->count; i++) {
        if (list->processes[i].processID == processID) {
            if (list->processes[i].isBackground) {
                // Tiếp tục tiến trình nền (background)
                HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
                if (hSnapshot == INVALID_HANDLE_VALUE) {
                    printf("Failed to create snapshot. Error: %lu\n", GetLastError());
                    return;
                }

                THREADENTRY32 te32;
                te32.dwSize = sizeof(THREADENTRY32);

                // Duyệt qua các thread của tiến trình
                if (Thread32First(hSnapshot, &te32)) {
                    do {
                        if (te32.th32OwnerProcessID == processID) {
                            // Tiếp tục tất cả các thread của tiến trình
                            HANDLE hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, te32.th32ThreadID);
                            if (hThread != NULL) {
                                ResumeThread(hThread);  // Tiếp tục thread
                                CloseHandle(hThread);
                            }
                        }
                    } while (Thread32Next(hSnapshot, &te32));
                }

                CloseHandle(hSnapshot);
                strcpy(list->processes[i].status, "Running");
                printf("Background process (PID: %lu) resumed.\n", processID);
            } else {
                printf("Foreground process (PID: %lu) cannot be resumed.\n", processID);
            }
            return;
        }
    }
    printf("Process with PID: %lu not found.\n", processID);
}



