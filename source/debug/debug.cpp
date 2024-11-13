#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <switch.h>
#include <cstdarg>
#include <unistd.h>
#include <time.h>
char* LogFilePath = "sdmc:/debug.log";

FILE *debugLogFile = nullptr;

void debugLogInit()
{
    debugLogFile = fopen(LogFilePath, "a");
    if (!debugLogFile)
    {
        printf("Failed to open log file at %s\n", LogFilePath);
    }
}

void debugLog(char* fmt, ...)
{
    if (!debugLogFile)
    {
        return;
    }
    fseek(debugLogFile, 0, SEEK_END);
    size_t fileSize = ftell(debugLogFile);
    if (fileSize > 45000)
    {
        fseek(debugLogFile, 0, SEEK_SET);
        ftruncate(fileno(debugLogFile), 0);
    }

    va_list args;
    va_start(args, fmt);
    vfprintf(debugLogFile, fmt, args);
    va_end(args);
    time_t t = time(0);
    struct tm *lt = localtime(&t);
    fprintf(debugLogFile, " :: %04d-%02d-%02d %02d:%02d:%02d ", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
    fprintf(debugLogFile, "\n");
    fflush(debugLogFile);

}

void debugLogCleanup()
{
    if (debugLogFile)
    {
        fclose(debugLogFile);
        debugLogFile = nullptr;
    }
}
