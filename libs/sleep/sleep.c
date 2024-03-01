

#include "../include/sHELL.h"
#include <shlwapi.h>
#include <windows.h>

const char Name[] = "sleep";
const char Help[] = "Pause execution (sleep) for the current thread."
                    "Example Sleeping for 1 second:"
                    ">>>sleep 1000 "
                    ">>>";

InternalAPI *core = NULL;

LPVOID lpOut = NULL;
__declspec(dllexport) VOID CommandCleanup() {
  if (lpOut) {
    core->free(lpOut);
    lpOut = NULL;
  }
}
// initialization code
__declspec(dllexport) BOOL CommandInit(InternalAPI *lpCore) {
  core = lpCore;
  return TRUE;
}

// Helper functions
void *lazy_print(char *szArg) {
  HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);

  WriteFile(stdOut, szArg, lstrlenA(szArg), NULL, NULL);
}

DWORD ConvertToDWORD(const char* str) {
    DWORD dwValue = 0;
    INT intValue = 0;
    BOOL success = StrToIntEx(str, STIF_DEFAULT, &intValue);

    // Check if conversion succeeded and the value is within the DWORD range
    if (success && intValue >= 0) {
        dwValue = (DWORD)intValue;
    } else {
        // Handle non-numeric strings or negative numbers by aborting
        lazy_print("What in the name of sHELL did you just do?!\n");
        ExitProcess(1); // Aborting
    }

    return dwValue;
}

// Exported function - Name
__declspec(dllexport) const char *CommandNameA() { return Name; }

// Exported function - Help
__declspec(dllexport) const char *CommandHelpA() { return Help; }

// Exported function - Run
__declspec(dllexport) LPVOID CommandRunA(int argc, char **argv) {
  if (argc != 2) {
    core->wprintf(L"Invalid input:\n %S\n", Help);
    return NULL;
  }
  Sleep(ConvertToDWORD(argv[1]));
  return (LPVOID)1;
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
