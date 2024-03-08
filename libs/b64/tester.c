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
  BOOL a = CryptBinaryToStringA(input, inputSize, CRYPT_STRING_BASE64, NULL, outputSize);
  if(!a) {
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

int main(int argc, char** argv) {
    char *input = "abcdefghijabcdefghijkabcdefghijkabcdefghijkabcdefghijkk";
    char *encodedString = NULL;
    DWORD encodedSize = 0;

    if (Base64Encode(input, &encodedString, &encodedSize)) {
        printf("Encoded String: %s\n", encodedString);
        // Free the allocated memory for the encoded string
        VirtualFree(encodedString, 0, MEM_RELEASE);
    } else {
        printf("Encoding failed.\n");
    }

    return 0;
}