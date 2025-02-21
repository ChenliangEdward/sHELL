#include <windows.h>
#include <wincrypt.h>
#include "../../src/utils.h"
#include "../include/sHELL.h"
// TODO: Fix linking problem
size_t strlen(const char *str) {
  const char *s;
  for (s = str; *s; ++s)
    ;
  return (s - str);
}
const char Name[] = "b64";
const char Help[] =
    "Base64 encode or decode input.\n"
    "Usage:\n"
    "    b64 encode <input> - Encodes the input string\n"
    "    b64 decode <input> - Decodes the base64 encoded string";

InternalAPI *core = NULL;

LPVOID lpOut = NULL;

// Initialization code
__declspec(dllexport) BOOL CommandInit(InternalAPI *lpCore) { core = lpCore; }

// Exported function - Name
__declspec(dllexport) const char *CommandNameA() { return Name; }

// Exported function - Help
__declspec(dllexport) const char *CommandHelpA() { return Help; }

// Clenaup
__declspec(dllexport) VOID CommandCleanup() {
  if (lpOut) {
    core->free(lpOut);
    lpOut = NULL;
  }
}

// Utility function to encode a string to base64
BOOL Base64Encode(const char *input, char **output, DWORD *outputSize) {
  // // your answer here
  
  // Check if input is NULL
  DWORD inputSize = strlen(input);
  if (input == NULL || output == NULL || outputSize == NULL) {
      return FALSE;
  }

  // Get output size
  if(!CryptBinaryToStringA(input, inputSize, CRYPT_STRING_BASE64, NULL, outputSize)) {
    return FALSE;
  }

  // Allocate memory for the base64 encoded string
  *output = (char*)VirtualAlloc(NULL, *outputSize, MEM_COMMIT, PAGE_READWRITE);
  if (*output == NULL) {
    // Memory allocation failed
    return FALSE;
  }

  // Convert binary data to a base64 encoded string
  if (!CryptBinaryToStringA((BYTE *)input, inputSize, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, *output, outputSize)) {
    VirtualFree(*output, 0, MEM_RELEASE);
    *output = NULL;
    return FALSE;
  }

  return TRUE;
}

// Utility function to decode a base64 string
BOOL Base64Decode(const char *input, BYTE **output, DWORD *outputSize) {
  // // your answer here
  if (input == NULL || output == NULL || outputSize == NULL) {
      return FALSE; // Invalid parameters
  }

  // Calculate the required size for the decoded output.
  *outputSize = 0; // Initialize output size
  if (!CryptStringToBinaryA(input, 0, CRYPT_STRING_BASE64, NULL, outputSize, NULL, NULL)) {
      return FALSE; // Failed to calculate the size
  }

  // Allocate memory for the decoded output
  *output = (BYTE *)VirtualAlloc(NULL, *outputSize, MEM_COMMIT, PAGE_READWRITE);
  if (*output == NULL) {
      return FALSE; // Memory allocation failed
  }

  // Perform the actual decoding
  if (!CryptStringToBinaryA(input, 0, CRYPT_STRING_BASE64, *output, outputSize, NULL, NULL)) {
      VirtualFree(*output, 0, MEM_RELEASE); // Free allocated memory on failure
      *output = NULL; // Avoid dangling pointer
      return FALSE; // Decoding failed
  }

  // Decoding was successful
  return TRUE;
}

// Exported function - Run
__declspec(dllexport) LPVOID CommandRunA(int argc, char **argv) {
  if (argc != 3) {
    core->wprintf(L"Invalid arguments.\n%s", CommandHelpA());
    return NULL; // Error code for invalid arguments
  }

  if (core->strcmp(argv[1], "encode") == 0) {
    char *encodedString = NULL;
    DWORD encodedSize = 0;
    if (Base64Encode(argv[2], &encodedString, &encodedSize)) {
      core->wprintf(L"%S\n", encodedString);
      lpOut = (LPVOID)encodedString;

    } else {
      core->wprintf(L"Error encoding string.\n");
      return (LPVOID)NULL;
    }
  } else if (core->strcmp(argv[1], "decode") == 0) {
    BYTE *decodedBytes = NULL;
    DWORD decodedSize = 0;
    if (Base64Decode(argv[2], &decodedBytes, &decodedSize)) {
      // Assuming decoded output is also an ANSI string
      core->wprintf(L"%.*S\n", decodedSize, decodedBytes);
      // core->free(decodedBytes);
      lpOut = (LPVOID)decodedBytes;
    } else {
      core->wprintf(L"Error decoding string.\n");
      return NULL;
    }
  } else {
    core->wprintf(L"Invalid command. Use 'encode' or 'decode'.\n");
    return NULL; // Error code for invalid command
  }
  core->wprintf(L"I have returned from sHELL\n");
  return lpOut; // Success
}

// Entrypoint for the DLL
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  switch (fdwReason) {
  case DLL_PROCESS_ATTACH:
    // Initialization code for when the DLL is loaded
    break;
  case DLL_PROCESS_DETACH:
    // Cleanup code for when the DLL is unloaded
    break;
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
    // Thread-specific initialization or cleanup
    break;
  }
  return TRUE; // Indicate successful DLL process attachment or detachment
}
