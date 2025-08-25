#pragma once
#define BASS_STATIC
#include <bass.h>
#include <string>
#include <iostream>
#include "Global.h"

class Sound{
    public:
        Sound(const std::string & filename);
        Sound();
        ~Sound();
        void play();
        void pause();
        void stop();
        void setVolume(const float & volume);
        //static void setGlobalVolume(const int & vol) { volume = std::clamp(vol, 0, 100); }
        static int volume;
        static Sound* __backgroundMusic;
    private:
        unsigned int channel;
        static bool audio_device;
        bool isPlaying;
        bool isAvailable;
        
};