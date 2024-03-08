#include "minwindef.h"
#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
BOOL Base64Encode(const char *input, char **output, DWORD *outputSize) {
  // // your answer here
      // Check if input is NULL
    
  DWORD inputSize = strlen(input);
  if (input == NULL || output == NULL || outputSize == NULL) {
      return FALSE;
  }

  // Get output size
  BOOL a = CryptBinaryToString(input, 3, CRYPT_STRING_BASE64, NULL, outputSize);
  if(!a) {
    return FALSE;
  }

  // Allocate memory for the base64 encoded string
  *output = (BYTE *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, *outputSize);
  if (*output == NULL) {
    // Memory allocation failed
    return FALSE;
  }

  // Convert binary data to a base64 encoded string
  if (!CryptBinaryToString((BYTE *)input, inputSize, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, (LPSTR)*output, outputSize)) {
    HeapFree(GetProcessHeap(), 0, *output);
    *output = NULL;
    return FALSE;
  }

  return TRUE;
}

int main(int argc, char** argv) {
    char *input = "abc";
    char *encodedString = NULL;
    DWORD encodedSize = 0;

    if (Base64Encode(input, &encodedString, &encodedSize)) {
        printf("Encoded String: %s\n", encodedString);
        // Free the allocated memory for the encoded string
        HeapFree(GetProcessHeap(), 0, encodedString);
    } else {
        printf("Encoding failed.\n");
    }

    return 0;
}