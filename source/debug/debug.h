
#ifndef DEBUG_H 
#define DEBUG_H

///Take in a string and write it to a file on the sd card
void debugLog(char* fmt, ...);
void debugLogInit();
void debugLogCleanup();

#endif