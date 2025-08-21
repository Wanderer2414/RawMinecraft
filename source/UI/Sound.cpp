#include "UI/Sound.h"

bool Sound::audio_device = false;

Sound::Sound(const std::string & filename) {
    if(!audio_device){
        if(!BASS_Init(-1, 44100, 0, nullptr, nullptr)){
            throw std::runtime_error("Failed to initialize audio device");
        }
        audio_device = true;
    }

    channel = BASS_StreamCreateFile(FALSE, filename.c_str(), 0, 0, BASS_SAMPLE_LOOP);
    if(!channel){
        throw std::runtime_error("Failed to create sound stream: " + filename);

    }
}

Sound::~Sound(){
    BASS_Free();
}

void Sound::play(){
    BASS_ChannelPlay(channel, FALSE);
}

void Sound::pause(){
    BASS_ChannelPause(channel);
}   

void Sound::stop(){
    BASS_ChannelStop(channel);
}