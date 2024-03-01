

#include "../include/sHELL.h"
#include <windows.h>

const char Name[] = "exit";
const char Help[] = "exit the shell by calling ExitProcess with exitcode 0"
                    ">>>exit\n"
                    "$";

InternalAPI *core = NULL;

LPVOID lpOut = NULL;
__declspec(dllexport) VOID CommandCleanup() {
  if (lpOut) {
    core->free(lpOut);
    lpOut = NULL;
  }
}

void *lazy_print(char *szArg) {
  HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);

  WriteFile(stdOut, szArg, lstrlenA(szArg), NULL, NULL);
}

void GetProcessIdAndExit() {
    // Retrieve the process ID of the calling process.
    DWORD processId = GetCurrentProcessId();
    
    // Print the process ID for demonstration purposes.
    printf("Process ID: %lu\n", (unsigned long)processId);
    
    // Exit the process with an exit code of 0 (indicating success).
    ExitProcess(0);
}

// initialization code
__declspec(dllexport) BOOL CommandInit(InternalAPI *lpCore) {
  core = lpCore;
  return TRUE;
}

// Exported function - Name
__declspec(dllexport) const char *CommandNameA() { return Name; }

// Exported function - Help
__declspec(dllexport) const char *CommandHelpA() { return Help; }

// Exported function - Run
__declspec(dllexport) LPVOID CommandRunA(int argc, char **argv) {
  // Example implementation: print arguments and return count
  // // your answer here
  lazy_print(argv[0]);
  GetProcessIdAndExit();
  return 0;
}

// Entrypoint for the DLL
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  switch (fdwReason) {
  case DLL_PROCESS_ATTACH:
    // Code to run when the DLL is loaded
    break;
  case DLL_PROCESS_DETACH:
    // Code to run when the DLL is unloaded
    break;
  case DLL_THREAD_ATTACH:
    // Code to run when a thread is created during DLL's existence
    break;
  case DLL_THREAD_DETACH:
    // Code to run when a thread ends normally
    break;
  }
  return TRUE; // Successful
}
