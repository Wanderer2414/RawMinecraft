#pragma once
#define BASS_STATIC
#include <../bass24/c/bass.h>
#include <string>
#include <iostream>

class Sound{
    public:
        Sound(const std::string & filename);
        ~Sound();
        void play();
        void pause();
        void stop();
    private:
        unsigned int channel;
        static bool audio_device;
};