#include "UI/Sound.h"
#include "Global.h"

bool Sound::audio_device = false;
int Sound::volume = 70;
Sound* Sound::__backgroundMusic;
Sound::Sound(const std::string & filename) : isPlaying(false) {
    isAvailable = true;
    if(!audio_device){
        if(!BASS_Init(-1, 44100, 0, nullptr, nullptr)){
            throw std::runtime_error("Failed to initialize audio device");
        }
        audio_device = true;
        isAvailable = true;
    }

    channel = BASS_StreamCreateFile(FALSE, filename.c_str(), 0, 0, BASS_SAMPLE_LOOP);
    if(!channel){
        throw std::runtime_error("Failed to create sound stream: " + filename);
        isAvailable = false;
    }
    setVolume(volume);
    std::cout << "Loaded sound: " << filename << "\n";
    std::cout<<"isAvailable cons: "<<isAvailable<<"\n";
    play();
}

Sound::Sound() : isPlaying(false){
    isAvailable = false;
}

Sound::~Sound(){
    BASS_Free();
}

void Sound::play(){
    //std::cout<<"isPlaying: "<<isPlaying<<", isAvailable: "<<isAvailable<<"\n";
    if(!isPlaying && isAvailable){
        BASS_ChannelPlay(channel, FALSE);
        isPlaying = true;
    }
    else{
        if(!isAvailable) std::cout<<"Sound not available\n";
        
    }
}

void Sound::pause(){
    if(isPlaying){
        BASS_ChannelPause(channel);
        isPlaying = false;
    }
}   

void Sound::stop(){
    if(isPlaying){
        BASS_ChannelStop(channel);
        isPlaying = false;
    }
}

void Sound::setVolume(const float & volume){
    float vol = std::clamp(volume, 0.f, 100.f);
    BASS_ChannelSetAttribute(channel, BASS_ATTRIB_VOL, vol / 100.f);
}