#ifndef SHELL_H
#define SHELL_H

#include "process.h"

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType);
void runShell();
void executeCommand(const char* command, ProcessList* processList);
void executeBatchFile(const char* filename, ProcessList* processList);
void handleBuiltInCommands(const char* command, ProcessList* processList);

#endif
