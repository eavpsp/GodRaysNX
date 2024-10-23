#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H
#include <raylib.h>

//must be implemented into render system
//beging texture mode highly specific and should use a flag case
//default PP shall include bloom, blur and anti aliasing
enum PostProcessingFX
{
    BLOOM, BLUR, ANTIALIASING
};

#endif