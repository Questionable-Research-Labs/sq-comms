#include <helper.h>

// Have to call delete[] on the result of this function
char* copyCharRange(const char* src, int start, int end) {
    int length = end - start + 1;
    char* result = new char[length];
    memcpy(result, &src[start], length);
    result[length - 1] = '\0';
    return result;
}

// Scan for sub string '[;]' in a char array, and return all results
int scanForBreak(int* results, const char* src, int srcLength, const char* breakString, int maxLength) {
    int breakLength = strlen(breakString);
    int resultIndex = 0;
    for (int i = 0; i < srcLength; i++) {
        if (strncmp(&src[i], breakString, breakLength) == 0) {
            results[resultIndex] = i;
            resultIndex++;
        }
        if (resultIndex >= maxLength) {
            return resultIndex;
        }
    }
    return resultIndex;
}