#ifndef GX_VIDEPLAYER_H
#define GX_VIDEPLAYER_H
#pragma once
#define __STDC_CONSTANT_MACROS
extern "C"
{
    #include <libavutil/avutil.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/opt.h>
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
    #include <libswresample/swresample.h>
    #include <nanojpeg.h>
}
#include <switch.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
using namespace std;
namespace Player
{
    void playbackInit(string VideoPath);
    bool playbackLoop();
    void playbackPause();
    void playbackResume();
    void playbackPauseResume();
    void playbackStop();
    void playbackExit();
    void playbackSetFastForward(int Power);
    void playbackThrowError(string Error);
    vector<string> getVideoFiles();
}
#endif // GX_VIDEPLAYER_H
