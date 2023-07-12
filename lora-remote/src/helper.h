#pragma once
#include <Arduino.h>

char* copyCharRange(const char* src, int start, int end);
int scanForBreak(int* results, const char* src, int srcLength, const char* breakString, int maxLength);