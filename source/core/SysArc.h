#ifndef SYSARC_H
#define SYSARC_H
#include <zlib.h>
#include <string.h>


void* DecompresSceneFile(void* data, unsigned int dataSize, unsigned int* outputSize);
#endif