#ifndef AUDIOCOMPONENT_H_
#define AUDIOCOMPONENT_H_
#include <GameObject.h>
// Include sdl2 headers
#include <SDL.h>
#include <SDL_mixer.h>
#include "../debug/debug.h"
class AudioComponent : public GameComponent
{
    private:
    Mix_Music *audio;
    Mix_Chunk *sfx;
    bool isSFX = false;
    bool isPlaying = true;
    bool loop;
    //Audio data
    public:
    void OnUpdate() override
    {
        return;
    };
    void ComponentAddedCallback() override
    {
        return;
    };
    bool IsPlaying();
    void PlayAudio(int loops = -1)
    {
        if(isSFX)
        {
            PlaySFX();
        }
        else
        {
            Mix_PlayMusic(audio, loops);

        }
        isPlaying = true;
    };
    void PlaySFX()
    {
        if(loop)
        {
            Mix_PlayChannel(1, sfx, -1);

        }
        else
        {
            Mix_PlayChannel(1, sfx, 0);

        }
    }
    void StopAudio()
    {
        if(isSFX)
        {
            StopSFX();
        }
        else
        {
            Mix_HaltMusic();
        }
        isPlaying = false;
       
    };
    void StopSFX()
    {
        Mix_HaltChannel(1);
    };
    void PauseAudio()
    {
        Mix_PauseMusic();
    };
    void ResumeAudio()
    {
        Mix_ResumeMusic();
    };
    AudioComponent(const char* audioFile)
    {
        Mix_Init(MIX_INIT_MP3);
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
        debugLog("Audio File: %s", audioFile);
        audio = Mix_LoadMUS(audioFile);
        if(audio == NULL)
        {
            debugLog("Failed to load audio file: %s", audioFile);
        }
        isSFX = false;
        
    };
    AudioComponent(int freq, Uint16 format, int channels, int buffersize, const char* audioSFXFile, bool loop) 
    {
        Mix_OpenAudio(freq, format, channels, buffersize);
        sfx = Mix_LoadWAV(audioSFXFile);
        isSFX = true;
    };
    ~AudioComponent()
    {
        Mix_FreeMusic(audio);
        Mix_FreeChunk(sfx);
        Mix_CloseAudio();
        SDL_Quit();

    };

};


#endif /* AUDIOCOMPONENT_H_ */
