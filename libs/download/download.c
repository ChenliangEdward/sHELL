
#include "../include/sHELL.h"
#include "errhandlingapi.h"
#include <windows.h>
#include <urlmon.h>

const char Name[] = "download";
const char Help[] =
    "Download a file from a specified URL to the local filesystem.\n"
    "Usage:\n"
    "    download <URL> <local file path>";

InternalAPI *core = NULL;

LPVOID lpOut = NULL;
__declspec(dllexport) VOID CommandCleanup() {
  if (lpOut) {
    core->free(lpOut);
    lpOut = NULL;
  }
}
// helper Function
void *lazy_print(char *szArg) {
  HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);

  WriteFile(stdOut, szArg, lstrlenA(szArg), NULL, NULL);
}

BOOL DownloadFile(const char* url, const char* localFilePath) {
    return URLDownloadToFile(NULL, url, localFilePath, 0, NULL);

}

// initialization code
__declspec(dllexport) BOOL CommandInit(InternalAPI *lpCore) { core = lpCore; }

// Exported function - Name
__declspec(dllexport) const char *CommandNameA() { return Name; }

// Exported function - Help
__declspec(dllexport) const char *CommandHelpA() { return Help; }

// Exported function - Run
__declspec(dllexport) LPVOID CommandRunA(int argc, char **argv) {
  // Example implementation: print arguments and return count
  if (argc != 3) {
    core->wprintf(L"Invalid arguments.\n%s", CommandHelpA());
    return (LPVOID)1; // Error code for invalid arguments
  }
  // // your answer here
  if (DownloadFile(argv[1], argv[2]) == S_OK){
    lazy_print("Good work, download successful!\n");
  } else {
    lazy_print("sHELL NAWWW... You failed your deeds\n");
    ExitProcess(1);
  }
  return (LPVOID)1; // Success
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
